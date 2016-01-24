// Name:    playuvFrame.cpp
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

/*
Copyright (C) 2005 - 2014 Giuseppe Baruffa

This file is part of PYUV.

PYUV is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PYUV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PYUV.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define PACKAGE_VERSION PYUV_VERSION
#endif

// turn off 'warning C4355: 'this' : used in base member initializer list'
#if defined(_WIN32_)
    #pragma warning ( disable:4355 )
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "playuvFrame.cpp"
    #pragma interface "playuvFrame.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/clipbrd.h>
#include <wx/cmdline.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/fs_zip.h>
#include <wx/generic/numdlgg.h>
#include <wx/html/helpctrl.h>
#include <wx/ipc.h>
#include <wx/propdlg.h>
#include <wx/rawbmp.h>
#include <wx/regex.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/intl.h>

#include <stdint.h>

// Inclusion of the main header file
#include "playuvFrame.h"
#include "playuvApp.h"
#include "playuvFormat.h"
#include "playuvSync.h"
#include "converter.h"

// Icons include
#include "playuv16.xpm"
#include "play16.xpm"
#include "pause16.xpm"
#include "stop16.xpm"

////////////////////////////////////////////////////////////
//// Main frame (window) methods
////////////////////////////////////////////////////////////

// Event table for pyuvFrame
wxBEGIN_EVENT_TABLE(pyuvFrame, wxFrame)
    // File menu
    EVT_MENU(wxID_OPEN, pyuvFrame::OnOpen)                      // Open menu
    EVT_MENU(wxID_CLOSE, pyuvFrame::OnClose)                    // Close menu
    EVT_MENU(wxID_SAVEAS, pyuvFrame::OnSaveAs)                  // Save As menu
    EVT_MENU(Menu_File_Format, pyuvFrame::OnFormat)             // Format menu
    EVT_MENU(wxID_EXIT, pyuvFrame::OnQuit)                      // Quit menu

    // Control menu
    EVT_MENU(Menu_Control_Play, pyuvFrame::OnPlay)              // Play menu
    EVT_MENU(Menu_Control_Stop, pyuvFrame::OnStop)              // Stop menu
    EVT_MENU(Menu_Control_Rewind, pyuvFrame::OnRewind)          // Rewind menu
    EVT_MENU(Menu_Control_Forward, pyuvFrame::OnForward)        // Forward menu
    EVT_MENU(Menu_Control_Goto, pyuvFrame::OnGoto)              // Goto menu
    EVT_MENU(Menu_Control_Loop, pyuvFrame::OnLoop)              // Loop menu
    EVT_MENU(Menu_Control_Sync, pyuvFrame::OnSync)              // Sync menu

    // View menu
    EVT_MENU(Menu_View_Timecode, pyuvFrame::OnTimecode)         // Time code menu
    EVT_MENU(Menu_View_Deinterlace, pyuvFrame::OnDeinterlace)   // Deinterlace menu
    EVT_MENU(Menu_View_Identity, pyuvFrame::OnIdentity)         // Identity menu
    EVT_MENU(Menu_View_Zoom_400, pyuvFrame::OnScale)            // Change of scale
    EVT_MENU(Menu_View_Zoom_200, pyuvFrame::OnScale)            // Change of scale
    EVT_MENU(Menu_View_Zoom_100, pyuvFrame::OnScale)            // Change of scale
    EVT_MENU(Menu_View_Zoom_50, pyuvFrame::OnScale)             // Change of scale
    EVT_MENU(Menu_View_Zoom_33, pyuvFrame::OnScale)             // Change of scale
    EVT_MENU(Menu_View_Zoom_25, pyuvFrame::OnScale)             // Change of scale

    EVT_MENU(Menu_View_Comp_All, pyuvFrame::OnComp)             // Change of component
    EVT_MENU(Menu_View_Comp_Next, pyuvFrame::OnComp)            // Change of component

    EVT_MENU(wxID_COPY, pyuvFrame::OnCopy)                      // Copy to clipboard

    // Settings menu
    EVT_MENU(Menu_Settings_Draw_Bitmap, pyuvFrame::OnEngine)    // Switch engine
    EVT_MENU(Menu_Settings_Draw_Raw, pyuvFrame::OnEngine)       // Switch engine
    EVT_MENU(Menu_Settings_Draw_OGL, pyuvFrame::OnEngine)       // Switch engine
    EVT_MENU(Menu_Settings_Draw_Double, pyuvFrame::OnDouble)    // Switch to double buffering

    // Help menu
    EVT_MENU(wxID_ABOUT, pyuvFrame::OnAbout)                    // About menu
    EVT_MENU(Menu_Help_Contents, pyuvFrame::OnContents)         // Contents menu

    // Link timer ticks to its handler
    EVT_TIMER(ANIMATE_TIMER_ID, pyuvFrame::OnAnimate)

    //EVT_SIZE(pyuvFrame::OnResize)

    // Slider events
    EVT_COMMAND_SCROLL(SLIDER_ID, pyuvFrame::OnScroll)

    // Button events
    EVT_BUTTON(PlayButton_id, pyuvFrame::OnPlay)
    EVT_BUTTON(StopButton_id, pyuvFrame::OnStop)
wxEND_EVENT_TABLE()

uint8_t *pyuvFrame::pyuvScreen = NULL;
uint8_t *pyuvFrame::pyuvScreenS = NULL;
uint8_t *pyuvFrame::pyuvBuffer = NULL;

int pyuvFrame::pyuvScale = 1;
unsigned long int pyuvFrame::pyuvWidth = PYUV_WIDTH;
unsigned long int pyuvFrame::pyuvHeight = PYUV_HEIGHT;
uint32_t pyuvFrame::pyuvArea;
int pyuvFrame::pyuvFrameTime;
double pyuvFrame::pyuvSleepTime;
bool pyuvFrame::pyuvAnamorphic = false;
double pyuvFrame::pyuvGamma = 1.0;
int pyuvFrame::pyuvSamplebits = PYUV_DEPTH;
bool pyuvFrame::pyuvSigned = PYUV_SIGNED;
bool pyuvFrame::pyuvInterleaved = false;
bool pyuvFrame::pyuvEndianness = false;
int pyuvFrame::pyuvShowcomps = 0;
double pyuvFrame::pyuvRate = PYUV_RATE;
uint32_t pyuvFrame::pyuvFramenumber = 0;
uint32_t pyuvFrame::pyuvNumframes = 0;
bool pyuvFrame::pyuvInterlaced = false;
bool pyuvFrame::pyuvDeinterlace = true;

int pyuvFrame::pyuvFormat = PYUV_FORMAT;
wxString pyuvFrame::pyuvFilename = wxT("");
wxFile *pyuvFrame::pyuvFile = NULL;
unsigned long int pyuvFrame::pyuvHeadersize = 0;

wxColour pyuvFrame::pyuvBackColor = wxColour(134U, 0U, 0U);

bool pyuvFrame::pyuvDoublebuffer = false;
int pyuvFrame::pyuvEngine = 0;

wxString pyuvFrame::pyuvCurtime = wxT("0000: 00'00\"00 00.00");
wxCoord pyuvFrame::tcwidth = 0;
wxCoord pyuvFrame::tcheight = 0;
wxCoord pyuvFrame::tcx = 0;
wxCoord pyuvFrame::tcy = 0;
wxCoord pyuvFrame::idwidth = 0;
wxCoord pyuvFrame::idheight = 0;
wxCoord pyuvFrame::idx = 0;
wxCoord pyuvFrame::idy = 0;
wxFont pyuvFrame::pyuvTimeFont;
wxFont pyuvFrame::pyuvIdFont;

bool pyuvFrame::pyuvPlay = false;
bool pyuvFrame::pyuvShowtimecode = false;
bool pyuvFrame::pyuvShowidentity = false;
bool pyuvFrame::pyuvLoop = true;

wxString pyuvFrame::pyuvTopbasetitle = wxT(PYUV_SHORTNAME);
bool pyuvFrame::pyuvHelpsystem = false;

const wxString pyuvFrame::pyuvComponents[] =
{
    _("Choose one"),
    wxT("RGB"), wxT("XYZ"), wxT("HSV"), wxT("YUV"), wxT("YCbCr"),
    wxT("YCbCr709"), wxT("YIQ"), wxT("YCoCg"), wxT("YCxCz"), wxT("BW"),
    _("Custom")
};

const wxString pyuvFrame::pyuvFormatShort[] =
{
    wxT("P444"), wxT("P422"), wxT("P420"), wxT("P411"), wxT("P410"),
    wxT("P400"), wxT("I444"), wxT("I422"), wxT("I420"), wxT("I411"),
    wxT("I410"), wxT("UYVY"), wxT("YUY2"), wxT("YVYU"), wxT("IYUV"),
    wxT("UNK")
};

const int pyuvFrame::pyuvFormatSampling[] =
{
    SS_444, SS_422, SS_420, SS_411, SS_410,
    SS_400, SS_444, SS_422, SS_420, SS_411,
    SS_410, SS_422, SS_422, SS_422, SS_420,
    SS_UNDEF
};

const bool pyuvFrame::pyuvFormatInterleave[] =
{
    false, false, false, false, false,
    false, true, true, true, true,
    true, true, true, true, false,
    false
};

const int pyuvFrame::pyuvFormatOrder[] =
{
    OR_DEFAULT, OR_DEFAULT, OR_DEFAULT, OR_DEFAULT, OR_DEFAULT,
    OR_DEFAULT, OR_DEFAULT, OR_DEFAULT, OR_DEFAULT, OR_DEFAULT,
    OR_DEFAULT, OR_UYVY, OR_YUY2, OR_YVYU, OR_DEFAULT,
    OR_DEFAULT
};

const float pyuvFrame::pyuvFramedensity[] =
{
    0.0, 3.0, 2.0, 1.5, 1.5,
    1.25, 1.0, 0.0
};

uint32_t pyuvFrame::pyuvSliderupdate = 1;
wxImage *pyuvFrame::pyuvImg = NULL;
uint32_t pyuvFrame::pyuvWidthS;
uint32_t pyuvFrame::pyuvHeightS;
int pyuvFrame::pyuvColorspacenumber;
int pyuvFrame::pyuvSubsamplingnumber;
int pyuvFrame::pyuvOrder;
wxFileOffset pyuvFrame::pyuvFilesize;
uint32_t pyuvFrame::pyuvFramesize;
uint32_t pyuvFrame::pyuvFramebytes;

// Frame ctor
pyuvFrame::pyuvFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1), wxSize(PYUV_WIDTH, PYUV_HEIGHT), wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCAPTION|wxCLOSE_BOX|wxCLIP_CHILDREN),
	animTimer(this, ANIMATE_TIMER_ID), help(wxHF_DEFAULT_STYLE|wxHF_OPEN_FILES)
{
    pyuvTimeFont = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    pyuvIdFont = wxFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    pyuvArea = pyuvWidth * pyuvHeight;

    pyuvFrameTime = (int)floor((1000.0 / PYUV_RATE) + 0.5);
	pyuvSleepTime = pyuvFrameTime;

    // Set the frame icon
    SetIcon(wxIcon(playuv16));

    // Minimum size, by default
    SetMinSize(wxSize(PYUV_MINWIDTH, PYUV_MINHEIGHT));

    // Create a file menu bar
    wxMenu *fileMenu = new wxMenu;

    // Append file menus
    fileMenu->Append(wxID_OPEN,  _("O&pen\tCtrl+O"), _("Open a file"));
    fileMenu->Append(wxID_CLOSE, _("C&lose\tCtrl+C"), _("Close the file"));
    fileMenu->Append(wxID_SAVEAS, _("S&ave frame\tCtrl+S"), _("Save the frame"));
    fileMenu->AppendSeparator();
    fileMenu->Append(Menu_File_Format, _("&Format\tCtrl+F"), _("Select the video format"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT,  _("E&xit\tCtrl+Q"), _("Quit this program"));

    // Create a control menu bar
    wxMenu *controlMenu = new wxMenu;

    // Append control menus
    pyuvPlay = false;
    controlMenu->Append(Menu_Control_Play, _("&Play\tSpace"), _("Play/pause a sequence"));
    controlMenu->Enable(Menu_Control_Play, false);
    controlMenu->Append(Menu_Control_Stop, _("&Stop\tEsc"), _("Stop a sequence"));
    controlMenu->Enable(Menu_Control_Stop, false);
    controlMenu->Append(Menu_Control_Rewind, _("&Backward\tLeft"), _("Backward a frame"));
    controlMenu->Enable(Menu_Control_Rewind, false);
    controlMenu->Append(Menu_Control_Forward, _("&Forward\tRight"), _("Forward a frame"));
    controlMenu->Enable(Menu_Control_Forward, false);
    controlMenu->Append(Menu_Control_Goto, _("&Goto\tCtrl+G"), _("Go to frame")); 
    controlMenu->Enable(Menu_Control_Goto, false);
    controlMenu->AppendSeparator();
    controlMenu->AppendCheckItem(Menu_Control_Loop, _("&Loop\tCtrl+Y"), _("Loop continuously"));
    pyuvLoop = true;
    controlMenu->Check(Menu_Control_Loop, true);
    controlMenu->Enable(Menu_Control_Loop, false);

    // Add a sync menu
    controlMenu->Append(Menu_Control_Sync, _("&Sync\tCtrl+K"), _("Sync commands between PYUVs"));

    // Create a view menu bar
    wxMenu *viewMenu = new wxMenu;

    // Create a zoom submenu
    wxMenu *zoomMenu = new wxMenu;

    // Append zoom menus
    zoomMenu->AppendRadioItem(Menu_View_Zoom_400, _("4&00%\tCtrl+Q"), _("Four x size"));
    zoomMenu->Enable(Menu_View_Zoom_400, true);
    zoomMenu->AppendRadioItem(Menu_View_Zoom_200, _("2&00%\tCtrl+D"), _("Double size"));
    zoomMenu->Enable(Menu_View_Zoom_200, true);
    zoomMenu->AppendRadioItem(Menu_View_Zoom_100, _("&100%\tCtrl+1"), _("Original size"));
    zoomMenu->Check(Menu_View_Zoom_100, true);
    zoomMenu->AppendRadioItem(Menu_View_Zoom_50, _("&50%\tCtrl+2"), _("Half size"));
    zoomMenu->Enable(Menu_View_Zoom_50, true);
    zoomMenu->AppendRadioItem(Menu_View_Zoom_33, _("&33%\tCtrl+3"), _("Third size"));
    zoomMenu->Enable(Menu_View_Zoom_33, true);
    zoomMenu->AppendRadioItem(Menu_View_Zoom_25, _("&25%\tCtrl+4"), _("Quarter size"));
    zoomMenu->Enable(Menu_View_Zoom_25, true);

    // Append them to View
    viewMenu->Append(Menu_View_Zoom, _("Zoom"), zoomMenu);

    // Create a component submenu
    wxMenu *compMenu = new wxMenu;

    // Append component menus
    compMenu->Append(Menu_View_Comp_All, _("&All\tA"), _("Show all components"));
    compMenu->Append(Menu_View_Comp_Next, _("&Next\tN"), _("Show next component"));

    // Append them to View
    viewMenu->Append(Menu_View_Comp, _("Components"), compMenu);

    // Add other menus to View
    viewMenu->AppendCheckItem(Menu_View_Deinterlace, _("&Deinterlace\tD"), _("Deinterlace"));
    pyuvDeinterlace = false;
    viewMenu->Check(Menu_View_Deinterlace, false);

    viewMenu->AppendCheckItem(Menu_View_Timecode, _("&Time code\tT"), _("Show the timecode"));
    pyuvShowtimecode = false;
    viewMenu->Check(Menu_View_Timecode, false);

    viewMenu->AppendCheckItem(Menu_View_Identity, _("&Identity\tI"), _("Show identity"));
    pyuvShowidentity = false;
    viewMenu->Check(Menu_View_Identity, false);

    viewMenu->AppendSeparator();
    viewMenu->Append(wxID_COPY, _("Cop&y frame\tF8"), _("Copy the frame to clipboard"));

    // Create a settings menu bar
    wxMenu *settingsMenu = new wxMenu;

    // Append drawing engine selection
    wxMenu *drawMenu = new wxMenu;

    // Drawing engines
    drawMenu->AppendRadioItem(Menu_Settings_Draw_Bitmap, _("&Bitmap\tCtrl+B"), _("Draw with bitmaps"));
    if (pyuvEngine == 0)
        drawMenu->Check(Menu_Settings_Draw_Bitmap, true);
		
    /*drawMenu->AppendRadioItem(Menu_Settings_Draw_Raw, _("&Raw bitmap\tCtrl+R"), _("Draw with raw bitmaps"));
    if (pyuvEngine == 1)
        drawMenu->Check(Menu_Settings_Draw_Raw, true);
    drawMenu->Enable(Menu_Settings_Draw_Raw, false);*/
		
    /*drawMenu->AppendRadioItem(Menu_Settings_Draw_OGL, _("&OpenGL\tCtrl+L"), _("Draw with OpenGL"));
    if (pyuvEngine == 2)
        drawMenu->Check(Menu_Settings_Draw_OGL, true);
    drawMenu->Enable(Menu_Settings_Draw_OGL, false);*/
		
    drawMenu->AppendSeparator();
	
    drawMenu->AppendCheckItem(Menu_Settings_Draw_Double, _("&Double buffer\tCtrl+F"), _("Use double buffering"));
    if (pyuvDoublebuffer)
        drawMenu->Check(Menu_Settings_Draw_Double, true);
    else
        drawMenu->Check(Menu_Settings_Draw_Double, false);
    drawMenu->Enable(Menu_Settings_Draw_Double, false);


    // Append them to Settings
    settingsMenu->Append(Menu_Settings_Draw, _("Draw engine"), drawMenu);

    // Create a help menu bar
    wxMenu *helpMenu = new wxMenu;

    // Activate the "Contents" help system
    help.UseConfig(wxConfig::Get());
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    wxImage::AddHandler(new wxPNGHandler);
    if (help.AddBook(wxFileName(wxT(HELP_FILENAME), wxPATH_UNIX))
        || help.AddBook(wxFileName(wxT("/Applications/pyuv.app/Contents/MacOS/doc.htb"), wxPATH_UNIX))
        || help.AddBook(wxFileName(wxT("/usr/share/doc/pyuv/doc.htb"), wxPATH_UNIX))) {
        helpMenu->Append(Menu_Help_Contents, _("&Contents\tCtrl+H"), _("Show program help"));
        pyuvHelpsystem = true;
    } else {
        helpMenu->Append(Menu_Help_Contents, _("&Contents\tCtrl+H"), wxString(wxT(HELP_FILENAME)) + _(" not found!!!"));
        helpMenu->Enable(Menu_Help_Contents, false);
        pyuvHelpsystem = false;
    }
    /*wxStandardPaths sp = wxStandardPaths::Get();
    wxString cwd = wxFileName(sp.GetExecutablePath(), wxPATH_UNIX);
    printf("%s\n", (const char*)cwd.mb_str());*/

    // The "About" item should be in the help menu
    helpMenu->Append(wxID_ABOUT, _("&About...\tF1"), _("Show about dialog"));

    // Now append the freshly created menus to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _("&File"));
    menuBar->Append(controlMenu, _("&Control"));
    menuBar->Append(viewMenu, _("&View"));
    menuBar->Append(settingsMenu, _("&Settings"));
    menuBar->Append(helpMenu, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // Create a status bar
    CreateStatusBar(2);
    int stwid[] = {250, -1};
    SetStatusWidths(2, stwid);
    SetStatusText(_("Welcome to PYUV!"), 0);

    // This is the top frame sizer
    frameSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(frameSizer); 

    // Create the drawing area panel
    drawCanvas = new pyuvCanvas(this);
    drawCanvas->SetBackgroundColour(wxNullColour);

    // Drag and drop files
#ifdef __WXMSW__
    drawCanvas->DragAcceptFiles(true);
#endif

    // add drawCanvas to the frame sizer
    frameSizer->Add(drawCanvas, // the image panel
                    0, // vertically unstretchable
                    wxALL,  // border in every direction
                    2
                    );

    // Create the control panel
    wxPanel *controlPanel = new wxPanel(this, // child of this frame
                                        wxID_ANY, // no particular ID
                                        wxDefaultPosition, // position in frame
                                        wxDefaultSize, // position in size
                                        wxTAB_TRAVERSAL // let TAB control the focus
                                        );

    // This is the control panel sizer
    controlSizer = new wxBoxSizer(wxHORIZONTAL);
    controlPanel->SetAutoLayout(true);
    controlPanel->SetSizer(controlSizer);

    // add controlPanel to the frame sizer
    frameSizer->Add(controlPanel, // the control panel
                    0, // vertically unstretchable
                    wxGROW // grow in every direction
                    );

    // Create the slider
    frameSlider = new wxSlider(controlPanel, // child of
                             SLIDER_ID, // particular ID
                             0, // initial value
                             0, // minimum value
                             100, // maximum value
                             wxDefaultPosition, // position in panel
                             wxDefaultSize, // size
                             wxSL_HORIZONTAL | wxSL_AUTOTICKS // style
                             );

    // Add the slider to the control panel sizer
    controlSizer->Add(frameSlider, // managed control
                      1, // horizontally stretchable
                      wxEXPAND | wxALL, // grow everywhere
                      2 // border
                      );
    
    // Create the play/pause button
    //wxBitmap bitmap = wxArtProvider::GetIcon(wxART_GO_FORWARD, wxART_BUTTON, wxSize(16, 16));
    playButton = new wxBitmapButton(controlPanel, // child of
                                    PlayButton_id, // particular ID
                                    wxBitmap(play16), // the bitmap to show
                                    wxDefaultPosition, // position
                                    wxDefaultSize, // size
                                    wxBU_AUTODRAW // manage 3D automagically
                                    );
    playButton->Enable(false);

    // Add the button to the control panel sizer
    controlSizer->Add(playButton, // managed control
                      0, // horizontally unstretchable
                      wxALIGN_CENTER | wxTOP | wxBOTTOM, // center and border all around
                      4 // border
                      );
    
    // Create the stop button
    stopButton = new wxBitmapButton(controlPanel, // child of
                                    StopButton_id, // particular ID
                                    wxBitmap(stop16), // the bitmap to show
                                    wxDefaultPosition, // position
                                    wxDefaultSize, // size
                                    wxBU_AUTODRAW // manage 3D automagically
                                    );
    stopButton->Enable(false);

    // Add the button to the control panel sizer
    controlSizer->Add(stopButton, // managed control
                      0, // horizontally unstretchable
                      wxALIGN_CENTER | wxRIGHT | wxTOP | wxBOTTOM, // center and border all around
                      4 // border
                      );
    
    // Set the minimal size of the control panel
    controlSizer->SetSizeHints(controlPanel); 
    controlSizer->Fit(controlPanel);

    // Check if we are invoked by command line
    /*if (pyuvFilename != wxT(""))
        openfile(pyuvFilename);*/

    // Associate a background image
    pyuvImg = &(drawCanvas->logoimg);
    pyuvWidthS  = PYUV_WIDTH / pyuvScale;
    pyuvHeightS = PYUV_HEIGHT / pyuvScale;

    // Prepare some things
    pyuvPrepare();

    // don't allow frame to get smaller than what the sizers tell
    frameSizer->Layout();
    frameSizer->SetSizeHints(this);
    frameSizer->Fit(this);

    // Get color
    pyuvBackColor = GetBackgroundColour();

    // Enlarge the screen
    wxCommandEvent e;
    OnClose(e);

}

