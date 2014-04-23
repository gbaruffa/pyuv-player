// Name:    playuvApp.cpp
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

// Inclusion of the main header file
#include "playuvApp.h"
#include "converter.h"

// Contexts and bitmaps
static unsigned char *pyuvScreen  = NULL;
static unsigned char *pyuvScreenS = NULL;
static unsigned char *pyuvBuffer  = NULL;

/////////////////////////
// Sequence parameters //
/////////////////////////

// Video resolutions: short names and sizes
static wxString pyuvResolutions[] = {wxT("Choose one"),
									 wxT("QCIF"),
									 wxT("SIF"),
									 wxT("CIF"),
									 wxT("VGA"),
									 wxT("NTSC"),
									 wxT("SD576"),
									 wxT("PAL"),
									 wxT("HD720"),
									 wxT("HD1080"),
									 wxT("DCI2K"),
									 wxT("SHD"),
									 wxT("DCI4K"),
									 wxT("RED"),
									 wxT("UHD"),
									 wxT("Custom")};
static unsigned long pyuvWidths[] =  {0, // choose one
									  176, // qcif
									  352, // sif
									  352, // cif
									  640, // vga
									  700, // ntsc
									  720, // sd576
									  780, // pal
									  1280, // hd720
									  1920, // hd1080
									  2048, // dci2k
									  3840, // shd
									  4096, // dci4k
									  4520, // red
									  7680, // uhd
									  0}; // custom
static unsigned long pyuvHeights[] = {0, // choose one
									  144, // qcif
									  240, // sif
									  288, // cif
									  480, // vga
									  525, // ntsc
									  576, // sd576
									  586, // pal
									  720, // hd720
									  1080, // hd1080
									  1080, // dci2k
									  2160, // shd
									  2160, // dci4k
									  2540, // red
									  4320, // uhd
									  0}; // custom
static unsigned long pyuvWidth = PYUV_WIDTH;
static unsigned long pyuvHeight = PYUV_HEIGHT;
static unsigned long pyuvArea = PYUV_WIDTH * PYUV_HEIGHT;
static bool pyuvAnamorphic = false;
static double pyuvGamma = 1.0;

// Components and color spaces
enum {CS_UNDEF = 0,
	  CS_RGB,
	  CS_XYZ,
	  CS_HSV,
	  CS_YUV,
	  CS_YCBCR,
	  CS_YCBCR709,
	  CS_YIQ,
	  CS_YCOCG,
	  CS_YCXCZ,
	  CS_BW,
	  CS_CUST};
static int pyuvColorspacenumber = CS_UNDEF;
static wxString pyuvComponents[] ={wxT("Choose one"),
								   wxT("RGB"),
								   wxT("XYZ"),
								   wxT("HSV"),
								   wxT("YUV"),
								   wxT("YCbCr"),
								   wxT("YCbCr709"),
								   wxT("YIQ"),
								   wxT("YCoCg"),
								   wxT("YCxCz"),
								   wxT("BW"),
								   wxT("Custom")};
static int pyuvSamplebits = PYUV_DEPTH;
static bool pyuvSigned = PYUV_SIGNED;
static bool pyuvInterleaved = false;
static bool pyuvEndianness = false;
static int pyuvShowcomps = 0;

// Frame rates and number of frame in sequence
static double pyuvRate = PYUV_RATE;
static unsigned long pyuvFramenumber = 0, pyuvNumframes = 0;
static int pyuvFrameTime = (int) floor((1000.0 / PYUV_RATE) + 0.5);
static bool pyuvInterlaced = false;
static bool pyuvDeinterlace = true;

// Subsampling
enum {SS_UNDEF = 0,
	  SS_444,
	  SS_422,
	  SS_420,
	  SS_411,
	  SS_410,
	  SS_400,
	  SS_CUST}; 
static int pyuvSubsamplingnumber = SS_UNDEF;
static wxString pyuvSampling[] ={wxT("Choose one"),
								 wxT("4:4:4"),
								 wxT("4:2:2"),
								 wxT("4:2:0"),
								 wxT("4:1:1"),
								 wxT("4:1:0"),
								 wxT("4:0:0"),
								 wxT("Custom")};
static double pyuvFramedensity[] = {0.0,
									3.0,
									2.0,
									1.5,
									1.5,
									1.25,
									1.0,
									0.0};

// Packed order
enum {OR_DEFAULT,
	  OR_UYVY,
	  OR_YUY2,
	  OR_YVYU,
	  OR_BGR,
      OR_CUST};
static int pyuvOrder = OR_DEFAULT;
static wxString pyuvFourcc[] = {wxT("Default"),
								wxT("UYVY"),
								wxT("YUY2"),
								wxT("YVYU"),
								wxT("BGR"),
								wxT("Custom")};

// Format shortcut
static wxString pyuvFormatShort[] = {wxT("P444"),
									 wxT("P422"),
									 wxT("P420"),
									 wxT("P411"),
									 wxT("P410"),
									 wxT("P400"),
									 wxT("I444"),
									 wxT("I422"),
									 wxT("I420"),
									 wxT("I411"),
									 wxT("I410"),
									 wxT("UYVY"),
									 wxT("YUY2"),
									 wxT("YVYU"),
									 wxT("IYUV"),
									 wxT("UNK")};
/*static int pyuvFormatColor[] = {CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_BW,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_YCBCR,
								CS_UNDEF};*/
static int pyuvFormatSampling[] = {SS_444,
								   SS_422,
								   SS_420,
								   SS_411,
								   SS_410,
								   SS_400,
								   SS_444,
								   SS_422,
								   SS_420,
								   SS_411,
								   SS_410,
								   SS_422,
								   SS_422,
								   SS_422,
								   SS_420,
								   SS_UNDEF};
static bool pyuvFormatInterleave[] = {false,
									  false,
									  false,
									  false,
									  false,
									  false,
									  true,
									  true,
									  true,
									  true,
									  true,
									  true,
									  true,
									  true,
									  false,
									  false};
static int pyuvFormatOrder[] = {OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_DEFAULT,
								OR_UYVY,
								OR_YUY2,
								OR_YVYU,
								OR_DEFAULT,
								OR_DEFAULT};

static int pyuvFormat = PYUV_FORMAT;

// File related items
static wxString pyuvFilename = wxT("");
static wxFile *pyuvFile = NULL;
static wxFileOffset pyuvFilesize;
static unsigned long pyuvFramesize;
static unsigned long pyuvFramebytes;
static unsigned long pyuvHeadersize = 0;

// Visualization
static int pyuvScale = 1;
static unsigned long pyuvWidthS = pyuvWidth / pyuvScale;
static unsigned long pyuvHeightS = pyuvHeight / pyuvScale;
static wxColour pyuvBackColor = wxColour(134U, 0U, 0U);
static bool pyuvDoublebuffer = false;
static int pyuvEngine = 0;

