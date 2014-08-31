// Name:    playuvApp.cpp
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

// turn off 'warning C4355: 'this' : used in base member initializer list'
#if defined(_WIN32_)
    #pragma warning ( disable:4355 )
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "playuvApp.cpp"
    #pragma interface "playuvApp.cpp"
#endif

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/cmdline.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/html/helpctrl.h>
#include <wx/ipc.h>
#include <wx/ipcbase.h>
#include <wx/panel.h>
#include <wx/propdlg.h>
#include <wx/rawbmp.h>

#include <stdint.h>

// Inclusion of the main header file
#include "playuvFrame.h"
#include "playuvApp.h"

#include "playuv250x200.xpm"

/////////////////////////
// Sequence parameters //
/////////////////////////

/////////////////////////////////////////////////////
//// Application methods
/////////////////////////////////////////////////////

// Description of possible command line parameters
const wxCmdLineEntryDesc pyuvApp::pyuvCommandLine[] =
{
    {wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters"},
    {wxCMD_LINE_SWITCH, "v", "version", "print version"},
    {wxCMD_LINE_OPTION, "d", "debug", "specify a debug level"},
    {wxCMD_LINE_PARAM, NULL, NULL, "input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL},
    {wxCMD_LINE_NONE}
};

wxColour pyuvApp::pyuvTimeColour = wxColour(0x00, 0x00, 0x00);
wxColour pyuvApp::pyuvIdColour = wxColour(0xFF, 0xFF, 0xFF);
wxString pyuvApp::pyuvTopbasetitle = wxT(PYUV_SHORTNAME);
int pyuvApp::pyuvServno = 0;
int pyuvApp::pyuvMyMaster = -1;
bool pyuvApp::pyuvIHaveASlave = false;
bool pyuvApp::pyuvMySlave[MAX_NO_SERVERS];
wxColour pyuvApp::pyuvSlaveColors[MAX_NO_SERVERS];
wxRect pyuvApp::pyuvTimeBack = wxRect();
wxRect pyuvApp::pyuvIdBack = wxRect();

wxString pyuvApp::GetServerNoString(void)
{
    return wxString::Format(wxT("%02d"), pyuvServno);
}

// Initialize the application
bool pyuvApp::OnInit()
{
#if 0
    int res;
#endif

    // Set the English locale
    wxLocale(wxLANGUAGE_ENGLISH, wxLOCALE_LOAD_DEFAULT);

    // Look for existing clients and set up an IPC server
    {
        pyuvClient* tmp_client = new pyuvClient;
        wxConnectionBase* connection = NULL;

        for(pyuvServno = 0; pyuvServno < MAX_NO_SERVERS; pyuvServno++)
        {
            wxLogNull noLog;

            if(!(connection = tmp_client->MakeConnection(wxEmptyString, wxString::Format(wxT("/tmp/PYUV%02d"), pyuvServno), wxT("intro"))))
            {
                break;
            }
            else
            {
                connection->Disconnect();
                connection = NULL;
            }
        }

        delete connection;
        delete tmp_client;
    }

    // Create the main application window
    frame = new pyuvFrame(pyuvTopbasetitle + wxT(" - a portable YUV player"));

    if(pyuvServno < MAX_NO_SERVERS)
    {
        m_server = new pyuvServer(frame);

        if(!m_server->Create(wxString::Format(wxT("/tmp/PYUV%02d"), pyuvServno)))
        {
            wxLogDebug(wxT("Failed to create an IPC service."));
        }
    }
    else
    {
        wxLogDebug(wxT("Increase number of IPC servers."));
    }

    // Init colors for slaves background
    int i = 0;
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(*wxRED);
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(*wxBLUE);
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(*wxGREEN);
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(*wxCYAN);
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(wxString(wxT("ORANGE")));
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(wxString(wxT("SKY BLUE")));
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(wxString(wxT("GOLD")));
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(wxString(wxT("MAGENTA")));
    }
    if(i < MAX_NO_SERVERS)
    {
        pyuvSlaveColors[i++] = wxColour(wxString(wxT("AQUAMARINE")));
    }

    for(; i < MAX_NO_SERVERS; i++)
    {
        pyuvSlaveColors[i] = wxColour((uint8_t)(i * 18 + 31), (uint8_t)(i * 19 + 100), (uint8_t)(i * 13 + 200));
    }

    // Show it
    frame->Show(true);
    SetTopWindow(frame);

    // Parse command line
    wxCmdLineParser cmdParser(pyuvCommandLine, argc, argv);

    {
        // suppress disturbing warnings
        wxLogNull log;

        // Pass false to suppress auto Usage() message
#if 0
        res = cmdParser.Parse(false);
#else
        cmdParser.Parse(false);
#endif
    }

    // Check for a sequence filename
    if(cmdParser.GetParamCount() > 0)
    {
        frame->GetFileName() = cmdParser.GetParam(0);

        // Under Windows when invoking via a document
        // in Explorer, we are passed the short form.
        // So normalize and make the long form.
        wxFileName fName(frame->GetFileName());

        fName.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
        frame->GetFileName() = fName.GetFullPath();

        // open the passed file
        frame->openfile(wxFileName(frame->GetFileName(), wxPATH_NATIVE));
    }

    // Add handler
#if 0
    // the wxBMPHandler is always installed, so no need to do it again
    wxImage::AddHandler(new wxBMPHandler);
#endif

    // Start the event loop
    return true;
}

// Exit the application
int pyuvApp::OnExit()
{
    delete m_server;

    return 0;
}

/////////////////////////////////////////////////////
//// Drawing canvas methods
/////////////////////////////////////////////////////

// Event table for pyuvCanvas
wxBEGIN_EVENT_TABLE(pyuvCanvas, wxWindow)
    EVT_MOTION(pyuvCanvas::OnMotion)
    EVT_PAINT(pyuvCanvas::OnPaint)
    EVT_DROP_FILES(pyuvCanvas::OnDrop)
wxEND_EVENT_TABLE()

// Canvas constructor
pyuvCanvas::pyuvCanvas(pyuvFrame *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(PYUV_MINWIDTH, PYUV_MINHEIGHT), wxFULL_REPAINT_ON_RESIZE)
{
    // Create the logo img
    logoimg.~wxImage();
    logoimg = wxImage(playuv250x200);

    // Create the drawing bitmap
    pyuvBmp = new wxBitmap(logoimg);
    pyuvBmpdata = new PixelData(*pyuvBmp);
    pyuvPixel = new PixelData::Iterator(*pyuvBmpdata);
    pyuvRowstart = new PixelData::Iterator(*pyuvBmpdata);
}

// Motion handler
void pyuvCanvas::OnMotion(wxMouseEvent& event)
{
    if(event.Dragging())
    {
        wxClientDC dc(this);

        wxPen pen(*wxRED, 1, wxSOLID); // red pen of width 1
        dc.SetPen(pen);
        dc.DrawPoint(event.GetPosition());
    }
}

// Paint Handler
void pyuvCanvas::OnPaint(wxPaintEvent& event)
{
#if 0
    // Create a bitmap from our pixel data
    wxBitmap bmp(wxImage(PYUV_WIDTH, PYUV_HEIGHT, (unsigned char *) screen, true));
    wxBitmap bmp(logoimg);
    wxBitmap bmp(*pyuvImg);

    // See this: http://wxforum.shadonet.com/viewtopic.php?t=10654
    // THIS IS WORKING FINE
    wxPaintDC dc(this);
    dc.DrawBitmap(wxBitmap(*pyuvImg), 0, 0, true);

    // look here : http://wxforum.shadonet.com/viewtopic.php?t=9393
    wxBitmap b = wxBitmap(30, 20, -1);
    unsigned int *data = b.GetRawData();
    dc.DrawBitmap(b, 0, 0, true);

    // Working with raw bitmaps
    // Reset iterator
    pyuvPixel->Reset(*pyuvBmpdata);

    // Copy data
    int x, y;
    for (y = 0; y < pyuvHeightS; ++y) {

        *pyuvRowstart = *pyuvPixel;

        for (x = 0; x < pyuvWidthS; ++x, ++(*pyuvPixel)) {

            pyuvPixel->Red()   = pyuvScreen[3 * (y * pyuvWidthS + x)];
            pyuvPixel->Green() = pyuvScreen[3 * (y * pyuvWidthS + x) + 1];
            pyuvPixel->Blue()  = pyuvScreen[3 * (y * pyuvWidthS + x) + 2];
        }

        *pyuvPixel = *pyuvRowstart;
        pyuvPixel->OffsetY(*pyuvBmpdata, 1);
    }
#endif

    if(pyuvFrame::GetDoubleBuffer())
    {
#if 0
        wxBufferedPaintDC dc;

        // Paint the screen with double buffering
        if (pyuvEngine == 1)
            dc((wxWindow *) this, pyuvBmp, wxBUFFER_CLIENT_AREA);
        else
            dc((wxWindow *) this, wxBitmap(*pyuvImg), wxBUFFER_CLIENT_AREA);

        // Show the timecode
        if (pyuvShowtimecode) {

            // Set pen and brush
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush(*wxWHITE_BRUSH);

            // Set font
            dc.SetFont(pyuvTimeFont);
            dc.SetTextForeground(pyuvTimeColour);

            // Background
            dc.DrawRectangle(pyuvTimeBack);

            // Watermark the screen
            dc.DrawText(pyuvCurtime, tcx, tcy);
        };

        // Show the identity
        if (pyuvShowidentity) {

            // Set pen and brush
            dc.SetPen(*wxTRANSPARENT_PEN);

            // Set font
            dc.SetFont(pyuvIdFont);
            dc.SetTextForeground(pyuvIdColour);

            // Background
            if (pyuvServno >= 0)
                dc.SetBrush(pyuvSlaveColors[pyuvServno]);
            else
                dc.SetBrush(*wxWHITE_BRUSH);
            dc.DrawRectangle(pyuvIdBack);

            // Watermark the screen
            dc.DrawText(pyuvApp::GetServerNoString(), idx, idy);
        };
#endif
    }
    else
    {
        // Paint the screen with single buffering
        wxPaintDC dc(this);
        int pyuvEngine = pyuvFrame::GetEngine();

        if(pyuvEngine == 1)
        {
            // raw bitmap
            dc.DrawBitmap(*pyuvBmp, 0, 0, true);
        }
        else
        {
            if(pyuvEngine == 0)
            {
                // normal bitmap
                dc.DrawBitmap(wxBitmap(*pyuvFrame::GetImage()), 0, 0, true);
            }
        }

        // Show the timecode
        if(pyuvFrame::GetShowTimeCodeFlag())
        {
            wxCoord tcx, tcy;
            wxCoord tcwidth, tcheight;

            // Set pen and brush
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush(*wxWHITE_BRUSH);

            // Set font
            dc.SetFont(pyuvFrame::GetTimeFont());
            dc.SetTextForeground(pyuvApp::GetTimeColour());

            // Background
            pyuvFrame::GetTimeCodePosition(tcx, tcy);
            pyuvFrame::GetTimeCodeDimension(tcwidth, tcheight);
            pyuvApp::GetTimeBack() = wxRect(tcx - 2, tcy + 1, tcwidth + 4, tcheight - 2);
            dc.DrawRectangle(pyuvApp::GetTimeBack());

            // Watermark the screen
            dc.DrawText(pyuvFrame::GetCurrentTime(), tcx, tcy);
        }

        // Show the identity
        if(pyuvFrame::GetShowIdentityFlag())
        {
            wxCoord idx, idy;
            wxCoord idwidth, idheight;

            // Set pen and brush
            dc.SetPen(*wxTRANSPARENT_PEN);

            // Set font
            dc.SetFont(pyuvFrame::GetIdentityFont());
            dc.SetTextForeground(pyuvApp::GetIdColour());

            // Background
            if(pyuvApp::GetServerNo() >= 0)
            {
                dc.SetBrush((*pyuvApp::GetClientColour())[pyuvApp::GetServerNo()]);
            }
            else
            {
                dc.SetBrush(*wxWHITE_BRUSH);
            }
            pyuvFrame::GetIdentityPosition(idx, idy);
            pyuvFrame::GetIdentityDimension(idwidth, idheight);
            pyuvApp::GetIdBack() = wxRect(idx - 2, idy + 0, idwidth + 2, idheight - 2);
            dc.DrawRectangle(pyuvApp::GetIdBack());

            // Watermark the screen
            dc.DrawText(pyuvApp::GetServerNoString(), idx, idy);
        }
    }
}

// Drag and drop handler
void pyuvCanvas::OnDrop(wxDropFilesEvent& event)
{

    pyuvFrame *parent = (pyuvFrame *) GetParent();
    parent->openfile(wxFileName(event.m_files[0]));
}

// Accepts a connection from another instance
wxConnectionBase *pyuvServer::OnAcceptConnection(const wxString& topic)
{
    //wxMessageBox(wxString::Format(wxT("%02d - incoming connection - "), pyuvServno) + topic, wxT("Busy"), wxICON_INFORMATION|wxOK);

    if(topic.Lower() == wxT("intro") || topic.Lower() == wxT("dothis"))
    {
        // This is used to signal the existance in life
        if(pyuvApp::GetServerNo() == MAX_NO_SERVERS)
        {
            return NULL;
        }
        else
        {
            return new pyuvConnection(frame);
        }
    }
    else if(topic.Lower() == wxT("avail") || topic.Lower() == wxT("book"))
    {
        // This is used to verify that I am available to be commanded

        // Check that there are no modal dialogs active
        wxWindowList::Node* node = wxTopLevelWindows.GetFirst();
        while(node)
        {
            wxDialog* dialog = wxDynamicCast(node->GetData(), wxDialog);
            if(dialog && dialog->IsModal())
            {
                return NULL;
            }
            node = node->GetNext();
        }

        return new pyuvConnection(frame);

    }
    else if(topic.Lower() == wxT("ident"))
    {
        frame->switchidentity();

        return new pyuvConnection(frame);

    }
    else
    {
        // Dunno what it wants
        return NULL;
    }
}

// Respond to a connection
bool pyuvConnection::OnExecute(const wxString& topic, char *data, int WXUNUSED(size), wxIPCFormat WXUNUSED(format))
{
    // First, raise the main window
    /*if (frame)
        frame->Raise();*/
    //const char *data = wxString(wdata).ToAscii();

    //wxMessageBox(wxString::Format(wxT("%02d - incoming connection (\"%s\" - \"%s\")"), pyuvServno, topic.c_str(), data), wxT("Busy"), wxICON_INFORMATION|wxOK);

    if(topic.Lower() == wxT("book"))
    {
        int servno = -1;

        if(sscanf(data, "master%02d", &servno) == 1)
        {
            // sorry, already booked
            if((pyuvApp::GetMyMaster() >= 0) && (pyuvApp::GetMyMaster() != servno))
            {
                return false;
            }

            // booking
            if((servno >= 0) && (servno < MAX_NO_SERVERS))
            {
                pyuvApp::GetMyMaster() = servno;

                frame->SetOwnBackgroundColour((*pyuvApp::GetClientColour())[pyuvApp::GetMyMaster()]);
                frame->ClearBackground();

                //wxMessageBox(wxString::Format("We (%02d) have been booked by %02d", pyuvServno, pyuvMyMaster),
                //    wxT("Busy"), wxICON_INFORMATION|wxOK);

                return true;

            }
            else
            {
                return false;
            }
        }

        servno = -1;
        if(sscanf(data, "unmaster%02d", &servno) == 1)
        {
            // sorry, already booked
            if((pyuvApp::GetMyMaster() >= 0) && (pyuvApp::GetMyMaster() != servno))
            {
                return false;
            }

            // unbooking
            if((servno >= 0) && (servno < MAX_NO_SERVERS))
            {
                pyuvApp::GetMyMaster() = -1;

                frame->SetOwnBackgroundColour(frame->GetInternalBackgroundColor());
                frame->ClearBackground();

                /*wxMessageBox(wxString::Format("We (%02d) have been un-booked by %02d", pyuvServno, servno),
                    wxT("Busy"), wxICON_INFORMATION|wxOK);*/

                return true;

            }
            else
            {
                return false;
            }
        }
    }
    else if(topic.Lower() == wxT("avail"))
    {
        int servno = -1;
        sscanf(data, "master%02d", &servno);

        // availability
        if(pyuvApp::GetMyMaster() >= 0)
        {
            if(pyuvApp::GetMyMaster() == servno)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }

    }
    else if(topic.Lower() == wxT("dothis"))
    {
        // I cannot be commanded
        if(pyuvApp::GetMyMaster() == -1)
        {
            return false;
        }

        wxCommandEvent e;
        int frameno;

        // Execute the play/pause/stop commands
        if(!strcmp(data, "play"))
        {
            // play
            frame->OnPlay(e);

        }
        else if(!strcmp(data, "stop"))
        {
            // stop
            frame->OnStop(e);

        }
        else if(!strcmp(data, "forward"))
        {
            // forward
            frame->OnForward(e);

        }
        else if(sscanf(data, "moveto%d", &frameno) == 1)
        {
            // moveto
            if((frameno < 0) || (frameno >= (int)pyuvFrame::GetNumFrame()))
            {
                return true;
            }

            pyuvFrame::GetFrameNumber() = frameno;
            frame->moveto(frameno);

            // Move the slider from time to time
            if(!(pyuvFrame::GetFrameNumber() % frame->GetSliderUpdate()))
            {
                frame->frameSlider->SetValue(pyuvFrame::GetFrameNumber());
            }

        }
        else
        {
            return false;
        }

        return true;

    }
    else
    {
        // no good!!!!
        return false;
    }

    return true;
}

wxIMPLEMENT_APP(pyuvApp);