size_t pyuvFrame::GetSizeOfComponents(void) const
{
    return WXSIZEOF(pyuvComponents);
}

// File menu handlers
// Open menu handler
void pyuvFrame::OnOpen(wxCommandEvent& event)
{
    // Prepare file open dialog
    wxFileDialog dialog(
        this,  // parent
        _("Open"), // title of the dialog
        wxT(""), // starting directory
        wxT(""), // starting filename
#ifdef __WXMOTIF__
        _("PYUV files (*.*)|*.*") // only 1 choice for MOTIF
#else
        _("PYUV files (*.yuv;*.raw;*.rgb;*.xyz;*.hsv;*.yiq;*.bw;*.cif;*.sif;*.qcif;*.vix)|") +
            _("*.yuv;*.raw;*.rgb;*.xyz;*.hsv;*.yiq;*.bw;*.cif;*.sif;*.qcif;*.vix|All files (*.*)|*.*")
#endif
    );

    // Show the dialog
    if (dialog.ShowModal() == wxID_OK) {

        // Debug dialog: disposable
        /*wxString info;
        info.Printf(wxT("Full file name: %s\n")
            wxT("Path: %s\n")
            wxT("Name: %s\n"),
            dialog.GetPath().c_str(),
            dialog.GetDirectory().c_str(),
            filename.c_str());
        wxMessageDialog dialog2(this, info, _T("Selected file"));
        dialog2.ShowModal();*/

        openfile(wxFileName(dialog.GetPath()));
    };
}