// Timecode and identity boxes
static wxString pyuvCurtime = wxT("0000: 00'00\"00");
static wxCoord tcwidth = 0, tcheight = 0, idwidth = 0, idheight = 0, tcx, tcy, idx, idy;
static wxFont pyuvTimeFont = wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
static wxFont pyuvIdFont = wxFont(16, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
static wxColour pyuvTimeColour = wxColour(0x00, 0x00, 0x00), pyuvIdColour = wxColour(0xFF, 0xFF, 0xFF);
static wxRect pyuvTimeBack, pyuvIdBack;

// Other things
static bool pyuvPlay = false, pyuvShowtimecode = false, pyuvShowidentity = false, pyuvLoop = true;
static wxImage *pyuvImg;
static wxString pyuvTopbasetitle = wxT(PYUV_SHORTNAME);
static pyuvFrame *frame;
static bool pyuvHelpsystem = false;
static unsigned int pyuvSliderupdate;

// Synchronization
static int pyuvServno = 0;
static wxString pyuvServnostring = wxString::Format(wxT("%02d"), pyuvServno);
static int pyuvMyMaster = -1;
static bool pyuvMySlave[MAX_NO_SERVERS], pyuvIHaveASlave = false;
static wxColour pyuvSlaveColors[MAX_NO_SERVERS];

/////////////////////////////////////////////////////
//// Application methods
/////////////////////////////////////////////////////
IMPLEMENT_APP(pyuvApp)

// Description of possible command line parameters
static const wxCmdLineEntryDesc pyuvCommandLine[] = {
	{wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters" },
	{wxCMD_LINE_SWITCH, "v", "version", "print version" },
	{wxCMD_LINE_OPTION, "d", "debug", "specify a debug level" },
	{wxCMD_LINE_PARAM, NULL, NULL, "input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_NONE}
};

// Initialize the application
bool pyuvApp::OnInit()
{
	int res;

	// Set the English locale
	wxLocale(wxLANGUAGE_ENGLISH, wxLOCALE_LOAD_DEFAULT);

	// Look for existing clients and set up an IPC server
	{
		pyuvClient* tmp_client = new pyuvClient;
		wxConnectionBase* connection = NULL;		

		for (pyuvServno = 0; pyuvServno < MAX_NO_SERVERS; pyuvServno++) {
			
			wxLogNull noLog;

			if (!(connection = tmp_client->MakeConnection(wxEmptyString, 
														  wxString::Format(wxT("/tmp/PYUV%02d"), pyuvServno),
														  wxT("intro"))))
				break;
			else {
				connection->Disconnect();
				connection = NULL;
			}
		}
		delete connection;
		delete tmp_client;
	}
	if (pyuvServno < MAX_NO_SERVERS) {
		m_server = new pyuvServer;
		pyuvServnostring = wxString::Format(wxT("%02d"), pyuvServno);
		if (!m_server->Create(wxString::Format(wxT("/tmp/PYUV%02d"), pyuvServno)))
			wxLogDebug(wxT("Failed to create an IPC service."));
	} else
		wxLogDebug(wxT("Increase number of IPC servers."));

	// Init colors for slaves background
	int i = 0;
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(*wxRED);
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(*wxBLUE);
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(*wxGREEN);
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(*wxCYAN);
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(wxString(wxT("ORANGE")));
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(wxString(wxT("SKY BLUE")));
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(wxString(wxT("GOLD")));
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(wxString(wxT("MAGENTA")));
	if (i < MAX_NO_SERVERS) pyuvSlaveColors[i++] = wxColour(wxString(wxT("AQUAMARINE")));
	for (; i < MAX_NO_SERVERS; i++)
		pyuvSlaveColors[i] = wxColour((unsigned char) (i * 18 + 31),
									  (unsigned char) (i * 19 + 100),
									  (unsigned char) (i * 13 + 200));

	// Create the main application window
	frame = new pyuvFrame(pyuvTopbasetitle + wxT(" - a portable YUV player"));

	// Show it
	frame->Show(true);
    SetTopWindow(frame);

	// Parse command line
	wxCmdLineParser cmdParser(pyuvCommandLine, argc, argv);

	{
		// suppress disturbing warnings
		wxLogNull log;

		// Pass false to suppress auto Usage() message
		res = cmdParser.Parse(false);
	}

	// Check for a sequence filename
	if (cmdParser.GetParamCount() > 0) {

		pyuvFilename = cmdParser.GetParam(0);

		// Under Windows when invoking via a document
		// in Explorer, we are passed the short form.
		// So normalize and make the long form.
		wxFileName fName(pyuvFilename);

		fName.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
		pyuvFilename = fName.GetFullPath();

		// open the passed file
		frame->openfile(wxFileName(pyuvFilename, wxPATH_NATIVE));

	};

	// Add handler
	wxImage::AddHandler( new wxBMPHandler );

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
BEGIN_EVENT_TABLE(pyuvCanvas, wxWindow)
	EVT_MOTION(pyuvCanvas::OnMotion)
	EVT_PAINT(pyuvCanvas::OnPaint)
	EVT_DROP_FILES(pyuvCanvas::OnDrop)
END_EVENT_TABLE()

// Canvas constructor
pyuvCanvas::pyuvCanvas(pyuvFrame *parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(PYUV_MINWIDTH, PYUV_MINHEIGHT), wxFULL_REPAINT_ON_RESIZE)
{
	// Create the logo img
	logoimg.~wxImage();
	logoimg = wxImage(playuv250x200);

	// Create the drawing bitmap
	pyuvBmp      = new wxBitmap(logoimg);
    pyuvBmpdata  = new PixelData(*pyuvBmp);
    pyuvPixel    = new PixelData::Iterator(*pyuvBmpdata);
    pyuvRowstart = new PixelData::Iterator(*pyuvBmpdata);
}

// Motion handler
void pyuvCanvas::OnMotion(wxMouseEvent& event)
{
	if (event.Dragging()) {
		wxClientDC dc(this);

		wxPen pen(*wxRED, 1, wxSOLID); // red pen of width 1
		dc.SetPen(pen);
		dc.DrawPoint(event.GetPosition());
	}
}

// Paint Handler
void pyuvCanvas::OnPaint(wxPaintEvent& event)
{
	// Create a bitmap from our pixel data
    //wxBitmap bmp(wxImage(PYUV_WIDTH, PYUV_HEIGHT, (unsigned char *) screen, true));
	//wxBitmap bmp(logoimg);
	//wxBitmap bmp(*pyuvImg);

	// See this: http://wxforum.shadonet.com/viewtopic.php?t=10654
	// THIS IS WORKING FINE
	/*wxPaintDC dc(this);
    dc.DrawBitmap(wxBitmap(*pyuvImg), 0, 0, true);*/

	// look here : http://wxforum.shadonet.com/viewtopic.php?t=9393
	/*wxBitmap b = wxBitmap(30, 20, -1);
	unsigned int *data = b.GetRawData();
    dc.DrawBitmap(b, 0, 0, true);*/

	/*
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
	*/

	if (pyuvDoublebuffer) {

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
			dc.DrawText(pyuvServnostring, idx, idy);
		};
#endif
	} else {

		// Paint the screen with single buffering
		wxPaintDC dc(this);

		if (pyuvEngine == 1)
			// raw bitmap
			dc.DrawBitmap(*pyuvBmp, 0, 0, true);
		else if (pyuvEngine == 0)
			// normal bitmap
			dc.DrawBitmap(wxBitmap(*pyuvImg), 0, 0, true);

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
			dc.DrawText(pyuvServnostring, idx, idy);
		};

	}

}

// Drag and drop handler
void pyuvCanvas::OnDrop(wxDropFilesEvent& event)
{

	pyuvFrame *parent = (pyuvFrame *) GetParent();	
	parent->openfile(wxFileName(event.m_files[0]));

}

////////////////////////////////////////////////////////////
//// Main frame (window) methods
////////////////////////////////////////////////////////////

// Event table for pyuvFrame
BEGIN_EVENT_TABLE(pyuvFrame, wxFrame)

	// File menu
	EVT_MENU(wxID_OPEN,        pyuvFrame::OnOpen)    // Open menu
	EVT_MENU(wxID_CLOSE,       pyuvFrame::OnClose)   // Close menu
	EVT_MENU(wxID_SAVEAS,      pyuvFrame::OnSaveAs)  // Save As menu
	EVT_MENU(Menu_File_Format, pyuvFrame::OnFormat)  // Format menu
	EVT_MENU(wxID_EXIT,        pyuvFrame::OnQuit)    // Quit menu

	// Control menu
	EVT_MENU(Menu_Control_Play,    pyuvFrame::OnPlay)    // Play menu
	EVT_MENU(Menu_Control_Stop,    pyuvFrame::OnStop)    // Stop menu
	EVT_MENU(Menu_Control_Rewind,  pyuvFrame::OnRewind)  // Rewind menu
	EVT_MENU(Menu_Control_Forward, pyuvFrame::OnForward) // Forward menu
	EVT_MENU(Menu_Control_Goto,    pyuvFrame::OnGoto)    // Goto menu
	EVT_MENU(Menu_Control_Loop,    pyuvFrame::OnLoop)    // Loop menu
	EVT_MENU(Menu_Control_Sync,    pyuvFrame::OnSync)    // Sync menu

	// View menu
	EVT_MENU(Menu_View_Timecode,    pyuvFrame::OnTimecode)    // Time code menu
	EVT_MENU(Menu_View_Deinterlace, pyuvFrame::OnDeinterlace) // Deinterlace menu
	EVT_MENU(Menu_View_Identity,    pyuvFrame::OnIdentity)    // Identity menu
	EVT_MENU(Menu_View_Zoom_200,    pyuvFrame::OnScale)       // Change of scale
	EVT_MENU(Menu_View_Zoom_100,    pyuvFrame::OnScale)       // Change of scale
	EVT_MENU(Menu_View_Zoom_50,     pyuvFrame::OnScale)       // Change of scale
	EVT_MENU(Menu_View_Zoom_33,     pyuvFrame::OnScale)       // Change of scale
	EVT_MENU(Menu_View_Zoom_25,     pyuvFrame::OnScale)       // Change of scale

	EVT_MENU(Menu_View_Comp_All,   pyuvFrame::OnComp)    // Change of component
	EVT_MENU(Menu_View_Comp_Next,  pyuvFrame::OnComp)    // Change of component

	EVT_MENU(wxID_COPY,  pyuvFrame::OnCopy)    // Copy to clipboard

	// Settings menu
	EVT_MENU(Menu_Settings_Draw_Bitmap, pyuvFrame::OnEngine) // Switch engine
	EVT_MENU(Menu_Settings_Draw_Raw,    pyuvFrame::OnEngine) // Switch engine
	EVT_MENU(Menu_Settings_Draw_OGL,    pyuvFrame::OnEngine) // Switch engine
	EVT_MENU(Menu_Settings_Draw_Double, pyuvFrame::OnDouble) // Switch to double buffering

	// Help menu
	EVT_MENU(wxID_ABOUT,         pyuvFrame::OnAbout)   // About menu
	EVT_MENU(Menu_Help_Contents, pyuvFrame::OnContents)   // Contents menu

	// Link timer ticks to its handler
	EVT_TIMER(ANIMATE_TIMER_ID, pyuvFrame::OnAnimate)

	//EVT_SIZE(pyuvFrame::OnResize)

	// Slider events
	EVT_COMMAND_SCROLL(SLIDER_ID, pyuvFrame::OnScroll)

	// Button events
	EVT_BUTTON(PlayButton_id, pyuvFrame::OnPlay) 
	EVT_BUTTON(StopButton_id, pyuvFrame::OnStop) 

END_EVENT_TABLE()

// Frame ctor
pyuvFrame::pyuvFrame(const wxString& title)
:wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1),
	wxSize(PYUV_WIDTH, PYUV_HEIGHT), wxMINIMIZE_BOX |
	wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN),
	animTimer(this, ANIMATE_TIMER_ID),
	help(wxHF_DEFAULT_STYLE | wxHF_OPEN_FILES)
{
	// Set the frame icon
	SetIcon(wxIcon(playuv16));

	// Minimum size, by default
	SetMinSize(wxSize(PYUV_MINWIDTH, PYUV_MINHEIGHT));

	// Create a file menu bar
	wxMenu *fileMenu = new wxMenu;

	// Append file menus
	fileMenu->Append(wxID_OPEN,  wxT("O&pen\tCtrl+O"), wxT("Open a file"));
	fileMenu->Append(wxID_CLOSE, wxT("C&lose\tCtrl+C"), wxT("Close the file"));
	fileMenu->Append(wxID_SAVEAS, wxT("S&ave frame\tCtrl+S"), wxT("Save the frame"));
	fileMenu->AppendSeparator();
	fileMenu->Append(Menu_File_Format, wxT("&Format\tCtrl+F"), wxT("Select the video format"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT,  wxT("E&xit\tCtrl+Q"), wxT("Quit this program"));

	// Create a control menu bar
	wxMenu *controlMenu = new wxMenu;

	// Append control menus
	pyuvPlay = false;
	controlMenu->Append(Menu_Control_Play, wxT("&Play\tSpace"), wxT("Play/pause a sequence"));
	controlMenu->Enable(Menu_Control_Play, false);
	controlMenu->Append(Menu_Control_Stop, wxT("&Stop\tEsc"), wxT("Stop a sequence"));
	controlMenu->Enable(Menu_Control_Stop, false);
	controlMenu->Append(Menu_Control_Rewind, wxT("&Backward\tLeft"), wxT("Backward a frame"));
	controlMenu->Enable(Menu_Control_Rewind, false);
	controlMenu->Append(Menu_Control_Forward, wxT("&Forward\tRight"), wxT("Forward a frame"));
	controlMenu->Enable(Menu_Control_Forward, false);
	controlMenu->Append(Menu_Control_Goto, wxT("&Goto\tCtrl+G"), wxT("Go to frame")); 
	controlMenu->Enable(Menu_Control_Goto, false);
	controlMenu->AppendSeparator();
	controlMenu->AppendCheckItem(Menu_Control_Loop, wxT("&Loop\tCtrl+Y"), wxT("Loop continuously"));
	pyuvLoop = true;
	controlMenu->Check(Menu_Control_Loop, true);
	controlMenu->Enable(Menu_Control_Loop, false);

	// Add a sync menu
	controlMenu->Append(Menu_Control_Sync, wxT("&Sync\tCtrl+K"), wxT("Sync commands between PYUVs"));

	// Create a view menu bar
	wxMenu *viewMenu = new wxMenu;

	// Create a zoom submenu
	wxMenu *zoomMenu = new wxMenu;

	// Append zoom menus
	zoomMenu->AppendRadioItem(Menu_View_Zoom_200, wxT("2&00%\tCtrl+D"), wxT("Double size"));
	zoomMenu->Enable(Menu_View_Zoom_200, true);
	zoomMenu->AppendRadioItem(Menu_View_Zoom_100, wxT("&100%\tCtrl+1"), wxT("Original size"));
	zoomMenu->Check(Menu_View_Zoom_100, true);
	zoomMenu->AppendRadioItem(Menu_View_Zoom_50, wxT("&50%\tCtrl+2"), wxT("Half size"));
	zoomMenu->Enable(Menu_View_Zoom_50, true);
	zoomMenu->AppendRadioItem(Menu_View_Zoom_33, wxT("&33%\tCtrl+3"), wxT("Third size"));
	zoomMenu->Enable(Menu_View_Zoom_33, true);
	zoomMenu->AppendRadioItem(Menu_View_Zoom_25, wxT("&25%\tCtrl+4"), wxT("Quarter size"));
	zoomMenu->Enable(Menu_View_Zoom_25, true);

	// Append them to View
	viewMenu->Append(Menu_View_Zoom, wxT("Zoom"), zoomMenu);

	// Create a component submenu
	wxMenu *compMenu = new wxMenu;

	// Append component menus
	compMenu->Append(Menu_View_Comp_All, wxT("&All\tA"), wxT("Show all components"));
	compMenu->Append(Menu_View_Comp_Next, wxT("&Next\tN"), wxT("Show next component"));

	// Append them to View
	viewMenu->Append(Menu_View_Comp, wxT("Components"), compMenu);

	// Add other menus to View
	viewMenu->AppendCheckItem(Menu_View_Deinterlace, wxT("&Deinterlace\tD"), wxT("Deinterlace"));
	pyuvDeinterlace = false;
	viewMenu->Check(Menu_View_Deinterlace, false);

	viewMenu->AppendCheckItem(Menu_View_Timecode, wxT("&Time code\tT"), wxT("Show the timecode"));
	pyuvShowtimecode = false;
	viewMenu->Check(Menu_View_Timecode, false);

	viewMenu->AppendCheckItem(Menu_View_Identity, wxT("&Identity\tI"), wxT("Show identity"));
	pyuvShowidentity = false;
	viewMenu->Check(Menu_View_Identity, false);

	viewMenu->AppendSeparator();
	viewMenu->Append(wxID_COPY, wxT("Cop&y frame\tF8"), wxT("Copy the frame to clipboard"));

	// Create a settings menu bar
	wxMenu *settingsMenu = new wxMenu;

	// Append drawing engine selection
	wxMenu *drawMenu = new wxMenu;

	// Drawing engines
	drawMenu->AppendRadioItem(Menu_Settings_Draw_Bitmap, wxT("&Bitmap\tCtrl+B"), wxT("Draw with bitmaps"));
	if (pyuvEngine == 0)
		drawMenu->Check(Menu_Settings_Draw_Bitmap, true);
	drawMenu->AppendRadioItem(Menu_Settings_Draw_Raw, wxT("&Raw bitmap\tCtrl+R"), wxT("Draw with raw bitmaps"));
	drawMenu->Enable(Menu_Settings_Draw_Raw, false);
	if (pyuvEngine == 1)
		drawMenu->Check(Menu_Settings_Draw_Raw, true);
	drawMenu->AppendRadioItem(Menu_Settings_Draw_OGL, wxT("&OpenGL\tCtrl+L"), wxT("Draw with OpenGL"));
	drawMenu->Enable(Menu_Settings_Draw_OGL, false);
	if (pyuvEngine == 2)
		drawMenu->Check(Menu_Settings_Draw_OGL, true);
	drawMenu->AppendSeparator();
	drawMenu->AppendCheckItem(Menu_Settings_Draw_Double, wxT("&Double buffer\tCtrl+F"), wxT("Use double buffering"));
	if (pyuvDoublebuffer)
		drawMenu->Check(Menu_Settings_Draw_Double, true);
	else
		drawMenu->Check(Menu_Settings_Draw_Double, false);
	drawMenu->Enable(Menu_Settings_Draw_Double, false);


	// Append them to Settings
	settingsMenu->Append(Menu_Settings_Draw, wxT("Draw engine"), drawMenu);

	// Create a help menu bar
	wxMenu *helpMenu = new wxMenu;

	// Activate the "Contents" help system
	help.UseConfig(wxConfig::Get());
	wxFileSystem::AddHandler(new wxZipFSHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxImage::AddHandler(new wxPNGHandler);
	if (help.AddBook(wxFileName(wxT(HELP_FILENAME), wxPATH_UNIX)) || help.AddBook(wxFileName(wxT("/usr/share/doc/pyuv/doc.htb"), wxPATH_UNIX))) {
		helpMenu->Append(Menu_Help_Contents, wxT("&Contents\tCtrl+H"), wxT("Show program help"));
		pyuvHelpsystem = true;
	} else {
		helpMenu->Append(Menu_Help_Contents, wxT("&Contents\tCtrl+H"), wxT(HELP_FILENAME) wxT(" not found!!!"));
		helpMenu->Enable(Menu_Help_Contents, false);
		pyuvHelpsystem = false;
	}

	// The "About" item should be in the help menu
	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

	// Now append the freshly created menus to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(controlMenu, wxT("&Control"));
	menuBar->Append(viewMenu, wxT("&View"));
	menuBar->Append(settingsMenu, wxT("&Settings"));
	menuBar->Append(helpMenu, wxT("&Help"));

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// Create a status bar
	CreateStatusBar(2);
	int stwid[] = {120, -1};
	SetStatusWidths(2, stwid);
	SetStatusText(wxT("Welcome to PYUV!"), 0);

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

// File menu handlers
// Open menu handler
void pyuvFrame::OnOpen(wxCommandEvent& event)
{
	// Prepare file open dialog
	wxFileDialog dialog(
        this,  // parent
        wxT("Open"), // title of the dialog
        wxT(""), // starting directory
        wxT(""), // starting filename
#ifdef __WXMOTIF__
        wxT("PYUV files (*.*)|*.*") // only 1 choice for MOTIF
#else
        wxT("PYUV files (*.yuv;*.raw;*.rgb;*.xyz;*.hsv;*.yiq;*.bw;*.cif;*.sif;*.qcif;*.vix)|")
			wxT("*.yuv;*.raw;*.rgb;*.xyz;*.hsv;*.yiq;*.bw;*.cif;*.sif;*.qcif;*.vix|All files (*.*)|*.*")
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
        wxT("Save frame as BMP"), // title of the dialog
        wxT(""), // starting directory
		wxString::Format(wxT("frame%06d.bmp"), pyuvFramenumber), // starting filename
        wxT("BMP files (*.bmp)|*.bmp|All files (*.*)|*.*"), 
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	// Show the dialog
    if (dialog.ShowModal() == wxID_OK) {

		// Debug dialog: disposable
        wxString info;
        info.Printf(wxT("Full file name: %s\n")
            wxT("Path: %s\n")
            wxT("Name: %s\n"),
            dialog.GetPath().c_str(),
            dialog.GetDirectory().c_str(),
            dialog.GetFilename().c_str());
        wxMessageDialog dialog2(this, info, wxT("Selected file"));
        //dialog2.ShowModal();

		wxImage myImg;
		wxBitmap myBmp(*pyuvImg);
		myImg = myBmp.ConvertToImage();

		// Save the image
		if (myImg.IsOk()) {
			if (!myImg.SaveFile(dialog.GetPath(), (wxBitmapType) wxBITMAP_TYPE_BMP)) {
				wxMessageDialog dialog2(this, wxT("The frame could not be saved"), wxT("File not saved"));
				dialog2.ShowModal();
			};
		} else {
			wxMessageDialog dialog2(this, wxT("The frame could not really be saved"), wxT("File not saved"));
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
		wxMessageBox(wxT("File could not be opened: ") + pyuvFilename, wxT("PYUV Error"), wxOK, this);
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
		wxMessageDialog dialog3(this, wxT("BAD REGEX!!!"), wxT(""));
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
		wxMessageDialog dialog3(this, wxT("BAD REGEX!!!"), wxT(""));
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
		wxMessageDialog dialog3(this, wxT("BAD REGEX!!!"), wxT(""));
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
		wxMessageDialog dialog3(this, wxT("BAD REGEX!!!"), wxT(""));
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
		info.Printf(wxT("%d frames, %.0f'%.0f\", %.3f MB"),
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
	GetMenuBar()->SetLabel(Menu_Control_Play, wxT("P&lay\tSpace"));
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
	SetTitle(pyuvTopbasetitle + wxT(" - a portable YUV player"));

	// Abandon your master
	pyuvMyMaster = -1;
	SetOwnBackgroundColour(pyuvBackColor);
	pyuvIHaveASlave = false;
	int i;
	for (i = 0; i < MAX_NO_SERVERS; i++)
		pyuvMySlave[i] = false;

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
	if (pyuvIHaveASlave && (pyuvMyMaster == -1)) {
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
		animTimer.Start(pyuvFrameTime);
		pyuvPlay = true;
		mbar->SetLabel(Menu_Control_Play, wxT("P&ause\tSpace"));
		playButton->SetBitmapLabel(wxBitmap(pause16));
		mbar->Enable(Menu_Control_Stop, true);
		stopButton->Enable(true);
		mbar->Enable(Menu_Control_Rewind, true);
		mbar->Enable(Menu_Control_Forward, true);

	} else {

		// Go to Pause mode
		animTimer.Stop();
		pyuvPlay = false;
		mbar->SetLabel(Menu_Control_Play, wxT("P&lay\tSpace"));
		playButton->SetBitmapLabel(wxBitmap(play16));

	};

	// Send the command to your slave(s)
	if (pyuvIHaveASlave && (pyuvMyMaster == -1)) {
		
		sendcommand((char *) "play");
	};
}

// Stop menu handler
void pyuvFrame::OnStop(wxCommandEvent& event)
{
	wxMenuBar *mbar = GetMenuBar();

	// Stop the timer and rewind the file
	animTimer.Stop();
	if (pyuvFile && pyuvFile->IsOpened())
		pyuvFile->Seek(pyuvHeadersize, wxFromStart);

	// Not playing now
	pyuvPlay = false;
	mbar->SetLabel(Menu_Control_Play, wxT("P&lay\tSpace"));
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

	// Move the slider from time to time
	if (!(pyuvFramenumber % pyuvSliderupdate))
		frameSlider->SetValue(pyuvFramenumber);

	// Send the command to your slave(s)
	if (pyuvIHaveASlave && (pyuvMyMaster == -1)) {
		
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
	mbar->SetLabel(Menu_Control_Play, wxT("P&lay\tSpace"));
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
	mbar->SetLabel(Menu_Control_Play, wxT("P&lay\tSpace"));
	playButton->SetBitmapLabel(wxBitmap(play16));

	// Send the command to your slave(s)
	if (pyuvIHaveASlave && (pyuvMyMaster == -1)) {
		
		sendcommand((char *)"forward");
	};
}

// Goto menu handler
void pyuvFrame::OnGoto(wxCommandEvent& event)
{
	pyuvFramenumber = (long unsigned int) wxGetNumberFromUser(wxString::Format(wxT("You can directly go to a frame\nin the video sequence\n(select a number between 0 and %d)"), pyuvNumframes - 1),
								wxT("Jump to frame:"), // prompt text
								wxT("Frame selection dialog"), // title test
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
	pyuvClient* client = new pyuvClient;
	wxConnectionBase* connection = NULL;
	int servno;

	for (servno = 0; servno < MAX_NO_SERVERS; servno++) {

		// Disable screen warnings of MakeConnection()
		wxLogNull noLog;

		// Skip yourself and nonselected ones
		if ((servno == pyuvServno) || !pyuvMySlave[servno])
			continue;

		// Look for available server
		if ((connection = client->MakeConnection(wxT("localhost"), 
							wxString::Format(wxT("/tmp/PYUV%02d"), servno),
							wxT("dothis")))) {
			// Ask the other instance, booking in case
			if (!connection->Execute(wxString::FromAscii(cmd)))
				/*pyuvMySlave[servno] = false*/;
			connection->Disconnect();

		} else {

			// no more connected
			pyuvMySlave[servno] = false;
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
		wxMessageBox(wxT("Scale is unknown: assuming 100%"),
				 wxT("Scaling"),
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
		wxMessageBox(wxT("The file '") wxT(HELP_FILENAME) wxT("' is missing in\n")
			wxT("the program directory:\n")
			wxT("the help system is not available"));
	else
		help.Display(wxT("doc.htm"));
}

// About menu handler
void pyuvFrame::OnAbout(wxCommandEvent& event)
{
	wxString msg;

	msg.Printf(
		wxT("PYUV - A portable and multiplatform RAW sequence player\n")
		wxT("Version %d.%d %s - ")
#ifdef __WXMSW__
		wxT("for Windows\n")
#endif
#ifdef __WXGTK__
		wxT("for Linux\n")
#endif
		wxT("(%s)\n\n")
		wxT("Official site:\n%s\n")
		wxT("\nCreated with %s\n")
		wxT("\n(c) 2006-2013, Giuseppe Baruffa, DSPLab"),
		PYUV_VERSION, PYUV_SUBVERSION, wxT(PYUV_BETA),
		PYUV_BTIME,
		wxT(PYUV_WEBSITE),
		wxVERSION_STRING);

	// Display a message box with the info
	wxMessageBox(msg, wxT("About PYUV"), wxOK | wxICON_INFORMATION, this);
}

// Animation timer handler
bool pyuvFrame::pyuvDraw(void)
{
	int n, area = pyuvWidth * pyuvHeight * 3;

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

			pyuvCurtime.Printf(wxT("%04u: %02.0f'%02.0f\"%02.0f"),
				pyuvFramenumber,
				floorf(tot_min),
				floorf(tot_sec),
				tot_cent);
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

	// Sets the animation timer with proper interval
	pyuvFrameTime = (int) floor((1000.0 / pyuvRate) + 0.5);

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
	pyuvTimeBack = wxRect(tcx - 2, tcy + 1, tcwidth + 4, tcheight - 2);

	// Adjust the settings for the identity show
	dc.SetFont(pyuvIdFont);
	dc.GetTextExtent(wxString::Format(wxT("%02d"), pyuvServno), &idwidth, &idheight);
	idx = pyuvWidthS - 2 - idwidth;
	idy = 3;
	pyuvIdBack = wxRect(idx - 2, idy + 0, idwidth + 2, idheight - 2);

	// Readjust minimum width
	frameSizer->Layout();
    frameSizer->SetSizeHints(this);
    frameSizer->Fit(this);
}

// Dispose old sequence
void pyuvFrame::pyuvDispose(void)
{
	// Stop the animation, if any
	wxCommandEvent e;
	OnStop(e);
}

/////////////////////////////////////////////////////////
// Video format selection dialog methods
/////////////////////////////////////////////////////////
IMPLEMENT_CLASS(pyuvFormatDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(pyuvFormatDialog, wxPropertySheetDialog)
	EVT_SPIN(ID_INTRATESPIN,      pyuvFormatDialog::iratespinHandler)
	EVT_SPIN(ID_FRACRATESPIN,     pyuvFormatDialog::fratespinHandler)
	EVT_TEXT_ENTER(ID_RATETEXT,   pyuvFormatDialog::ratetextHandler)
	EVT_BUTTON(wxID_OK,           pyuvFormatDialog::formatApplyClick)
	EVT_BUTTON(wxID_APPLY,        pyuvFormatDialog::formatApplyClick)
	EVT_CHOICE(ID_SIZECHOICE,     pyuvFormatDialog::sizechoiceHandler)
	EVT_TEXT_ENTER(ID_WIDTHTEXT,  pyuvFormatDialog::sizetextHandler)
	EVT_TEXT_ENTER(ID_HEIGHTTEXT, pyuvFormatDialog::sizetextHandler)
	EVT_CHECKBOX(ID_INTERLEAVED,  pyuvFormatDialog::interleavedcheckHandler)
END_EVENT_TABLE()

// Ctor
pyuvFormatDialog::pyuvFormatDialog(wxWindow* win)
{
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
		if ((pyuvWidths[i] == pyuvWidth) && (pyuvHeights[i] == pyuvHeight))
			standardsizeChoice->SetSelection(i);

    itemSizer2->Add(standardsizeChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

	// Second row
    wxBoxSizer* itemSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemSizer3->Add(new wxStaticText(panel, wxID_ANY, wxT("Size (WxH):")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer3->Add(3, 3, 1, wxALL, 0);

	customwidthText = new wxTextCtrl(panel, ID_WIDTHTEXT, wxString::Format(wxT("%d"), pyuvWidth),
		wxDefaultPosition, wxSize(60, -1), wxTE_RIGHT);
    itemSizer3->Add(customwidthText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer3->Add(new wxStaticText(panel, wxID_ANY, wxT("x")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 1);

	customheightText = new wxTextCtrl(panel, ID_HEIGHTTEXT, wxString::Format(wxT("%d"), pyuvHeight),
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

	framerateText = new wxTextCtrl(panel, ID_RATETEXT, wxString::Format(wxT("%.2f"), pyuvRate),
		wxDefaultPosition, wxSize(60, -1), wxTE_LEFT);
    itemSizer4->Add(framerateText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

	iframerateSpin = new wxSpinButton(panel, ID_INTRATESPIN, wxDefaultPosition,
		wxSize(-1, framerateText->GetSize().GetHeight()));
	iframerateSpin->SetRange(1, 60);
	iframerateSpin->SetValue((int) floorf((float) pyuvRate));
    itemSizer4->Add(iframerateSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer4->Add(new wxStaticText(panel, wxID_ANY, wxT(".")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 1);

	fframerateSpin = new wxSpinButton(panel, ID_FRACRATESPIN, wxDefaultPosition,
		wxSize(-1, framerateText->GetSize().GetHeight()));
	fframerateSpin->SetRange(100, 6000);
	fframerateSpin->SetValue((int) floorf(100.0F * (float) pyuvRate));
    itemSizer4->Add(fframerateSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Second row
    wxBoxSizer* itemSizer41 = new wxBoxSizer(wxHORIZONTAL);

    interlaceCheck = new wxCheckBox(panel, wxID_ANY, wxT("Interlaced"),
		wxDefaultPosition, wxDefaultSize);
	interlaceCheck->SetValue(pyuvInterlaced);
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

    wxArrayString componentChoices(WXSIZEOF(pyuvComponents), pyuvComponents);
    componentChoice = new wxChoice(panel, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, componentChoices);
	componentChoice->SetSelection(pyuvColorspacenumber);

    itemSizer5->Add(componentChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

	// second row
    wxBoxSizer* itemSizer8 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer8->Add(new wxStaticText(panel, wxID_ANY, wxT("Gamma:")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer8->Add(3, 3, 1, wxALL, 0);

	gammaText = new wxTextCtrl(panel, ID_GAMMATEXT, wxString::Format(wxT("%.3f"), pyuvGamma),
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
	samplingChoice->SetSelection(pyuvSubsamplingnumber);

    itemSizer6->Add(samplingChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Fourth row
    wxBoxSizer* itemSizer61 = new wxBoxSizer(wxHORIZONTAL);

    itemSizer61->Add(new wxStaticText(panel, wxID_ANY, wxT("Ordering:")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer61->Add(3, 3, 1, wxALL, 0);

    wxArrayString orderChoices(WXSIZEOF(pyuvFourcc), pyuvFourcc);
    orderChoice = new wxChoice(panel, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, orderChoices);
	orderChoice->SetSelection(pyuvOrder);

    itemSizer61->Add(orderChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

    // Fifth row
    wxBoxSizer* itemSizer7 = new wxBoxSizer(wxHORIZONTAL);

    interleaveCheck = new wxCheckBox(panel, ID_INTERLEAVED, wxT("Interleaved"),
		wxDefaultPosition, wxDefaultSize);
	interleaveCheck->SetValue(pyuvInterleaved);
    itemSizer7->Add(interleaveCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	/*if (interleaveCheck->IsChecked())
		orderChoice->Enable(true);
	else
		orderChoice->Enable(false);*/

    anamorphCheck = new wxCheckBox(panel, wxID_ANY, wxT("Anamorphic"),
		wxDefaultPosition, wxDefaultSize);
	anamorphCheck->SetValue(pyuvAnamorphic);
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

	bitpersampleText = new wxSpinCtrl(panel, wxID_ANY, wxString::Format(wxT("%d"), pyuvSamplebits),
		wxDefaultPosition, wxSize(60, -1), wxTE_LEFT, 1, 16, pyuvSamplebits);

    itemSizer8->Add(bitpersampleText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);

	// Second row
    wxBoxSizer* itemSizer85 = new wxBoxSizer(wxHORIZONTAL);

    signedCheck = new wxCheckBox(panel, wxID_ANY, wxT("Signed format"),
		wxDefaultPosition, wxDefaultSize);
	signedCheck->SetValue(pyuvSigned);

    itemSizer85->Add(signedCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);

    itemSizer85->Add(3, 3, 1, wxALL, 0);

    endianCheck = new wxCheckBox(panel, wxID_ANY, wxT("Endianness"),
		wxDefaultPosition, wxDefaultSize);
	endianCheck->SetValue(pyuvEndianness);

    itemSizer85->Add(endianCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5);

	// Third row
    wxBoxSizer* itemSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemSizer9->Add(new wxStaticText(panel, wxID_ANY, wxT("Header bytes:")), 0,
		wxALL|wxALIGN_CENTER_VERTICAL, 3);

    itemSizer9->Add(3, 3, 1, wxALL, 0);

	headersizeText = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%d"), pyuvHeadersize),
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
	framerateText->GetValue().ToDouble(&pyuvRate);
	customwidthText->GetValue().ToULong(&pyuvWidth);
	pyuvWidthS = pyuvWidth / pyuvScale;
	customheightText->GetValue().ToULong(&pyuvHeight);
	pyuvHeightS = pyuvHeight / pyuvScale;
	headersizeText->GetValue().ToULong(&pyuvHeadersize);
	pyuvColorspacenumber = componentChoice->GetSelection();
	pyuvSubsamplingnumber = samplingChoice->GetSelection();
	pyuvOrder = orderChoice->GetSelection();
	pyuvInterleaved = interleaveCheck->GetValue();
	pyuvInterlaced = interlaceCheck->GetValue();
	pyuvAnamorphic = anamorphCheck->GetValue();
	pyuvSamplebits = bitpersampleText->GetValue();
	pyuvSigned = signedCheck->GetValue();
	pyuvEndianness = endianCheck->GetValue();
	gammaText->GetValue().ToDouble(&pyuvGamma);

	// When size is not well defined
	if (!(pyuvWidth*pyuvHeight)) {
		wxMessageBox(wxT("You have to select a valid size"),
					 wxT("Wait, wait..."),
					 wxOK | wxICON_WARNING,
					 this);
		return;
	}

	// When colorspace is not defined
	if (pyuvColorspacenumber == CS_UNDEF) {
		wxMessageBox(wxT("You have to select a valid colorspace"),
					 wxT("Wait, wait..."),
					 wxOK | wxICON_WARNING,
					 this);
		return;
	}

	// When subsampling is not defined
	if (pyuvSubsamplingnumber == SS_UNDEF) {
		wxMessageBox(wxT("You have to select a valid subsampling"),
					 wxT("Wait, wait..."),
					 wxOK | wxICON_WARNING,
					 this);
		return;
	}

	if (pyuvFile && pyuvFile->IsOpened()) { 

		// File and frame size
		pyuvFilesize = pyuvFile->Length();
		pyuvFramesize = (unsigned long) ((double) (pyuvWidth * pyuvHeight) * pyuvFramedensity[pyuvSubsamplingnumber]);
		pyuvFramebytes = pyuvFramesize * ((pyuvSamplebits > 8) ? 2 : 1);
		pyuvNumframes = (unsigned long) floor((double) (pyuvFilesize - pyuvHeadersize) / (double) pyuvFramebytes);
		pyuvFramenumber = 0;

		// Set up
		wxString info;
		info.Printf(wxT("%d frames, %.0f'%.0f\", %.3f MB"),
			pyuvNumframes,
			floorf(((float) pyuvNumframes / (float) pyuvRate) / 60.0F),
			((float) pyuvNumframes / (float) pyuvRate) -
				60.0F * floorf(((float) pyuvNumframes / (float) pyuvRate) / 60.0F),
			(float) pyuvFilesize / 1048576.0F);
		frame->SetStatusText(info, 1);
		frame->pyuvDispose();
		frame->pyuvPrepare();

		// Do a one-shot timer tick
		wxCommandEvent ev;
		frame->OnStop(ev);

		// Associate the drawing image
		pyuvImg = &(frame->drawCanvas->img);

	}

	// create header file
	wxFileName headername(pyuvFilename);
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
	headerfile.AddLine(wxString::Format(wxT("width = %d"), pyuvWidth));
	headerfile.AddLine(wxString::Format(wxT("height = %d"), pyuvHeight));
	headerfile.AddLine(wxString::Format(wxT("rate = %f"), pyuvRate));
	headerfile.AddLine(wxString::Format(wxT("format = %s"), pyuvFormatShort[pyuvFormat]));
	headerfile.AddLine(wxString::Format(wxT("depth = %d"), pyuvSamplebits));
	headerfile.AddLine(wxString::Format(wxT("space = %s"), pyuvComponents[pyuvColorspacenumber]));
	headerfile.AddLine(wxString::Format(wxT("gamma = %f"), pyuvGamma));

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


/////////////////////////////////////////////////////////
// Synchronization dialog methods
/////////////////////////////////////////////////////////
IMPLEMENT_CLASS(pyuvSyncDialog, wxDialog)

BEGIN_EVENT_TABLE(pyuvSyncDialog, wxDialog)
	// Button events
	EVT_BUTTON(Search_Button_ID, pyuvSyncDialog::OnSearchApps) 
	EVT_BUTTON(Identify_Button_ID, pyuvSyncDialog::OnIdentifyApps) 
	EVT_BUTTON(Unlink_Button_ID, pyuvSyncDialog::OnUnlinkApps) 
	EVT_BUTTON(wxID_OK, pyuvSyncDialog::syncApplyClick)
	EVT_BUTTON(wxID_APPLY, pyuvSyncDialog::syncApplyClick)
END_EVENT_TABLE()

// Ctor
pyuvSyncDialog::pyuvSyncDialog(wxWindow* win)
{
	// Transfer all data from window children
	// Activate a contextual help
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

	// Create the window
    Create(win, wxID_ANY, wxT("Command synchronization"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE);

	// Create the top sizer
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(topSizer); 

	// A description message
	wxString description;
	if (pyuvMyMaster >= 0)
		description = wxString::Format(wxT("Mastered by %02d"), pyuvMyMaster);
	else
		description = wxString::Format(wxT("List of instances available to me (%02d)\n(select to command them)"), pyuvServno);
	wxStaticText* descrText = new wxStaticText(this,
											   Serverlist_Description_ID,
											   description,
											   wxDefaultPosition,
											   wxDefaultSize,
											   5);
	topSizer->Add(descrText, 1, wxALIGN_LEFT|wxLEFT|wxTOP|wxRIGHT, 10);

	// Create the applications management sizer
	wxBoxSizer* appmagSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(appmagSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

	// Create the PYUVs list box
	appListbox =  new wxListBox(this, // parent
								Server_Listbox_ID, // no particular id
								wxDefaultPosition, // default position
								wxSize(-1, 180), // default size
								0, // initial number of items
								NULL, // pointer to initial items
								wxLB_MULTIPLE | wxLB_NEEDED_SB // style
								);
    appmagSizer->Add(appListbox, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
	if (pyuvMyMaster >= 0)
		appListbox->Enable(false);

	// Create the server operations sizer
	wxBoxSizer* opsSizer = new wxBoxSizer(wxVERTICAL);
    appmagSizer->Add(opsSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

	// Create the look for apps button
	wxButton* searchButton = new wxButton(this,
										  Search_Button_ID,
										  wxT("&Search"),
										  wxDefaultPosition,
										  wxDefaultSize,
										  wxBU_EXACTFIT
										  );
	opsSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);

	// Create the identify apps button
	wxButton* identifyButton = new wxButton(this,
										  Identify_Button_ID,
										  wxT("&Identify"),
										  wxDefaultPosition,
										  wxDefaultSize,
										  wxBU_EXACTFIT
										  );
	opsSizer->Add(identifyButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);

	// Create the unlink apps button
	wxButton* unlinkButton = new wxButton(this,
										  Unlink_Button_ID,
										  wxT("&Unlink"),
										  wxDefaultPosition,
										  wxDefaultSize,
										  wxBU_EXACTFIT
										  );
	opsSizer->Add(unlinkButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);
	if (pyuvMyMaster >= 0)
		unlinkButton->Enable(true);
	else
		unlinkButton->Enable(false);

	// A separation line
	wxStaticLine* bottomLine = new wxStaticLine(this,
											   wxID_STATIC,
											   wxDefaultPosition,
											   wxSize(100, 1),
											   wxLI_HORIZONTAL 
											   );
	topSizer->Add(bottomLine, 0, wxGROW|wxALL, 5);

	// Create the bottom buttons
	wxStdDialogButtonSizer* buttons = new wxStdDialogButtonSizer();
	buttons->AddButton(new wxButton(this, wxID_OK));
	buttons->AddButton(new wxButton(this, wxID_APPLY));
	buttons->AddButton(new wxButton(this, wxID_CANCEL));
	buttons->Realize();
    topSizer->Add(buttons, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

	// Lay out the dialog
	topSizer->Layout();
    topSizer->SetSizeHints(this);
    topSizer->Fit(this);

	// Fill the server fields by default and highlight my slaves
	wxCommandEvent e;
	OnSearchApps(e);
	int n/*, m*/;
	for (n = 0; n < (int) appListbox->GetCount(); n++) {
		long m;
		appListbox->GetString(n).ToLong(&m);
		if (pyuvMySlave[m])
			appListbox->Select(n);
	}
}

// Search and display other applications
void pyuvSyncDialog::OnSearchApps(wxCommandEvent& event)
{
	wxConnectionBase* connection = NULL;	
	pyuvClient* client = new pyuvClient;
	
	int servno;
	wxString command;
	//char command[255];

	// Look for existing clients and set up an IPC server
	appListbox->Clear();
	for (servno = 0; servno < MAX_NO_SERVERS; servno++) {
		
	    // Suppress the log messages from MakeConnection()
		wxLogNull noLog;

		// Skip yourself
		if (servno == pyuvServno)
			continue;

		// Look for available servers
		if ((connection = client->MakeConnection(wxEmptyString, 
							wxString::Format(wxT("/tmp/PYUV%02d"), servno),
							wxT("avail")))) {
			// Ask the other instance
			command = wxString::Format(wxT("master%02d"), pyuvServno);
			//sprintf(command, "master%02d", pyuvServno);
			if (connection->Execute(command)) {
				connection->Disconnect();
				connection = NULL;
				appListbox->Append(wxString::Format(wxT("%02d"), servno));
			};
		}
		delete connection;
	}
	delete client;

	return;
}

// Identify other applications
void pyuvSyncDialog::OnIdentifyApps(wxCommandEvent& event)
{
	wxConnectionBase* connection = NULL;	
	pyuvClient* client = new pyuvClient;
	
	int servno;

	// Look for existing clients and set up an IPC server
	for (servno = 0; servno < MAX_NO_SERVERS; servno++) {
		
	    // Suppress the log messages from MakeConnection()
		wxLogNull noLog;

		// Look for available servers
		if ((connection = client->MakeConnection(wxEmptyString,
				wxString::Format(wxT("/tmp/PYUV%02d"), servno),
				wxT("ident")))) {
			connection->Disconnect();
		}
		delete connection;
	}
	delete client;
}

// Unlink this application from the master
void pyuvSyncDialog::OnUnlinkApps(wxCommandEvent& event)
{
	wxCommandEvent e;

	// Unlink this way
	pyuvMyMaster = -1;

	// Disable unlink button
	FindWindow(Unlink_Button_ID)->Enable(false);

	// Rewrite top description
	wxString description;
	description = wxString::Format(wxT("List of instances available to me (%02d)\n(select to command them)"), pyuvServno);
	FindWindow(Serverlist_Description_ID)->SetLabel(wxString(description));

	// Reenable and refill server list
	FindWindow(Server_Listbox_ID)->Enable(true);
	OnSearchApps(e);

	// Revert background
	frame->SetOwnBackgroundColour(pyuvBackColor);
	frame->ClearBackground();

	// Lay out the dialog
	GetSizer()->Layout();
    GetSizer()->SetSizeHints(this);
    GetSizer()->Fit(this);
}

// Apply button handler
void
pyuvSyncDialog::syncApplyClick(wxCommandEvent& e)
{
	wxConnectionBase* connection = NULL;	
	pyuvClient* client;
	int n, servno;
	wxString command;
	//char command[255];

	//wxMessageBox(wxT("applySync"), wxT("Busy"), wxICON_INFORMATION|wxOK);

	// don't do anything, if we are commanded
	if (pyuvMyMaster >= 0) {
		// Check if OK is pushed
		if (e.GetId() == wxID_OK) {
			Destroy();
			return;

		} else
			return;
	};

	// First, reset all slaves
	for (servno = 0; servno < MAX_NO_SERVERS; servno++)
		pyuvMySlave[servno] = false;

	// Now, load the requested slaves
	pyuvIHaveASlave = false;
	for (n = 0; n < (int) appListbox->GetCount(); n++) {
		appListbox->GetString(n).ToLong((long *)&servno);
		if (appListbox->IsSelected(n)) {
			pyuvIHaveASlave = true;
			pyuvMySlave[servno] = true;
		}
	}

	// Call the selected slaves and book'em
	client = new pyuvClient;
	for (servno = 0; servno < MAX_NO_SERVERS; servno++) {

		// Disable screen warnings of MakeConnection()
		wxLogNull noLog;

		// Skip yourself and nonselected ones
		if (servno == pyuvServno)
			continue;

		// Look for available server
		if ((connection = client->MakeConnection(wxEmptyString, 
							wxString::Format(wxT("/tmp/PYUV%02d"), servno),
							wxT("book")))) {
			// Ask the other instance, booking in case
			if (pyuvMySlave[servno]) {
				command = wxString::Format(wxT("master%02d"), pyuvServno);
				//sprintf(command, "master%02d", pyuvServno);
				if (!connection->Execute(command))
					pyuvMySlave[servno] = false;
				else
					pyuvMySlave[servno] = true;
			} else {
				command = wxString::Format(wxT("unmaster%02d"), pyuvServno);
				//sprintf(command, "unmaster%02d", pyuvServno);
				if (!connection->Execute(command))
					pyuvMySlave[servno] = true;
				else
					pyuvMySlave[servno] = false;
			};
			connection->Disconnect();

		} else
			pyuvMySlave[servno] = false;

		delete connection;

	}
	delete client;

	// Re-highlight all confirmed slaves
	pyuvIHaveASlave = false;
	for (n = 0; n < (int) appListbox->GetCount(); n++) {
		appListbox->GetString(n).ToLong((long *)&servno);
		if (pyuvMySlave[servno]) {
			pyuvIHaveASlave = true;
			appListbox->Select(n);
		}
	}

	// Check if OK is pushed
	if (e.GetId() == wxID_OK)
		Destroy();
}

// Accepts a connection from another instance
wxConnectionBase *pyuvServer::OnAcceptConnection(const wxString& topic)
{
	//wxMessageBox(wxString::Format(wxT("%02d - incoming connection - "), pyuvServno) + topic, wxT("Busy"), wxICON_INFORMATION|wxOK);

	if ((topic.Lower() == wxT("intro")) || (topic.Lower() == wxT("dothis"))) {

		// This is used to signal the existance in life
		if (pyuvServno == MAX_NO_SERVERS)
			return NULL;
		else
			return new pyuvConnection();
		
	} else if ((topic.Lower() == wxT("avail")) || (topic.Lower() == wxT("book"))) {

		// This is used to verify that I am available to be commanded

		// Check that there are no modal dialogs active
		wxWindowList::Node* node = wxTopLevelWindows.GetFirst();
		while (node) {
			wxDialog* dialog = wxDynamicCast(node->GetData(), wxDialog);
			if (dialog && dialog->IsModal())
				return false;
			node = node->GetNext();
		};

		return new pyuvConnection();

	} else if (topic.Lower() == wxT("ident")) {

		frame->switchidentity();

		return new pyuvConnection();

	} else

		// Dunno what it wants
		return NULL;
}

// Respond to a connection
bool pyuvConnection::OnExecute(const wxString& topic, char *data,
							 int WXUNUSED(size), wxIPCFormat WXUNUSED(format))
{
	// First, raise the main window
	/*if (frame)
		frame->Raise();*/
	//const char *data = wxString(wdata).ToAscii();

	//wxMessageBox(wxString::Format(wxT("%02d - incoming connection (\"%s\" - \"%s\")"), pyuvServno, topic.c_str(), data), wxT("Busy"), wxICON_INFORMATION|wxOK);

	if (topic.Lower() == wxT("book")) {

		int servno = -1;
		if (sscanf(data, "master%02d", &servno) == 1) {

			// sorry, already booked
			if ((pyuvMyMaster >= 0) && (pyuvMyMaster != servno))
				return false;

			// booking
			if ((servno >= 0) && (servno < MAX_NO_SERVERS)) {
				pyuvMyMaster = servno;

				frame->SetOwnBackgroundColour(pyuvSlaveColors[pyuvMyMaster]);
				frame->ClearBackground();

				//wxMessageBox(wxString::Format("We (%02d) have been booked by %02d", pyuvServno, pyuvMyMaster),
				//	wxT("Busy"), wxICON_INFORMATION|wxOK);

				return true;

			} else
				return false;
		}

		servno = -1;
		if (sscanf(data, "unmaster%02d", &servno) == 1) {

			// sorry, already booked
			if ((pyuvMyMaster >= 0) && (pyuvMyMaster != servno))
				return false;

			// unbooking
			if ((servno >= 0) && (servno < MAX_NO_SERVERS)) {
				pyuvMyMaster = -1;

				frame->SetOwnBackgroundColour(pyuvBackColor);
				frame->ClearBackground();

				/*wxMessageBox(wxString::Format("We (%02d) have been un-booked by %02d", pyuvServno, servno),
					wxT("Busy"), wxICON_INFORMATION|wxOK);*/

				return true;

			} else
				return false;
		}

	} else if (topic.Lower() == wxT("avail")) {

		int servno = -1;
		sscanf(data, "master%02d", &servno);

		// availability
		if (pyuvMyMaster >= 0) {
			if (pyuvMyMaster == servno)
				return true;
			else
				return false;
		} else {
			return true;
		}

	} else if (topic.Lower() == wxT("dothis")) {

		// I cannot be commanded
		if (pyuvMyMaster == -1)
			return false;

		wxCommandEvent e;
		int frameno;

		// Execute the play/pause/stop commands
		if (!strcmp(data, "play")) {

			// play
			frame->OnPlay(e);

		} else if (!strcmp(data, "stop")) {

			// stop
			frame->OnStop(e);

		} else if (!strcmp(data, "forward")) {

			// forward
			frame->OnForward(e);

		} else if (sscanf(data, "moveto%d", &frameno) == 1) {

			// moveto
			if ((frameno < 0) || (frameno >= (int) pyuvNumframes))
				return true;

			pyuvFramenumber = frameno;
			frame->moveto(frameno);

			// Move the slider from time to time
			if (!(pyuvFramenumber % pyuvSliderupdate))
				frame->frameSlider->SetValue(pyuvFramenumber);

		} else

			return false;

		return true;

	} else

		// no good!!!!
		return false;

	return true;
}
