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

// turn off 'warning C4355: 'this' : used in base member initializer list'
#if defined(_WIN32_)
    #pragma warning ( disable:4355 )
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "playuvApp.cpp"
    #pragma interface "playuvApp.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// Main wxWidget include
#include <wx/wx.h>
#include <wx/log.h>
#include <wx/dcbuffer.h>
#include <wx/image.h>
#include <wx/rawbmp.h>
#include <wx/bitmap.h>
#include <wx/datetime.h>
#include <wx/regex.h>
#include <wx/file.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/html/helpctrl.h>
#include <wx/propdlg.h>
#include <wx/generic/propdlg.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/artprov.h>
#include <wx/event.h>
#include <wx/statline.h>
#include <wx/ipc.h>
#include <wx/snglinst.h>
#include <wx/numdlg.h>
#include <wx/clipbrd.h>

// Icons include
#include "playuv16.xpm"
#include "playuv250x200.xpm"
#include "play16.xpm"
#include "pause16.xpm"
#include "stop16.xpm"

// Versioning
#define  PYUV_SHORTNAME     "PYUV"
#define  PYUV_VERSION		0
#define  PYUV_SUBVERSION	6
#define  PYUV_BETA			"beta"
#define  PYUV_BTIME			wxT(__DATE__) wxT(", ") wxT(__TIME__)
#define  PYUV_WEBSITE		"http://dsplab.diei.unipg.it/pyuv_raw_video_sequence_player"

// Standard dims for the frame
#define  PYUV_WIDTH		250
#define  PYUV_HEIGHT	200
#define  PYUV_DEPTH		8
#define  PYUV_SIGNED	false
#define  PYUV_MINWIDTH	150
#define  PYUV_MINHEIGHT	100
#define  PYUV_RATE      25.00
#define  PYUV_FORMAT    3
#define  PYUV_VERTBORD  2
#define  PYUV_HORIZBORD 2

// Other small things
#define  HELP_FILENAME  "doc.htb"

// Classes declaration

////////////////////////////////////
// Interprocess communications
////////////////////////////////////
#define  MAX_NO_SERVERS	20

// Server class, for listening to connection requests
class pyuvServer: public wxServer
{
	public:
		wxConnectionBase *OnAcceptConnection(const wxString& topic);
};

// Connection class, for use by both communicating instances
class pyuvConnection : public wxConnection
{
	public:
		pyuvConnection() {};
		~pyuvConnection() {};
		bool OnExecute(const wxString& topic, char *data,
									 int WXUNUSED(size), wxIPCFormat WXUNUSED(format));
};

// Client class, to be used by subsequent instances in OnInit
class pyuvClient: public wxClient
{
	public:
		pyuvClient() {};
		wxConnectionBase *OnMakeConnection() { return new pyuvConnection; }
};

//////////////////////////////////////////////
// This is the application class declaration
//////////////////////////////////////////////
class pyuvApp : public wxApp
{
	public:
		// Called on application startup
		virtual bool OnInit();
		virtual int OnExit();

	private:
		pyuvServer* m_server;
};

// Implements pyuvApp& GetApp()
DECLARE_APP(pyuvApp)

//////////////////////////////
// Synchronization dialog
//////////////////////////////
class pyuvSyncDialog: public wxDialog
{
	DECLARE_CLASS(pyuvSyncDialog)
	public:
		pyuvSyncDialog(wxWindow* parent);

		void OnSearchApps(wxCommandEvent& event);
		void OnIdentifyApps(wxCommandEvent& event);
		void OnUnlinkApps(wxCommandEvent& event);
		void syncApplyClick(wxCommandEvent& e);


	private:
		// Constants
		enum
		{
			Search_Button_ID = 100,
			Identify_Button_ID,
			Unlink_Button_ID,
			Serverlist_Description_ID,
			Server_Listbox_ID
		};

		wxListBox* appListbox;

	protected:

		DECLARE_EVENT_TABLE()
};

//////////////////////////////
// Video format sheet dialog
//////////////////////////////
class pyuvFormatDialog: public wxPropertySheetDialog
{
	DECLARE_CLASS(pyuvFormatDialog)
	public:
		pyuvFormatDialog(wxWindow* parent);

		wxPanel* CreateMainSettingsPage(wxWindow* parent);
		wxPanel* CreateExtraSettingsPage(wxWindow* parent);

		void iratespinHandler(wxSpinEvent& e);
		void fratespinHandler(wxSpinEvent& e);
		void ratetextHandler(wxCommandEvent& e);
		void formatApplyClick(wxCommandEvent& e);
		void sizechoiceHandler(wxCommandEvent &event);
		void sizetextHandler(wxCommandEvent& e);
		void interleavedcheckHandler(wxCommandEvent& e);

	private:
		wxTextCtrl* framerateText;
		wxSpinButton* iframerateSpin;
		wxSpinButton* fframerateSpin;
		wxTextCtrl* customwidthText;
		wxTextCtrl* customheightText;
		wxTextCtrl* headersizeText;
		wxSpinCtrl* bitpersampleText;
		wxCheckBox* signedCheck;
		wxChoice* standardsizeChoice;
		wxChoice* componentChoice;
		wxChoice* samplingChoice;
		wxChoice* orderChoice;
		wxCheckBox* interleaveCheck;
		wxCheckBox* anamorphCheck;
		wxCheckBox* endianCheck;
		wxCheckBox* interlaceCheck;
		wxTextCtrl* gammaText;