// Save menu handler
void pyuvFrame::OnSaveAs(wxCommandEvent& event)
{
    // Prepare file save as dialog
    wxFileDialog dialog(
        this,  // parent
        _("Save frame as BMP"), // title of the dialog
        wxT(""), // starting directory
        wxString::Format(wxT("frame%06d.bmp"), pyuvFramenumber), // starting filename
        _("BMP files (*.bmp)|*.bmp|All files (*.*)|*.*"), 
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );

    // Show the dialog
    if (dialog.ShowModal() == wxID_OK) {

        // Debug dialog: disposable
        wxString info;
        info.Printf(_("Full file name: %s\n") +
            _("Path: %s\n") +
            _("Name: %s\n"),
            dialog.GetPath().c_str(),
            dialog.GetDirectory().c_str(),
            dialog.GetFilename().c_str());
        wxMessageDialog dialog2(this, info, _("Selected file"));
        //dialog2.ShowModal();

        wxImage myImg;
        wxBitmap myBmp(*pyuvImg);
        myImg = myBmp.ConvertToImage();

        // Save the image
        if (myImg.IsOk()) {
            if (!myImg.SaveFile(dialog.GetPath(), (wxBitmapType) wxBITMAP_TYPE_BMP)) {
                wxMessageDialog dialog2(this, _("The frame could not be saved"), _("File not saved"));
                dialog2.ShowModal();
            };
        } else {
            wxMessageDialog dialog2(this, _("The frame could not really be saved"), _("File not saved"));
            dialog2.ShowModal();
        }

    }

}


