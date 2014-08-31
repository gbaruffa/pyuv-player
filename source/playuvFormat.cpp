// Name:    playuvApp.cpp
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

// turn off 'warning C4355: 'this' : used in base member initializer list'
#if defined(_WIN32_)
    #pragma warning ( disable:4355 )
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "playuvFormat.cpp"
    #pragma interface "playuvFormat.cpp"
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
#include <wx/propdlg.h>
#include <wx/spinctrl.h>
#include <wx/spinbutt.h>
#include <wx/rawbmp.h>

#include <stdint.h>

// Inclusion of the main header file
#include "playuvFrame.h"
#include "playuvApp.h"
#include "playuvFormat.h"

/////////////////////////////////////////////////////////
// Video format selection dialog methods
/////////////////////////////////////////////////////////
wxIMPLEMENT_CLASS(pyuvFormatDialog, wxPropertySheetDialog)

wxBEGIN_EVENT_TABLE(pyuvFormatDialog, wxPropertySheetDialog)
    EVT_SPIN(ID_INTRATESPIN,      pyuvFormatDialog::iratespinHandler)
    EVT_SPIN(ID_FRACRATESPIN,     pyuvFormatDialog::fratespinHandler)
    EVT_TEXT_ENTER(ID_RATETEXT,   pyuvFormatDialog::ratetextHandler)
    EVT_BUTTON(wxID_OK,           pyuvFormatDialog::formatApplyClick)
    EVT_BUTTON(wxID_APPLY,        pyuvFormatDialog::formatApplyClick)
    EVT_CHOICE(ID_SIZECHOICE,     pyuvFormatDialog::sizechoiceHandler)
    EVT_TEXT_ENTER(ID_WIDTHTEXT,  pyuvFormatDialog::sizetextHandler)
    EVT_TEXT_ENTER(ID_HEIGHTTEXT, pyuvFormatDialog::sizetextHandler)
    EVT_CHECKBOX(ID_INTERLEAVED,  pyuvFormatDialog::interleavedcheckHandler)
wxEND_EVENT_TABLE()

const uint32_t pyuvFormatDialog::pyuvWidths[] =
{
	0,      // choose one
	176,    // qcif
	352,    // sif
	352,    // cif
	640,    // vga
	700,    // ntsc
	720,    // sd576
	780,    // pal
	1280,   // hd720
	1920,   // hd1080
	2048,   // dci2k
	3840,   // shd
	4096,   // dci4k
	4520,   // red
	7680,   // uhd
	0       // custom
};

const uint32_t pyuvFormatDialog::pyuvHeights[] =
{
	0,      // choose one
	144,    // qcif
	240,    // sif
	288,    // cif
	480,    // vga
	525,    // ntsc
	576,    // sd576
	586,    // pal
	720,    // hd720
	1080,   // hd1080
	1080,   // dci2k
	2160,   // shd
	2160,   // dci4k
	2540,   // red
	4320,   // uhd
	0       // custom
};

// Video resolutions: short names and sizes
const wxString pyuvFormatDialog::pyuvResolutions[] =
{
    wxT("Choose one"),
    wxT("QCIF"), wxT("SIF"), wxT("CIF"), wxT("VGA"), wxT("NTSC"),
    wxT("SD576"), wxT("PAL"), wxT("HD720"), wxT("HD1080"), wxT("DCI2K"),
    wxT("SHD"), wxT("DCI4K"), wxT("RED"), wxT("UHD"),
    wxT("Custom")
};

const wxString pyuvFormatDialog::pyuvSampling[] =
{
    wxT("Choose one"),
    wxT("4:4:4"), wxT("4:2:2"), wxT("4:2:0"), wxT("4:1:1"), wxT("4:1:0"),
    wxT("4:0:0"),
    wxT("Custom")
};

const wxString pyuvFormatDialog::pyuvFourcc[] =
{   wxT("Default"),
    wxT("UYVY"), wxT("YUY2"), wxT("YVYU"), wxT("BGR"),
    wxT("Custom")
};

pyuvFrame *pyuvFormatDialog::frame;