	protected:

		enum {
			ID_FRACRATESPIN = 100,
			ID_INTRATESPIN,
			ID_RATETEXT,
			ID_SIZECHOICE,
			ID_WIDTHTEXT,
			ID_HEIGHTTEXT,
			ID_INTERLEAVED,
			ID_GAMMATEXT
		};

		DECLARE_EVENT_TABLE()
};

// Define a canvas for drawing onto
class pyuvCanvas;

////////////////////////////////////////////////
//// This is the main frame class declaration
////////////////////////////////////////////////
class pyuvFrame : public wxFrame
{
	public:
		// Constructor
		pyuvFrame(const wxString& title);

		// Event handlers
		void OnOpen(wxCommandEvent& event);
		void OnClose(wxCommandEvent& event);
		void OnSaveAs(wxCommandEvent& event);
		void OnFormat(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);

		void OnPlay(wxCommandEvent& event);
		void OnPause(wxCommandEvent& event);
		void OnStop(wxCommandEvent& event);
		void OnRewind(wxCommandEvent& event);
		void OnForward(wxCommandEvent& event);
		void OnGoto(wxCommandEvent& event);
		void OnLoop(wxCommandEvent& event);
		void OnSync(wxCommandEvent& event);

		void OnTimecode(wxCommandEvent& event);
		void OnDeinterlace(wxCommandEvent& event);
		void OnIdentity(wxCommandEvent& event);
		void OnScale(wxCommandEvent& event);
		void OnComp(wxCommandEvent& event);
		void OnCopy(wxCommandEvent& event);

		void OnEngine(wxCommandEvent& event);
		void OnDouble(wxCommandEvent& event);

		void OnAbout(wxCommandEvent& event);
		void OnContents(wxCommandEvent& event);

		void OnScroll(wxScrollEvent& event);

		// Timer handler
		void OnAnimate(wxTimerEvent& event);

		bool pyuvDraw(void);
		void pyuvPrepare(void);
		void pyuvDispose(void);
		void openfile(wxFileName videofile);
		void saveasfile(wxString videofile);
		bool readfile(unsigned char *pyuvBuffer, int numbytes);
		void moveto(int frameno);

		void switchidentity(void);
		bool sendcommand(char *cmd);

		pyuvCanvas *drawCanvas;
		wxSlider *frameSlider;
		wxBoxSizer *frameSizer;
		wxBoxSizer *controlSizer;
		wxBitmapButton* playButton;
		wxBitmapButton* stopButton;

	private:
		// Constants for pyuvFrame
		enum
		{
			Menu_File_Format = 100,
			Menu_Control_Play,
			Menu_Control_Stop,
			Menu_Control_Rewind,
			Menu_Control_Forward,
			Menu_Control_Loop,
			Menu_Control_Sync,
			Menu_Control_Goto,
			Menu_View,
			Menu_View_Timecode,
			Menu_View_Deinterlace,
			Menu_View_Identity,
			Menu_View_Zoom,
			Menu_View_Zoom_200,
			Menu_View_Zoom_100,
			Menu_View_Zoom_75,
			Menu_View_Zoom_50,
			Menu_View_Zoom_33,
			Menu_View_Zoom_25,
			Menu_View_Comp,
			Menu_View_Comp_All,
			Menu_View_Comp_Next,
			Menu_Settings,
			Menu_Settings_Draw,
			Menu_Settings_Draw_Bitmap,
			Menu_Settings_Draw_Raw,
			Menu_Settings_Draw_OGL,
			Menu_Settings_Draw_Double,
			Menu_Help_Contents,
			ANIMATE_TIMER_ID,
			SLIDER_ID,
			PlayButton_id,
			StopButton_id
		};

		// The animation timer
		wxTimer animTimer;

		// The help controller
		wxHtmlHelpController help;

		// This class handles events
		DECLARE_EVENT_TABLE()
};


/////////////////////////////////////////
// Define a canvas for drawing onto
/////////////////////////////////////////
typedef wxPixelData<wxBitmap, wxNativePixelFormat> PixelData;

class pyuvCanvas: public wxPanel
{
	public:
		// Constructor
		pyuvCanvas(pyuvFrame *parent);

		// A device context
		//wxClientDC cdc;

		// The two images used for rendering
		wxImage img; // basic canvas
		wxImage logoimg; // default logo displaied when no file is loaded
		wxBitmap* pyuvBmp; // the bitmap used for drawing
		PixelData* pyuvBmpdata; // raw bitmap data
		PixelData::Iterator* pyuvPixel, *pyuvRowstart;

		// Some handlers
		void OnMotion(wxMouseEvent &event); // moving mouse on canvas
		void OnPaint(wxPaintEvent &event); // redraw the canvas
		void OnDrop(wxDropFilesEvent& event); // receive a dragged file

	private:

		// This class handles events
		DECLARE_EVENT_TABLE()
};