// Open the selected file name
void pyuvFrame::openfile(wxFileName videofile)
{
    // For automatic parameter setup, the following actions are performed
    //
    // 1: if the extension is .vix, then the header is parsed to setup the parameters
    //
    // 2: the filename can contain the following tokens, in any order
    //
    //  whatever_{width}x{height}_{rate}Hz_{format}_{depth}b_g{gamma}.{ext}
    //
    // {width}  = integer number, the frame width in pixel
    // {height} = integer number, the frame height in pixel
    // {rate}   = real number, the frames per second to be rendered
    // {format} = sampling format. See help for valid choices
    // {depth}  = integer number between -15 and 15, the component depth in bits
    //            (negative values denote a signed format for that depth)
    // {ext}    = 3-letter abbreviation of the colorspace (can be anywhere in the filename, also)
    // {gamma}  = gamma correction value multiplied by 10, without commas or points
    //
    // 3: a filename with the same base name, but with extension .hdr
    // is searched in the same directory. It can contain the following lines
    //
    // width = XXXX
    // height = XXXX
    // rate = XXXX
    // format = XXXX
    // depth = XXXX
    // space = XXXX
    // gamma = XXXX
    //
    // 4: a filename pyuvdefault.hdr is searched
    //

    // Parse the collected file name
    int hints = 0;
    wxRegEx myreg;
    wxString mystr, info;
    wxString filename = videofile.GetFullName();

    // Load the full file name, with the path
    pyuvFilename = videofile.GetFullPath();

    // First, try to open the file in binary mode
    // Declare the file
    pyuvFile = new wxFile(pyuvFilename, wxFile::read);
    if (!pyuvFile->IsOpened()) {
        // Display a message box with the info
        wxMessageBox(_("File could not be opened: ") + pyuvFilename, _("PYUV Error"), wxOK, this);
        return;
    };

    // Look for the anamorphic string
    if (filename.Contains(wxT("anamorphic"))) {
        pyuvAnamorphic = true;
    };

    // Look for the format string
    for (pyuvFormat = 0; pyuvFormat < (int) (WXSIZEOF(pyuvFormatShort) - 1); pyuvFormat++)
        if (filename.Contains(pyuvFormatShort[pyuvFormat])) {
            hints += 3;
            break;
        };

    // Look for the colorspace string
    for (pyuvColorspacenumber = 0; pyuvColorspacenumber < (int) (WXSIZEOF(pyuvComponents) - 1); pyuvColorspacenumber++)
        if (filename.Contains(pyuvComponents[pyuvColorspacenumber].Lower())) {
            hints += 1;
            break;
        };
    if (pyuvColorspacenumber == WXSIZEOF(pyuvComponents))
        pyuvColorspacenumber = CS_YCBCR;

    // Find interlacing
    if (filename.Contains(wxT("_int_"))) {
        pyuvInterlaced = true;
        hints += 0;
    };

    // Find a frame size
    mystr = wxT("([0-9]*)x([0-9]*)");
    if (!myreg.Compile(mystr)) {
        wxMessageDialog dialog3(this, _("BAD REGEX!!!"), wxT(""));
        dialog3.ShowModal();
        return;
    };
    if (myreg.Matches(filename)) {
        myreg.GetMatch(filename, 1).ToULong(&pyuvWidth, 10);
        myreg.GetMatch(filename, 2).ToULong(&pyuvHeight, 10);
        pyuvArea = pyuvWidth * pyuvHeight;
        pyuvWidthS = pyuvWidth / pyuvScale;
        pyuvHeightS = pyuvHeight / pyuvScale;
        hints += 2;
    };

    // Find a frame rate
    mystr = wxT("([.0-9]*)Hz");
    if (!myreg.Compile(mystr)) {
        wxMessageDialog dialog3(this, _("BAD REGEX!!!"), wxT(""));
        dialog3.ShowModal();
        return;
    };
    if (myreg.Matches(filename)) {
        myreg.GetMatch(filename, 1).ToDouble(&pyuvRate);
        hints++;
    };

    // Find a gamma value
    mystr = wxT("g([0-9]*)");
    if (!myreg.Compile(mystr)) {
        wxMessageDialog dialog3(this, _("BAD REGEX!!!"), wxT(""));
        dialog3.ShowModal();
        return;
    };
    if (myreg.Matches(filename)) {
        myreg.GetMatch(filename, 1).ToDouble(&pyuvGamma);
        if (pyuvGamma <= 0.0)
            pyuvGamma = 1.0;
    };

    // Find a bit depth
    mystr = wxT("_([-0-9]*)b");
    if (!myreg.Compile(mystr)) {
        wxMessageDialog dialog3(this, _("BAD REGEX!!!"), wxT(""));
        dialog3.ShowModal();
        return;
    };
    pyuvSamplebits = 8;
    pyuvSigned = false;
    if (myreg.Matches(filename)) {
        myreg.GetMatch(filename, 1).ToLong((long *) &pyuvSamplebits, 10);
        if (pyuvSamplebits < 0) {
            pyuvSamplebits = -pyuvSamplebits;
            pyuvSigned = true;
        } else
            pyuvSigned = false;
        //hints++;
    };

    // It's a VIX file
    if (videofile.GetExt() == wxT("vix")) {

        /*char buffer[1000];*/
        float framerate;
        int numframes, numbits, width, height, numcomps, /*c,*/ sw[3], sh[3];
        char colorspace[10], signedness[10], ssize[10], endianness[20];
        FILE *fp;

        // load first chunk
        //pyuvFile->Read(buffer, 1000);
        //pyuvFile->Seek(0);
        fp = fopen(pyuvFilename.ToAscii(), "rb");

        // Scan for parameters
        if (fscanf(fp, "vix\n>VIDEO<\n %f %d\n>COLOUR<\n%s\n>IMAGE<\n %s %s %d %s\n%d %d %d\n%d %d\n%d %d\n%d %d\n",
            &framerate, &numframes, colorspace, signedness, ssize, &numbits, endianness,
            &width, &height, &numcomps, &sw[0], &sh[0], &sw[1], &sh[1], &sw[2], &sh[2]) == 16) {

            hints = 10;

            pyuvWidth = width;
            pyuvHeight = height;
            pyuvArea = pyuvWidth * pyuvHeight;
            pyuvWidthS = pyuvWidth / pyuvScale;
            pyuvHeightS = pyuvHeight / pyuvScale;

            pyuvRate = framerate;

            pyuvSamplebits = numbits;

            if (!strcmp(signedness, "unsigned"))
                pyuvSigned = false;
            else
                pyuvSigned = true;

            if (!strcmp(colorspace, "YCbCr"))
                pyuvColorspacenumber = CS_YCBCR;

            if (!strcmp(colorspace, "RGB"))
                pyuvColorspacenumber = CS_RGB;

            if ((sw[0] == 1) && (sh[0] == 1) && (sw[1] == 1) && (sh[1] == 1) && (sw[2] == 1) && (sh[2] == 1))
                pyuvSubsamplingnumber = SS_444;

            if ((sw[0] == 1) && (sh[0] == 1) && (sw[1] == 2) && (sh[1] == 2) && (sw[2] == 2) && (sh[2] == 2))
                pyuvSubsamplingnumber = SS_420;

            pyuvFormat = -1;

        }

        pyuvHeadersize = ftell(fp);
        fclose(fp);

    }

#if 0
    // Not enough info collected: look for header file
    if (hints < 4) {

        hints = 0;
#endif

    // Not enough info collected: look for header file
    if (1) {

        int oldhints = hints;
        hints = 0;

        wxFileName headername(videofile);
        headername.SetExt(wxT("hdr"));
        wxString linea, value;

        if (!(headername.FileExists())) {

            headername.SetFullName(wxT("pyuvdefault.hdr"));
        }

        // Load the header file
        wxTextFile headerfile;
        if (headername.FileExists()) {

            // Open the file
            headerfile.Open(headername.GetFullPath());

            // Traverse all lines
            int lineno;
            for (lineno = 0; lineno < (int) headerfile.GetLineCount(); lineno++)
            {
                linea = headerfile.GetLine(lineno);
                if (linea.StartsWith(wxT("width = "), &value)) {
                    value.ToULong(&pyuvWidth);
                    pyuvWidthS = pyuvWidth / pyuvScale;
                    hints++;
                };
                if (linea.StartsWith(wxT("height = "), &value)) {
                    value.ToULong(&pyuvHeight);
                    pyuvHeightS = pyuvHeight / pyuvScale;
                    hints++;
                };
                if (linea.StartsWith(wxT("rate = "), &value)) {
                    value.ToDouble(&pyuvRate);
                    hints++;
                };
                if (linea.StartsWith(wxT("format = "), &value)) {
                    for (pyuvFormat = 0; pyuvFormat < (int) (WXSIZEOF(pyuvFormatShort) - 1); pyuvFormat++)
                        if (value == pyuvFormatShort[pyuvFormat]) {
                            hints += 3;
                            break;
                        };
                };
                if (linea.StartsWith(wxT("space = "), &value)) {
                    for (pyuvColorspacenumber = 0; pyuvColorspacenumber < (int) (WXSIZEOF(pyuvComponents) - 1); pyuvColorspacenumber++)
                        if (value.Lower() == pyuvComponents[pyuvColorspacenumber].Lower()) {
                            hints += 1;
                            break;
                        };
                    if (pyuvColorspacenumber == WXSIZEOF(pyuvComponents))
                        pyuvColorspacenumber = CS_YCBCR;
                };
                if (linea.StartsWith(wxT("depth = "), &value)) {
                    value.ToLong((long *) &pyuvSamplebits);
                    if (pyuvSamplebits < 0) {
                        pyuvSamplebits = -pyuvSamplebits;
                        pyuvSigned = true;
                    } else
                        pyuvSigned = false;
                    //hints++;
                };

            };

            headerfile.Close();

        } else

            hints = oldhints;

    };

    // Components and subsampling
    if (pyuvFormat >= 0) {
        /*pyuvColorspacenumber = pyuvFormatColor[pyuvFormat];*/
        pyuvSubsamplingnumber = pyuvFormatSampling[pyuvFormat];
        pyuvInterleaved = pyuvFormatInterleave[pyuvFormat];
        pyuvOrder = pyuvFormatOrder[pyuvFormat];
        //hints += 3;
    }

    // There are too few matches, ask the user
    if (hints < 4) {
        pyuvFormatDialog dialog(this);
        dialog.ShowModal();
    };

    // Give the results of the match
    SetTitle(pyuvTopbasetitle + wxT(" - ") + filename);

    // File and frame size
    if (pyuvFile && pyuvFile->IsOpened()) {

        pyuvFilesize = pyuvFile->Length();
        pyuvFramesize = (unsigned long) ((double) (pyuvWidth * pyuvHeight) * pyuvFramedensity[pyuvSubsamplingnumber]);
        pyuvFramebytes = pyuvFramesize * ((pyuvSamplebits > 8) ? 2 : 1);
        pyuvNumframes = (unsigned long) floor((double) (pyuvFilesize - pyuvHeadersize) / (double) pyuvFramebytes);
        pyuvFramenumber = 0;

        // Check if frame dimensions fit inside the display dimensions
        if (pyuvWidth > (unsigned int) wxSystemSettings::GetMetric(wxSYS_SCREEN_X))
            pyuvScale = 1 + pyuvWidth / wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
        if ((pyuvHeight / pyuvScale) > (unsigned int) wxSystemSettings::GetMetric(wxSYS_SCREEN_Y))
            pyuvScale = 1 + pyuvHeight / wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

        // fix check menu item
        switch (pyuvScale) {

        case (2):
            this->GetMenuBar()->Check(Menu_View_Zoom_50, true);
            break;

        case (3):
            this->GetMenuBar()->Check(Menu_View_Zoom_33, true);
            break;

        case (4):
            this->GetMenuBar()->Check(Menu_View_Zoom_25, true);
            break;

        case (1):
        default:
            pyuvScale = 1;
            this->GetMenuBar()->Check(Menu_View_Zoom_100, true);
            break;

        };

        // Update scaled dimensions
        pyuvWidthS = pyuvWidth / pyuvScale;
        pyuvHeightS = pyuvHeight / pyuvScale;

        // Set up
        info.Printf(_("%d frames, %.0f'%.0f\", %.3f MB"),
            pyuvNumframes,
            floorf(((double) pyuvNumframes / (double) pyuvRate) / 60.0F),
            ((double) pyuvNumframes / (double) pyuvRate) -
                60.0F * floorf(((double) pyuvNumframes / (double) pyuvRate) / 60.0F),
            (double) pyuvFilesize / 1048576.0F);
        SetStatusText(info, 1);
        pyuvDispose();
        pyuvPrepare();

        // Do a one-shot timer tick
        wxCommandEvent e;
        OnStop(e);

        // Associate the drawing image
        pyuvImg = &(drawCanvas->img);
    };

}

void pyuvFrame::OnClose(wxCommandEvent& event)
{
    // Stop the timer
    animTimer.Stop();

    // Close the file
    if (pyuvFile && pyuvFile->IsOpened()) {
        pyuvFile->Close();
        delete pyuvFile;
        pyuvFile = NULL;
    };
    pyuvFilename = wxT("");
    pyuvShowtimecode = false;
    GetMenuBar()->SetLabel(Menu_Control_Play, _("P&lay\tSpace"));
    playButton->SetBitmapLabel(wxBitmap(play16));
    GetMenuBar()->Enable(Menu_Control_Play, false);
    playButton->Enable(false);
    GetMenuBar()->Enable(Menu_Control_Stop, false);
    stopButton->Enable(false);
    GetMenuBar()->Enable(Menu_Control_Rewind, false);
    GetMenuBar()->Enable(Menu_Control_Forward, false);
    GetMenuBar()->Enable(Menu_Control_Goto, false);
    GetMenuBar()->Enable(Menu_Control_Loop, false);
    SetStatusText(wxT(""), 1);
    SetTitle(pyuvTopbasetitle + _(" - a portable YUV player"));

    // Abandon your master
    pyuvApp::GetMyMaster() = -1;
    SetOwnBackgroundColour(pyuvBackColor);
    pyuvApp::GetHaveClientFlag() = false;
    int i;
    for (i = 0; i < MAX_NO_SERVERS; i++)
        (*pyuvApp::GetMyClient())[i] = false;

    // Reset components
    pyuvShowcomps = 0;

    // Associate the background image
    pyuvImg = &(drawCanvas->logoimg);

    // Recreate the drawing bitmap
    delete drawCanvas->pyuvRowstart;
    delete drawCanvas->pyuvPixel;
    delete drawCanvas->pyuvBmpdata;
    delete drawCanvas->pyuvBmp;
       drawCanvas->pyuvBmp      = new wxBitmap(drawCanvas->logoimg);
    drawCanvas->pyuvBmpdata  = new PixelData(*(drawCanvas->pyuvBmp));
    drawCanvas->pyuvPixel    = new PixelData::Iterator(*(drawCanvas->pyuvBmpdata));
    drawCanvas->pyuvRowstart = new PixelData::Iterator(*(drawCanvas->pyuvBmpdata));

    // Reset iterator
    drawCanvas->pyuvPixel->Reset(*(drawCanvas->pyuvBmpdata));

    // Copy data
    /*int x, y;
    for (y = 0; y < pyuvHeightS; ++y) {

        *(drawCanvas->pyuvRowstart) = *(drawCanvas->pyuvPixel);

        for (x = 0; x < pyuvWidthS; ++x, ++(*(drawCanvas->pyuvPixel))) {

            pyuvScreen[3 * (y * pyuvWidthS + x)]     = drawCanvas->pyuvPixel->Red();
            pyuvScreen[3 * (y * pyuvWidthS + x) + 1] = drawCanvas->pyuvPixel->Green();
            pyuvScreen[3 * (y * pyuvWidthS + x) + 2] = drawCanvas->pyuvPixel->Blue();
        }

        *(drawCanvas->pyuvPixel) = *(drawCanvas->pyuvRowstart);
        drawCanvas->pyuvPixel->OffsetY(*(drawCanvas->pyuvBmpdata), 1);
    }*/

    // Fixes the size
    pyuvWidth  = pyuvWidthS  = PYUV_WIDTH;
    pyuvHeight = pyuvHeightS = PYUV_HEIGHT;
    drawCanvas->SetMinSize(wxSize(pyuvWidth, pyuvHeight));
    drawCanvas->SetMaxSize(wxSize(pyuvWidth, pyuvHeight));

    // prepare
    pyuvPrepare();

    // Adjust the canvas in the middle
    drawCanvas->SetSize(PYUV_HORIZBORD, PYUV_VERTBORD, pyuvWidth, pyuvHeight);

    // set minimum and maximum slider values
    pyuvNumframes = 0;
    frameSlider->SetValue(0);
    frameSlider->SetRange(0, (pyuvNumframes > 0) ? (pyuvNumframes - 1) : 0); 

    // Readjust minimum width
    frameSizer->Layout();
    frameSizer->SetSizeHints(this);
    frameSizer->Fit(this);

    // Refresh
    ClearBackground();
    drawCanvas->Refresh(false);
}

void pyuvFrame::OnFormat(wxCommandEvent& event)
{
    // Invoke the format selection dialog
    pyuvFormatDialog dialog(this);
    dialog.ShowModal();
}

void pyuvFrame::OnQuit(wxCommandEvent& event)
{
    // Destroy the frame
    Close();
}

void pyuvFrame::OnScroll(wxScrollEvent& event)
{
    pyuvFramenumber = frameSlider->GetValue();
    moveto(pyuvFramenumber);
}

void pyuvFrame::moveto(int frameno)
{

    // invalid seeked frame
    if (frameno < 0)
        frameno = 0;
    if (frameno >= (int) pyuvNumframes)
        frameno = pyuvNumframes - 1;

    // check if file is opened
    if (!pyuvFile || !pyuvFile->IsOpened())
        return;

    // we move to the indicated frame in file
    pyuvFile->Seek((wxFileOffset) pyuvFramebytes * (wxFileOffset) frameno, wxFromStart);

    // draw the frame
    if (!pyuvPlay)
        pyuvDraw();

    // Send the command to your slave(s)
    if (pyuvApp::GetHaveClientFlag() && (pyuvApp::GetMyMaster() == -1)) {
        char instr[16] = "";
        sprintf(instr, "moveto%d", frameno);
        sendcommand(instr);
    };
}

// Control menu handlers
// Play menu handler
void pyuvFrame::OnPlay(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    if (!pyuvPlay) {

        // Go to Play mode
		pyuvSW.Resume();
		//wxStopWatch sw;
        animTimer.Start(pyuvSleepTime + 0.5);
        pyuvPlay = true;
        mbar->SetLabel(Menu_Control_Play, _("P&ause\tSpace"));
        playButton->SetBitmapLabel(wxBitmap(pause16));
        mbar->Enable(Menu_Control_Stop, true);
        stopButton->Enable(true);
        mbar->Enable(Menu_Control_Rewind, true);
        mbar->Enable(Menu_Control_Forward, true);

    } else {

        // Go to Pause mode
		pyuvSW.Pause();
        animTimer.Stop();
        pyuvPlay = false;
        mbar->SetLabel(Menu_Control_Play, _("P&lay\tSpace"));
        playButton->SetBitmapLabel(wxBitmap(play16));

    };

    // Send the command to your slave(s)
    if (pyuvApp::GetHaveClientFlag() && (pyuvApp::GetMyMaster() == -1)) {
        
        sendcommand((char *) "play");
    };
}

// Stop menu handler
void pyuvFrame::OnStop(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Stop the timer and rewind the file
	pyuvSW.Start();
	pyuvSW.Pause();
    animTimer.Stop();
    if (pyuvFile && pyuvFile->IsOpened())
        pyuvFile->Seek(pyuvHeadersize, wxFromStart);

    // Not playing now
    pyuvPlay = false;
    mbar->SetLabel(Menu_Control_Play, _("P&lay\tSpace"));
    playButton->SetBitmapLabel(wxBitmap(play16));
    mbar->Enable(Menu_Control_Play, true);
    playButton->Enable(true);
    mbar->Enable(Menu_Control_Stop, false);
    stopButton->Enable(false);
    mbar->Enable(Menu_Control_Rewind, false);
    mbar->Enable(Menu_Control_Forward, false);
    mbar->Enable(Menu_Control_Goto, true);
    mbar->Enable(Menu_Control_Loop, true);

    // Do a one-shot timer tick
    pyuvPrepare();
    pyuvFramenumber = 0;
    pyuvDraw();
	pyuvTotalDoneFrames = 0;

    // Move the slider from time to time
    if (!(pyuvFramenumber % pyuvSliderupdate))
        frameSlider->SetValue(pyuvFramenumber);

    // Send the command to your slave(s)
    if (pyuvApp::GetHaveClientFlag() && (pyuvApp::GetMyMaster() == -1)) {
        
        sendcommand((char *)"stop");
    };
}

// Rewind menu handler
void pyuvFrame::OnRewind(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Go back and draw
    if (--pyuvFramenumber == (unsigned long) -1)
        pyuvFramenumber = 0;
    moveto(pyuvFramenumber);

    // Move the slider from time to time
    if (!(pyuvFramenumber % pyuvSliderupdate))
        frameSlider->SetValue(pyuvFramenumber);
    
    pyuvPlay = false;
    mbar->SetLabel(Menu_Control_Play, _("P&lay\tSpace"));
    playButton->SetBitmapLabel(wxBitmap(play16));

}

// Forward menu handler
void pyuvFrame::OnForward(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Go ahead and draw
    /*animTimer.Start(-1, wxTIMER_ONE_SHOT);*/
    if (++pyuvFramenumber >= pyuvNumframes)
        pyuvFramenumber = pyuvNumframes - 1;
    moveto(pyuvFramenumber);

    // Move the slider from time to time
    if (!(pyuvFramenumber % pyuvSliderupdate))
        frameSlider->SetValue(pyuvFramenumber);

    mbar->Enable(Menu_Control_Rewind, true);
    mbar->Enable(Menu_Control_Stop, true);
    stopButton->Enable(true);
    pyuvPlay = false;
    mbar->SetLabel(Menu_Control_Play, _("P&lay\tSpace"));
    playButton->SetBitmapLabel(wxBitmap(play16));

    // Send the command to your slave(s)
    if (pyuvApp::GetHaveClientFlag() && (pyuvApp::GetMyMaster() == -1)) {
        
        sendcommand((char *)"forward");
    };
}

// Goto menu handler
void pyuvFrame::OnGoto(wxCommandEvent& event)
{
    pyuvFramenumber = (long unsigned int) wxGetNumberFromUser(wxString::Format(_("You can directly go to a frame\nin the video sequence\n(select a number between 0 and %d)"), pyuvNumframes - 1),
                                _("Jump to frame:"), // prompt text
                                _("Frame selection dialog"), // title test
                                pyuvFramenumber, // default value
                                0, // minimum value
                                pyuvNumframes - 1 // maximum value
                                , NULL, wxDefaultPosition);

    moveto(pyuvFramenumber);

    // Move the slider from time to time
    if (!(pyuvFramenumber % pyuvSliderupdate))
        frameSlider->SetValue(pyuvFramenumber);

}

// Timecode menu handler
void pyuvFrame::OnTimecode(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Hide or show the timecode
    if (mbar->IsChecked(Menu_View_Timecode))
        pyuvShowtimecode = true;
    else
        pyuvShowtimecode = false;

    // Compute new sequence time
    if (pyuvShowtimecode) {

        float tot_time = (float) pyuvFramenumber / (float) pyuvRate;
        float tot_min  = tot_time / 60.0F;
        float tot_sec  = tot_time - (floorf(tot_min) * 60.0F);
        float tot_cent = 100.0F * (tot_time - floorf(tot_time));

        pyuvCurtime.Printf(wxT("%04u: %02.0f'%02.0f\"%02.0f"),
                            pyuvFramenumber,
                            floorf(tot_min),
                            floorf(tot_sec),
                            tot_cent);
    };

    // Send an OnPaint event
    drawCanvas->Refresh(false);
}

// Deinterlace menu handler
void pyuvFrame::OnDeinterlace(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    /*wxMessageBox(wxT("identitymenucall"), wxT("Busy"), wxICON_INFORMATION|wxOK);*/

    // Hide or show the timecode
    if (mbar->IsChecked(Menu_View_Deinterlace))
        pyuvDeinterlace = true;
    else
        pyuvDeinterlace = false;

    // Send an OnPaint event
    drawCanvas->Refresh(false);
}

// Identity menu handler
void pyuvFrame::OnIdentity(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    /*wxMessageBox(wxT("identitymenucall"), wxT("Busy"), wxICON_INFORMATION|wxOK);*/

    // Hide or show the timecode
    if (mbar->IsChecked(Menu_View_Identity))
        pyuvShowidentity = true;
    else
        pyuvShowidentity = false;

    // Send an OnPaint event
    drawCanvas->Refresh(false);
}

// Switch identity
void pyuvFrame::switchidentity(void)
{
    wxMenuBar *mbar = GetMenuBar();

    pyuvShowidentity = !pyuvShowidentity;

    mbar->Check(Menu_View_Identity, pyuvShowidentity);

    // Send an OnPaint event
    drawCanvas->Refresh(false);
}

// Send a command
bool pyuvFrame::sendcommand(char *cmd)
{
    pyuvClient* client = new pyuvClient();
    wxConnectionBase* connection = NULL;
    int servno;

    client->SetFrame(this);

    for (servno = 0; servno < MAX_NO_SERVERS; servno++) {

        // Disable screen warnings of MakeConnection()
        wxLogNull noLog;

        // Skip yourself and nonselected ones
        if ((servno == pyuvApp::GetServerNo()) || !pyuvApp::GetMyClient()[servno])
            continue;

        // Look for available server
        if ((connection = client->MakeConnection(wxT("localhost"), 
                            wxString::Format(wxT("/tmp/PYUV%02d"), servno),
                            wxT("dothis")))) {
            // Ask the other instance, booking in case
            /*if (!connection->Execute(wxString::FromAscii(cmd)))
                pyuvApp::GetMyClient()[servno] = false;*/
            connection->Disconnect();

        } else {

            // no more connected
            (*pyuvApp::GetMyClient())[servno] = false;
        };

        delete connection;

    }
    delete client;

    return true;
}

// Scaling menu handler
void pyuvFrame::OnScale(wxCommandEvent& event)
{
    float readjust;

    switch (event.GetId()) {

    case (Menu_View_Zoom_400):
        pyuvScale = -4;
        break;

    case (Menu_View_Zoom_200):
        pyuvScale = -2;
        break;

    case (Menu_View_Zoom_100):
        pyuvScale = 1;
        break;

    case (Menu_View_Zoom_50):
        pyuvScale = 2;
        break;

    case (Menu_View_Zoom_33):
        pyuvScale = 3;
        break;

    case (Menu_View_Zoom_25):
        pyuvScale = 4;
        break;

    default:
        pyuvScale = 1;
        wxMessageBox(_("Scale is unknown: assuming 100%"),
                 _("Scaling"),
                 wxOK | wxICON_WARNING,
                 this);
        break;
    }

    // anamorphic
    if (pyuvAnamorphic)
        readjust = 16.0F / 9.0F;
    else
        readjust = 1.0F;

    // Update scaled dimensions
    if (pyuvScale >= 1) {
        // Down
        pyuvWidthS = (unsigned long) (readjust * (float) pyuvWidth / (float) pyuvScale);
        pyuvHeightS = pyuvHeight / pyuvScale;
    } else {
        // Up
        pyuvWidthS = (unsigned long) (readjust * (float) pyuvWidth * (float) (-pyuvScale));
        pyuvHeightS = pyuvHeight * (-pyuvScale);
    }

    // Reprepare screen
    pyuvPrepare();

    // Send an OnPaint event
    moveto(pyuvFramenumber);
}

// Component menu handler
void pyuvFrame::OnComp(wxCommandEvent& event)
{
    switch (event.GetId()) {

    case (Menu_View_Comp_All):
        pyuvShowcomps = 0;
        break;

    case (Menu_View_Comp_Next):
        pyuvShowcomps = abs((pyuvShowcomps + 1) % 4);
        break;

    default:
        break;
    };

    // Send an OnPaint event
    moveto(pyuvFramenumber);
}

// Copy menu handler
void pyuvFrame::OnCopy(wxCommandEvent& event)
{
    // Write some text to the clipboard
    if (wxTheClipboard->Open()) {

        // This data objects are held by the clipboard, 
        // so do not delete them in the app.
        /*wxTheClipboard->SetData( new wxTextDataObject("Some text") );*/
        wxTheClipboard->SetData( new wxBitmapDataObject(wxBitmap(*pyuvImg)) );
        wxTheClipboard->Close();
    }

}

// Engine change handler
void pyuvFrame::OnEngine(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Re-enable double buffer menu
    mbar->Enable(Menu_Settings_Draw_Double, true);

    switch (event.GetId()) {

    case (Menu_Settings_Draw_Bitmap):
        pyuvEngine = 0;
        break;

    case (Menu_Settings_Draw_Raw):
        pyuvEngine = 1;
        break;

    case (Menu_Settings_Draw_OGL):
        pyuvEngine = 2;
        mbar->Enable(Menu_Settings_Draw_Double, false);
        break;

    default:
        break;
    };

}

// Double buffering handler
void pyuvFrame::OnDouble(wxCommandEvent& event)
{
    wxMenuBar *mbar = GetMenuBar();

    // Enable or not the double buffering
    if (mbar->IsChecked(Menu_Settings_Draw_Double))
        pyuvDoublebuffer = true;
    else
        pyuvDoublebuffer = false;

    // Send an OnPaint event
    drawCanvas->Refresh(false);

}

// Loop menu handler
void pyuvFrame::OnLoop(wxCommandEvent& event)
{
    // Enable or disable the continuous sequence loop
    pyuvLoop = !pyuvLoop;
}

void pyuvFrame::OnSync(wxCommandEvent& event)
{
    // Invoke the format selection dialog
    pyuvSyncDialog dialog(this);
    dialog.ShowModal();
}

// Help menu handlers
// Contents handler
void pyuvFrame::OnContents(wxCommandEvent& event)
{
    if (!pyuvHelpsystem)
        wxMessageBox(_("The file '") + wxString(wxT(HELP_FILENAME)) + _("' is missing in\nthe program directory:\nthe help system is not available"));
    else
        help.Display(wxT("doc.htm"));
}

// About menu handler
void pyuvFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg;

    msg.Printf(
        _("PYUV - A portable and multiplatform RAW sequence player\n") +
        _("Version %s - ") +
#ifdef __WXMSW__
        _("for Windows\n") +
#endif
#ifdef __WXGTK__
        _("for Linux\n") + 
#endif
 #ifdef __WXMAC__
        _("for Mac OS X\n") + 
#endif
       wxString(wxT("(%s)\n\n")) +
        _("Website: %s\n") + 
        wxString(wxT("Github:  %s\n")) +
        _("\nCreated with %s\n") +
        wxString(wxT("\n(c) 2006-2016, Giuseppe Baruffa, DSPLab")),
        PACKAGE_VERSION,
        PYUV_BTIME,
        wxT(PYUV_WEBSITE),
        wxT(PYUV_GITHUB),
        wxVERSION_STRING);
        
    // Display a message box with the info
    wxMessageBox(msg, _("About PYUV"), wxOK | wxICON_INFORMATION, this);
}