// Ctor
pyuvFormatDialog::pyuvFormatDialog(wxWindow *win)
{
    frame = (pyuvFrame *)GetParent();

    frame->GetWidthScreen() = frame->GetWidth() / frame->GetScale();
    frame->GetHeightScreen() = frame->GetHeight() / frame->GetScale();

    // Transfer all data from window children
    // Activate a contextual help
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    // Create the window
    Create(win, wxID_ANY, wxT("Video sequence format"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE);

    // Get the top sizer
    wxSizer* topSizer = this->GetInnerSizer();

    // Create the bottom buttons
    wxStdDialogButtonSizer* buttons = new wxStdDialogButtonSizer();
    buttons->AddButton(new wxButton(this, wxID_OK));
    buttons->AddButton(new wxButton(this, wxID_APPLY));
    buttons->AddButton(new wxButton(this, wxID_CANCEL));
    buttons->Realize();
    topSizer->Add(buttons, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

    // Get the notebook window
    wxBookCtrlBase* notebook = GetBookCtrl();

    // Create the main settings tab
    wxPanel* mainSettings = CreateMainSettingsPage(notebook);

    // Create the additional settings tab
    wxPanel* extraSettings = CreateExtraSettingsPage(notebook);

    // Add the tabs to the dialog
    notebook->AddPage(mainSettings, wxT("Main settings"));
    notebook->AddPage(extraSettings, wxT("Extra settings"));

    // Lay out the dialog
    LayoutDialog();
}

// Main settings tab
wxPanel* pyuvFormatDialog::CreateMainSettingsPage(wxWindow* parent)
{
    int i;

    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *item0 = new wxBoxSizer(wxVERTICAL);

    ////////////////////////
    //// Frame size
    ////////////////////////
    wxStaticBox* framesizeBox = new wxStaticBox(panel, wxID_ANY, wxT("Frame size"));
    wxBoxSizer* framesizeSizer = new wxStaticBoxSizer(framesizeBox, wxVERTICAL);
    item0->Add(framesizeSizer, 0, wxGROW|wxALL, 3);

    // First row
    wxBoxSizer* itemSizer2 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer2->Add(new wxStaticText(panel, wxID_ANY, wxT("Resolution:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer2->Add(3, 3, 1, wxALL, 0);

    wxArrayString standardChoices(WXSIZEOF(pyuvResolutions), pyuvResolutions);
    standardsizeChoice = new wxChoice(panel, ID_SIZECHOICE,
        wxDefaultPosition, wxDefaultSize, standardChoices);
    standardsizeChoice->SetSelection(WXSIZEOF(pyuvResolutions) - 1);
    for (i = 0; i < (int) WXSIZEOF(pyuvResolutions); i++)
        if ((pyuvWidths[i] == frame->GetWidth()) && (pyuvHeights[i] == frame->GetHeight()))
            standardsizeChoice->SetSelection(i);

    itemSizer2->Add(standardsizeChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Second row
    wxBoxSizer* itemSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemSizer3->Add(new wxStaticText(panel, wxID_ANY, wxT("Size (WxH):")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer3->Add(3, 3, 1, wxALL, 0);

    customwidthText = new wxTextCtrl(panel, ID_WIDTHTEXT, wxString::Format(wxT("%d"), frame->GetWidth()),
        wxDefaultPosition, wxSize(60, -1), wxTE_RIGHT);
    itemSizer3->Add(customwidthText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer3->Add(new wxStaticText(panel, wxID_ANY, wxT("x")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 1);

    customheightText = new wxTextCtrl(panel, ID_HEIGHTTEXT, wxString::Format(wxT("%d"), frame->GetHeight()),
        wxDefaultPosition, wxSize(60, -1));
    itemSizer3->Add(customheightText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Compose rows
    framesizeSizer->Add(itemSizer2, 0, wxGROW|wxALL, 3);
    framesizeSizer->Add(itemSizer3, 0, wxGROW|wxALL, 3);
    /////////////////////////////
    /////////////////////////////

    ////////////////////////
    //// Frame rate
    ////////////////////////
    wxStaticBox* framerateBox = new wxStaticBox(panel, wxID_ANY, wxT("Frame rate"));
    wxBoxSizer* framerateSizer = new wxStaticBoxSizer(framerateBox, wxVERTICAL);
    item0->Add(framerateSizer, 0, wxGROW|wxALL, 3);

    // First row
    wxBoxSizer* itemSizer4 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer4->Add(new wxStaticText(panel, wxID_ANY, wxT("Frames/second:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer4->Add(3, 3, 1, wxALL, 0);

    framerateText = new wxTextCtrl(panel, ID_RATETEXT, wxString::Format(wxT("%.2f"), frame->GetRate()),
        wxDefaultPosition, wxSize(60, -1), wxTE_LEFT);
    itemSizer4->Add(framerateText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    iframerateSpin = new wxSpinButton(panel, ID_INTRATESPIN, wxDefaultPosition,
        wxSize(-1, framerateText->GetSize().GetHeight()));
    iframerateSpin->SetRange(1, 60);
    iframerateSpin->SetValue((int) floorf((float)frame->GetRate()));
    itemSizer4->Add(iframerateSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer4->Add(new wxStaticText(panel, wxID_ANY, wxT(".")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 1);

    fframerateSpin = new wxSpinButton(panel, ID_FRACRATESPIN, wxDefaultPosition,
        wxSize(-1, framerateText->GetSize().GetHeight()));
    fframerateSpin->SetRange(100, 6000);
    fframerateSpin->SetValue((int) floorf(100.0F * (float)frame->GetRate()));
    itemSizer4->Add(fframerateSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Second row
    wxBoxSizer* itemSizer41 = new wxBoxSizer(wxHORIZONTAL);

    interlaceCheck = new wxCheckBox(panel, wxID_ANY, wxT("Interlaced"),
        wxDefaultPosition, wxDefaultSize);
    interlaceCheck->SetValue(frame->GetInterlacedFlag());
    interlaceCheck->Enable();
    itemSizer41->Add(interlaceCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    // Compose rows
    framerateSizer->Add(itemSizer4, 0, wxGROW|wxALL, 3);
    framerateSizer->Add(itemSizer41, 0, wxGROW|wxALL, 3);
    //////////////////////////////
    //////////////////////////////

    ///////////////////////////////
    //// Video format
    ///////////////////////////////
    wxStaticBox* videoformatBox = new wxStaticBox(panel, wxID_ANY, wxT("Video format"));
    wxBoxSizer* videoformatSizer = new wxStaticBoxSizer(videoformatBox, wxVERTICAL);
    item0->Add(videoformatSizer, 0, wxGROW|wxALL, 3);

    // First row
    wxBoxSizer* itemSizer5 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer5->Add(new wxStaticText(panel, wxID_ANY, wxT("Color space:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer5->Add(3, 3, 1, wxALL, 0);

    wxArrayString componentChoices(frame->GetSizeOfComponents(), (*frame->GetComponents()));
    componentChoice = new wxChoice(panel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, componentChoices);
    componentChoice->SetSelection(frame->GetColorSpaceNumber());

    itemSizer5->Add(componentChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // second row
    wxBoxSizer* itemSizer8 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer8->Add(new wxStaticText(panel, wxID_ANY, wxT("Gamma:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer8->Add(3, 3, 1, wxALL, 0);

    gammaText = new wxTextCtrl(panel, ID_GAMMATEXT, wxString::Format(wxT("%.3f"), frame->GetGamma()),
        wxDefaultPosition, wxSize(60, -1), wxTE_RIGHT);

    itemSizer8->Add(gammaText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Third row
    wxBoxSizer* itemSizer6 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer6->Add(new wxStaticText(panel, wxID_ANY, wxT("Subsampling:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer6->Add(3, 3, 1, wxALL, 0);

    wxArrayString samplingChoices(WXSIZEOF(pyuvSampling), pyuvSampling);
    samplingChoice = new wxChoice(panel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, samplingChoices);
    samplingChoice->SetSelection(frame->GetSubsamplingNumber());

    itemSizer6->Add(samplingChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Fourth row
    wxBoxSizer* itemSizer61 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer61->Add(new wxStaticText(panel, wxID_ANY, wxT("Ordering:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer61->Add(3, 3, 1, wxALL, 0);

    wxArrayString orderChoices(WXSIZEOF(pyuvFourcc), pyuvFourcc);
    orderChoice = new wxChoice(panel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, orderChoices);
    orderChoice->SetSelection(frame->GetYUVOrder());

    itemSizer61->Add(orderChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Fifth row
    wxBoxSizer* itemSizer7 = new wxBoxSizer(wxHORIZONTAL);

    interleaveCheck = new wxCheckBox(panel, ID_INTERLEAVED, wxT("Interleaved"),
        wxDefaultPosition, wxDefaultSize);
    interleaveCheck->SetValue(frame->GetInterleavedFlag());
    itemSizer7->Add(interleaveCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    /*if (interleaveCheck->IsChecked())
        orderChoice->Enable(true);
    else
        orderChoice->Enable(false);*/

    anamorphCheck = new wxCheckBox(panel, wxID_ANY, wxT("Anamorphic"),
        wxDefaultPosition, wxDefaultSize);
    anamorphCheck->SetValue(frame->GetAnamorphicFlag());
    itemSizer7->Add(anamorphCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    // Compose rows
    videoformatSizer->Add(itemSizer5, 0, wxGROW|wxALL, 3);
    videoformatSizer->Add(itemSizer8, 0, wxGROW|wxALL, 3);
    videoformatSizer->Add(itemSizer6, 0, wxGROW|wxALL, 3);
    videoformatSizer->Add(itemSizer61, 0, wxGROW|wxALL, 3);
    videoformatSizer->Add(itemSizer7, 0, wxGROW|wxALL, 3);
    /////////////////////////////////////
    /////////////////////////////////////

    // Compose sizer and adjust
    topSizer->Add(item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 3);
    topSizer->AddSpacer(5);
    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

// Extra settings tab
wxPanel* pyuvFormatDialog::CreateExtraSettingsPage(wxWindow* parent)
{
    // Operating panel
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *item0 = new wxBoxSizer(wxVERTICAL);

    ////////////////////////
    //// File format
    ////////////////////////
    wxStaticBox* fileformatBox = new wxStaticBox(panel, wxID_ANY, wxT("File format"));
    wxBoxSizer* fileformatSizer = new wxStaticBoxSizer(fileformatBox, wxVERTICAL);
    item0->Add(fileformatSizer, 0, wxGROW|wxALL, 3);

    // First row
    wxBoxSizer* itemSizer8 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer8->Add(new wxStaticText(panel, wxID_ANY, wxT("Bits per sample:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer8->Add(3, 3, 1, wxALL, 0);

    bitpersampleText = new wxSpinCtrl(panel, wxID_ANY, wxString::Format(wxT("%d"), frame->GetSampleBits()),
        wxDefaultPosition, wxSize(60, -1), wxTE_LEFT, 1, 16, frame->GetSampleBits());

    itemSizer8->Add(bitpersampleText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Second row
    wxBoxSizer* itemSizer85 = new wxBoxSizer(wxHORIZONTAL);

    signedCheck = new wxCheckBox(panel, wxID_ANY, wxT("Signed format"),
        wxDefaultPosition, wxDefaultSize);
    signedCheck->SetValue(frame->GetSignedFlag());

    itemSizer85->Add(signedCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    itemSizer85->Add(3, 3, 1, wxALL, 0);

    endianCheck = new wxCheckBox(panel, wxID_ANY, wxT("Endianness"),
        wxDefaultPosition, wxDefaultSize);
    endianCheck->SetValue(frame->GetEndiannessFlag());

    itemSizer85->Add(endianCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5);

    // Third row
    wxBoxSizer* itemSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemSizer9->Add(new wxStaticText(panel, wxID_ANY, wxT("Header bytes:")), 0,
        wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer9->Add(3, 3, 1, wxALL, 0);

    headersizeText = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%d"), frame->GetHeaderSize()),
        wxDefaultPosition, wxSize(60, -1));
    itemSizer9->Add(headersizeText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Compose rows
    fileformatSizer->Add(itemSizer8, 0, wxGROW|wxALL, 3);
    fileformatSizer->Add(itemSizer85, 0, wxGROW|wxALL, 3);
    fileformatSizer->Add(itemSizer9, 0, wxGROW|wxALL, 3);
    //////////////////////////////
    //////////////////////////////

    // Compose sizer and adjust
    topSizer->Add(item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 3);
    topSizer->AddSpacer(5);
    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return (panel);
}

// Integer frame rate spin handler
void
pyuvFormatDialog::iratespinHandler(wxSpinEvent& e)
{
    int irate = iframerateSpin->GetValue();
    int frate = fframerateSpin->GetValue();

    frate = 100 * irate + (frate % 100);
    fframerateSpin->SetValue(frate);

    framerateText->SetValue(wxString::Format(wxT("%.2f"), (float) frate / 100.0F));
}

// Change the size choice handler
void
pyuvFormatDialog::sizechoiceHandler(wxCommandEvent& e)
{
    int mysize = e.GetSelection();

    if (pyuvWidths[mysize])
        customwidthText->SetValue(wxString::Format(wxT("%d"), pyuvWidths[mysize]));
    else
        customwidthText->SetValue(wxT(""));

    if (pyuvHeights[mysize])
        customheightText->SetValue(wxString::Format(wxT("%d"), pyuvHeights[mysize]));
    else
        customheightText->SetValue(wxT(""));
}

// Change the size text handler
void
pyuvFormatDialog::sizetextHandler(wxCommandEvent& e)
{
    int mychoice;
    long mywidth, myheight;

    customwidthText->GetValue().ToLong(&mywidth);
    customheightText->GetValue().ToLong(&myheight);
    for (mychoice = 0; mychoice < (int) WXSIZEOF(pyuvResolutions); mychoice++) {
        if ((pyuvWidths[mychoice] == (unsigned int) mywidth) && (pyuvHeights[mychoice] == (unsigned int) myheight)) {
            standardsizeChoice->SetSelection(mychoice);
        };
    };
}

// Fractional frame rate spin handler
void
pyuvFormatDialog::fratespinHandler(wxSpinEvent& e)
{
    int frate = fframerateSpin->GetValue();

    iframerateSpin->SetValue(frate / 100);

    framerateText->SetValue(wxString::Format(wxT("%.2f"), (float) frate / 100.0F));
}

// Text rate change handler
void
pyuvFormatDialog::ratetextHandler(wxCommandEvent& e)
{
    double rate;
    framerateText->GetValue().ToDouble(&rate);

    if (rate < 1.0) {
        rate = 1.0;
        framerateText->SetValue(wxString::Format(wxT("%.2f"), rate));
    };

    if (rate > 60.0) {
        rate = 60.0;
        framerateText->SetValue(wxString::Format(wxT("%.2f"), rate));
    };

    iframerateSpin->SetValue((int) floor(rate));
    fframerateSpin->SetValue((int) floor(rate * 100.0));
}

// Apply button handler
void
pyuvFormatDialog::formatApplyClick(wxCommandEvent& e)
{
    // Apply data format changes
    framerateText->GetValue().ToDouble(&frame->GetRate());
    customwidthText->GetValue().ToULong(&frame->GetWidth());
    frame->GetWidthScreen() = frame->GetWidth() / frame->GetScale();
    customheightText->GetValue().ToULong(&frame->GetHeight());
    frame->GetHeightScreen() = frame->GetHeight() / frame->GetScale();
    headersizeText->GetValue().ToULong(&frame->GetHeaderSize());
    frame->GetColorSpaceNumber() = componentChoice->GetSelection();
    frame->GetSubsamplingNumber() = samplingChoice->GetSelection();
    frame->GetYUVOrder() = orderChoice->GetSelection();
    frame->GetInterleavedFlag() = interleaveCheck->GetValue();
    frame->GetInterlacedFlag() = interlaceCheck->GetValue();
    frame->GetAnamorphicFlag() = anamorphCheck->GetValue();
    frame->GetSampleBits() = bitpersampleText->GetValue();
    frame->GetSignedFlag() = signedCheck->GetValue();
    frame->GetEndiannessFlag() = endianCheck->GetValue();
    gammaText->GetValue().ToDouble(&frame->GetGamma());

    // When size is not well defined
    if (!(frame->GetWidth() * frame->GetHeight())) {
        wxMessageBox(wxT("You have to select a valid size"), wxT("Wait, wait..."), wxOK | wxICON_WARNING, this);
        return;
    }

    // When colorspace is not defined
    if (frame->GetColorSpaceNumber() == CS_UNDEF) {
        wxMessageBox(wxT("You have to select a valid colorspace"), wxT("Wait, wait..."), wxOK | wxICON_WARNING, this);
        return;
    }

    // When subsampling is not defined
    if (frame->GetSubsamplingNumber() == SS_UNDEF) {
        wxMessageBox(wxT("You have to select a valid subsampling"), wxT("Wait, wait..."), wxOK | wxICON_WARNING, this);
        return;
    }

    if (frame->GetFile() && frame->GetFile()->IsOpened()) {

        // File and frame size
        frame->GetFileSize() = frame->GetFile()->Length();
        frame->GetFrameSize() = (unsigned long) ((double) (frame->GetWidth() * frame->GetHeight()) * (*frame->GetFrameDensity())[frame->GetSubsamplingNumber()]);
        frame->GetFrameBytes() = frame->GetFrameSize() * ((frame->GetSampleBits() > 8) ? 2 : 1);
        frame->GetNumFrame() = (unsigned long)floor((double)(frame->GetFileSize() - frame->GetHeaderSize()) / (double)frame->GetFrameBytes());
        frame->GetFrameNumber() = 0;

        // Set up
        wxString info;
        info.Printf(wxT("%d frames, %.0f'%.0f\", %.3f MB"), frame->GetNumFrame(), floorf(((float)frame->GetNumFrame() / (float)frame->GetRate()) / 60.0F), ((float)frame->GetNumFrame() / (float)frame->GetRate()) - 60.0F * floorf(((float)frame->GetNumFrame() / (float)frame->GetRate()) / 60.0F), (float)frame->GetFileSize() / 1048576.0F);
        frame->SetStatusText(info, 1);
        frame->pyuvDispose();
        frame->pyuvPrepare();

        // Do a one-shot timer tick
        wxCommandEvent ev;
        frame->OnStop(ev);

        // Associate the drawing image
        frame->GetImage() = &(frame->drawCanvas->img);

    }

    // create header file
    wxFileName headername(frame->GetFileName());
    headername.SetExt(wxT("hdr"));
    wxTextFile headerfile;
    if (headername.FileExists()) {
        // Open the file
        headerfile.Open(headername.GetFullPath());
    }
    else {
        // Create the file
        headerfile.Create(headername.GetFullPath());            
    }

    // write settings in it
    // width = XXXX
    // height = XXXX
    // rate = XXXX
    // format = XXXX
    // depth = XXXX
    // space = XXXX
    // gamma = XXXX
    headerfile.AddLine(wxString::Format(wxT("width = %d"), frame->GetWidth()));
    headerfile.AddLine(wxString::Format(wxT("height = %d"), frame->GetHeight()));
    headerfile.AddLine(wxString::Format(wxT("rate = %f"), frame->GetRate()));
    headerfile.AddLine(wxString::Format(wxT("format = %s"), (*frame->GetFormatShort())[frame->GetFormat()]));
    headerfile.AddLine(wxString::Format(wxT("depth = %d"), frame->GetSampleBits()));
    headerfile.AddLine(wxString::Format(wxT("space = %s"), (*frame->GetComponents())[frame->GetColorSpaceNumber()]));
    headerfile.AddLine(wxString::Format(wxT("gamma = %f"), frame->GetGamma()));

    // close file
    headerfile.Write();
    headerfile.Close();


    // Check if OK is pushed
    if (e.GetId() == wxID_OK)
        Destroy();
}

// Enable/disable the order choice
void
pyuvFormatDialog::interleavedcheckHandler(wxCommandEvent& e)
{
    /*if (e.IsChecked()) {
        orderChoice->Enable(true);
    } else {
        orderChoice->Enable(false);
    };*/
}