// Animation timer handler
bool pyuvFrame::pyuvDraw(void)
{
#if 0
    int n, area = pyuvWidth * pyuvHeight * 3;
#endif

    if (readfile(pyuvBuffer, pyuvFramebytes)) {

        // Adjust samples depth to 8 bit, unsigned
        //if (pyuvSigned || pyuvSamplebits != 8)
            bitdepth_adjust(pyuvBuffer, (void *) pyuvBuffer, pyuvFramesize, pyuvSamplebits,
                            pyuvSigned, pyuvEndianness);

        // Convert the downsampled frame to interleaved 1:1:1

        // Planar or interleaved?
        if (!pyuvInterleaved) {

            // Planar
            switch (pyuvSubsamplingnumber) {

            case (SS_444):
                if (pyuvInterlaced)
                    p444i_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                else {
                    if (pyuvOrder == OR_BGR)
                        p444_321_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    else if (pyuvOrder == OR_GBR)
                        p444_231_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    else
                        p444_123_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                }
                break;

            case (SS_422):
                p422_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_420):
                p420_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_411):
                p411_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_410):
                p410_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_400):
                p400_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            default:
                break;
            };

        } else {

            // Interleaved

            switch (pyuvSubsamplingnumber) {

            case (SS_444):
                i444_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_422):

                switch (pyuvOrder) {

                case (OR_UYVY):
                    iuyvy422_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    break;

                case (OR_YUY2):
                    iyuy2422_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    break;

                case (OR_YVYU):
                    iyvyu422_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    break;

                default:
                    i422_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                    break;

                }
                break;

            case (SS_420):
                    i420_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_411):
                i411_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            case (SS_410):
                i410_to_i111(pyuvScreen, pyuvBuffer, pyuvWidth, pyuvHeight);
                break;

            default:
                break;
            };


        };

        // draw strange things
        //difference(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
        //drawgrid(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight, pyuvFramenumber + 1, 0x00ff00);

        // Convert from colorspace to RGB
        if (pyuvShowcomps)
            
            // Just show one component
            whatever_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight, pyuvShowcomps);

        else {

            // Show all components
            switch (pyuvColorspacenumber) {

            case (CS_YCBCR):
            case (CS_YUV):
                ycbcr601_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_YCBCR709):
                ycbcr709_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_XYZ):
                xyzD65_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_RGB):
                rgb_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_HSV):
                hsv_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_YIQ):
                yiq_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_YCOCG):
                ycocg_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_YCXCZ):
                ycxcz_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            case (CS_BW):
                bw_to_rgb(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);
                break;

            default:
                break;
            };
        }

        // Deinterlace
        if (pyuvDeinterlace) {

            deinterlace(pyuvScreen, pyuvScreen, pyuvWidth, pyuvHeight);

        }

        // apply screen lut
        prepare_screen(pyuvScreen, pyuvScreen,  pyuvWidth, pyuvHeight);

        // Drawing engines
        if (pyuvEngine == 0) {

            // normal bitmap

            // check scaling
            if (pyuvAnamorphic) {
                if (pyuvScale > 1)
                    downsample_anamorphic(pyuvScreenS, pyuvScreen, pyuvWidth, pyuvHeight, pyuvWidthS, pyuvHeightS, pyuvScale);
                else if (pyuvScale == 1)
                    upsample_anamorphic(pyuvScreenS, pyuvScreen, pyuvWidth, pyuvHeight, pyuvWidthS, pyuvHeightS, pyuvScale);
                else
                    upsample_anamorphic(pyuvScreenS, pyuvScreen, pyuvWidth, pyuvHeight, pyuvWidthS, pyuvHeightS, -pyuvScale);
            } else {
                if (pyuvScale > 1)
                    downsample(pyuvScreenS, pyuvScreen, pyuvWidth, pyuvHeight, pyuvWidthS, pyuvHeightS, pyuvScale);
                else if (pyuvScale == 1)
                    memcpy(pyuvScreenS, pyuvScreen, pyuvWidth * pyuvHeight * 3);
                else
                    upsample(pyuvScreenS, pyuvScreen, pyuvWidth, pyuvHeight, pyuvWidthS, pyuvHeightS, -pyuvScale);
            }

        } else if (pyuvEngine == 1) {

            // Let's update the screen buffer with a pixel iterator

            // Reset iterator
            drawCanvas->pyuvPixel->Reset(*(drawCanvas->pyuvBmpdata));

            // Copy data
            int row, col;
            unsigned char *origrow = pyuvScreen, *orig = pyuvScreen;
            unsigned int colstep = 3 * (pyuvScale - 1) + 1;
            unsigned int rowstep = 3 * pyuvWidth * pyuvScale;
            for (row = 0; row < (int) pyuvHeightS; ++row) {

                // Beginning of a row
                orig = origrow;
                *(drawCanvas->pyuvRowstart) = *(drawCanvas->pyuvPixel);

                for (col = 0; col < (int) pyuvWidthS; ++col, ++(*(drawCanvas->pyuvPixel))) {

                    // copy values from memory buffer to screen buffer
                    drawCanvas->pyuvPixel->Red()   = *(orig++);
                    drawCanvas->pyuvPixel->Green() = *(orig++);
                    drawCanvas->pyuvPixel->Blue()  = *(orig);

                    // next good column
                    orig += colstep;

                }

                // next good row
                origrow += rowstep;
                *(drawCanvas->pyuvPixel) = *(drawCanvas->pyuvRowstart);
                drawCanvas->pyuvPixel->OffsetY(*(drawCanvas->pyuvBmpdata), 1);
            }
        }

        // Compute new sequence time
        if (pyuvShowtimecode) {

            float tot_time = (float) pyuvFramenumber / (float) pyuvRate;
            float tot_min  = tot_time / 60.0F;
            float tot_sec  = tot_time - (floorf(tot_min) * 60.0F);
            float tot_cent = 100.0F * (tot_time - floorf(tot_time));

            /*pyuvCurtime.Printf(wxT("%04u: %02.0f'%02.0f\"%02.0f"),
                pyuvFramenumber,
                floorf(tot_min),
                floorf(tot_sec),
                tot_cent);*/

			float cur_fps = 1000.0F * pyuvTotalDoneFrames / pyuvSW.Time();

            pyuvCurtime.Printf(wxT("%04u: %02.0f'%02.0f\"%02.0f %.2f"),
                pyuvFramenumber,
                floorf(tot_min),
                floorf(tot_sec),
                tot_cent, cur_fps);
        };

        // Send an OnPaint event
        drawCanvas->Refresh(false);

        return true;

    } else

        return false;
}

// Animation timer handler
void pyuvFrame::OnAnimate(wxTimerEvent& event)
{
    // Try to load a frame
    if (!pyuvFile || !pyuvFile->IsOpened() || !pyuvBuffer)
        return;

    if (pyuvDraw()) {

        // Move the slider from time to time
        if (!(pyuvFramenumber % pyuvSliderupdate))
            frameSlider->SetValue(pyuvFramenumber);

        // Increment frame number
        pyuvFramenumber++;
		pyuvTotalDoneFrames++;

		// adjust animation timing from time to time
		/*if (!(pyuvTotalDoneFrames % 25))
		{
			double real_fps = 1000 * pyuvTotalDoneFrames / pyuvSW.Time();
			pyuvSleepTime = pyuvFrameTime * 0.8 * (real_fps / pyuvRate);
			if (pyuvSleepTime < 1)
				pyuvSleepTime = 1;
			animTimer.Start(pyuvSleepTime + 0.5);
		}*/

    } else {

        // The file is finished
        if (pyuvLoop) {

            // Rewind
            pyuvFile->Seek(pyuvHeadersize, wxFromStart);
            pyuvFramenumber = 0;

        } else {

            // Press stop
            wxCommandEvent e;
            OnStop(e);

        };

    };

}

// Read data from file, accounting for visual scaling
bool pyuvFrame::readfile(unsigned char *buffer, int numbytes)
{

    // Open and display at no scale
    if (pyuvFile->Read(buffer, numbytes) == numbytes)
        return true;
    else
        return false;

    // There is a scale: read as per the file format
    return false;
}

// Prepare visualization environment
void pyuvFrame::pyuvPrepare(void)
{
    float readjust;

    // anamorphic
    if (pyuvAnamorphic)
        readjust = 16.0F / 9.0F;
    else
        readjust = 1.0F;

    // Update scaled dimensions
    if (pyuvScale >= 1) {
        // Down
        pyuvWidthS = (unsigned long) (readjust * (float) pyuvWidth / (float) pyuvScale);
        pyuvHeightS = pyuvHeight / pyuvScale;
    } else {
        // Up
        pyuvWidthS = (unsigned long) (readjust * (float) pyuvWidth * (float) (-pyuvScale));
        pyuvHeightS = pyuvHeight * (-pyuvScale);
    }

    // Prepare the screen and the buffer: Realloc() the space
    if (!(pyuvScreen = (unsigned char *) realloc((unsigned char *) pyuvScreen,
        pyuvWidth * pyuvHeight * 3 * sizeof(unsigned char)))) {
        return;
    };
    if (!(pyuvScreenS = (unsigned char *) realloc((unsigned char *) pyuvScreenS,
        pyuvWidthS * pyuvHeightS * 3 * sizeof(unsigned char)))) {
        return;
    };
    if (!(pyuvBuffer = (unsigned char *) realloc((unsigned char *) pyuvBuffer, pyuvFramebytes))) {
        return;
    };

    // Prepare the loading lut
    prepare_lut(pyuvSamplebits, pyuvSigned, pyuvEndianness, pyuvGamma);

    // Compose a bitmap to show
    memset(pyuvScreenS, 0, pyuvWidthS * pyuvHeightS * 3);

    // Sets the animation timer with proper interval - will be adjusted along the way
    pyuvFrameTime = (int) floor((1000.0 / pyuvRate) + 0.5);
	pyuvSleepTime = pyuvFrameTime;

    // Recreate the drawing bitmap
    delete drawCanvas->pyuvRowstart;
    delete drawCanvas->pyuvPixel;
    delete drawCanvas->pyuvBmpdata;
    delete drawCanvas->pyuvBmp;
    drawCanvas->pyuvBmp      = new wxBitmap(wxImage(pyuvWidthS, pyuvHeightS));
    drawCanvas->pyuvBmpdata  = new PixelData(*(drawCanvas->pyuvBmp));
    drawCanvas->pyuvPixel    = new PixelData::Iterator(*(drawCanvas->pyuvBmpdata));
    drawCanvas->pyuvRowstart = new PixelData::Iterator(*(drawCanvas->pyuvBmpdata));

    // Fixes the size
    drawCanvas->SetMinSize(wxSize(pyuvWidthS, pyuvHeightS));
    drawCanvas->SetMaxSize(wxSize(pyuvWidthS, pyuvHeightS));

    // Adjust the canvas in the middle
    drawCanvas->SetSize(PYUV_HORIZBORD, PYUV_VERTBORD, pyuvWidthS, pyuvHeightS);

    // Create an image
    drawCanvas->img.~wxImage();
    drawCanvas->img = wxImage(pyuvWidthS, pyuvHeightS, (unsigned char *) pyuvScreenS, true);

    // set minimum and maximum slider values
    frameSlider->SetRange(0, (pyuvNumframes > 0) ? (pyuvNumframes - 1) : 0); 

    // how often the slider updates
    if (pyuvNumframes < 50)
        pyuvSliderupdate = 1; // short file, always
    else
        pyuvSliderupdate = (unsigned int) pyuvRate; // long file, circa once per second

    // Adjust the settings for the timecode show
    wxClientDC dc(drawCanvas);
    dc.SetFont(pyuvTimeFont);
    dc.GetTextExtent(pyuvCurtime, &tcwidth, &tcheight);
    tcx = 3;
    tcy = pyuvHeightS - 2 - tcheight;

    // Adjust the settings for the identity show
    dc.SetFont(pyuvIdFont);
    dc.GetTextExtent(wxString::Format(wxT("%02d"), pyuvApp::GetServerNo()), &idwidth, &idheight);
    idx = pyuvWidthS - 2 - idwidth;
    idy = 3;

    // Readjust minimum width
    frameSizer->Layout();
    frameSizer->SetSizeHints(this);
    frameSizer->Fit(this);
}

// Dispose of old sequence
void pyuvFrame::pyuvDispose(void)
{
    // Stop the animation, if any
    wxCommandEvent e;
    OnStop(e);
}
