// Name:    playuvFrame.h
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

// Components and color spaces
enum
{
    CS_UNDEF = 0,
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
    CS_CUST
};

// Subsampling
enum
{
    SS_UNDEF = 0,
    SS_444,
    SS_422,
    SS_420,
    SS_411,
    SS_410,
    SS_400,
    SS_CUST
};
// Packed order
enum
{
    OR_DEFAULT,
    OR_UYVY,
    OR_YUY2,
    OR_YVYU,
    OR_BGR,
    OR_CUST
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

        static uint32_t& GetNumFrame(void) { return pyuvNumframes; }
        static uint32_t& GetFrameNumber(void) { return pyuvFramenumber; }

        static wxString& GetFileName(void) { return pyuvFilename; }
        static wxFile *GetFile(void) { return pyuvFile; }
        static wxFileOffset& GetFileSize(void) { return pyuvFilesize; }
        static uint32_t& GetFrameSize(void) { return pyuvFramesize; }
        static uint32_t& GetFrameBytes(void) { return pyuvFramebytes; }
        static const float (*GetFrameDensity(void))[] { return &pyuvFramedensity; }

        static int GetScale(void) { return pyuvScale; }
        static unsigned long int& GetWidth(void) { return pyuvWidth; }
        static unsigned long int& GetHeight(void) { return pyuvHeight; }
        static uint32_t& GetWidthScreen(void) { return pyuvWidthS; }
        static uint32_t& GetHeightScreen(void) { return pyuvHeightS; }

        static wxColour& GetInternalBackgroundColor(void) { return pyuvBackColor; }
        static bool GetDoubleBuffer(void) { return pyuvDoublebuffer; }
        static int GetEngine(void) { return pyuvEngine; }

        static wxString& GetCurrentTime(void) { return pyuvCurtime; }

        static void GetTimeCodePosition(wxCoord& x, wxCoord& y) { x = tcx; y = tcy; }
        static void GetTimeCodeDimension(wxCoord& width, wxCoord& height) { width = tcwidth; height = tcheight; }
        static void GetIdentityPosition(wxCoord& x, wxCoord& y) { x = idx; y = idy; }
        static void GetIdentityDimension(wxCoord& width, wxCoord& height) { width = idwidth; height = idheight; }
        static wxFont& GetTimeFont(void) { return pyuvTimeFont; }
        static wxFont& GetIdentityFont(void) { return pyuvIdFont; }

        static bool GetShowTimeCodeFlag(void) { return pyuvShowtimecode; }
        static bool GetShowIdentityFlag(void) { return pyuvShowidentity; }

        static wxString& GetTopBaseTitle(void) { return pyuvTopbasetitle; }
        static uint32_t GetSliderUpdate(void) { return pyuvSliderupdate; }

        static wxImage*& GetImage(void) { return pyuvImg; }

        static int& GetColorSpaceNumber(void) { return pyuvColorspacenumber; }

        static double& GetRate(void) { return pyuvRate; }
        static bool& GetInterlacedFlag(void) { return pyuvInterlaced; }
        static int& GetFormat(void) { return pyuvFormat; }

        static const wxString (*GetComponents(void))[] { return &pyuvComponents; }
        size_t GetSizeOfComponents(void) const;
        static const wxString (*GetFormatShort(void))[] { return &pyuvFormatShort; }

        static double& GetGamma(void) { return pyuvGamma; }
        static int& GetSubsamplingNumber(void) { return pyuvSubsamplingnumber; }
        static int& GetYUVOrder(void) { return  pyuvOrder; }
        static bool& GetInterleavedFlag(void) { return pyuvInterleaved; }
        static bool& GetAnamorphicFlag(void) { return pyuvAnamorphic; }
        static int& GetSampleBits(void) { return pyuvSamplebits; }
        static bool& GetSignedFlag(void) { return pyuvSigned; }
        static bool& GetEndiannessFlag(void) { return pyuvEndianness; }
        static unsigned long int& GetHeaderSize(void) { return pyuvHeadersize; }

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
            Menu_View_Zoom_400,
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

        static int pyuvSubsamplingnumber;
        static int pyuvColorspacenumber;
        static int pyuvOrder;

        static const wxString pyuvComponents[];
        static const wxString pyuvFormatShort[];
        static const int pyuvFormatSampling[];
        static const bool pyuvFormatInterleave[];
        static const int pyuvFormatOrder[];
        static const float pyuvFramedensity[];


        // Contexts and bitmaps
        static uint8_t *pyuvScreen;
        static uint8_t *pyuvScreenS;
        static uint8_t *pyuvBuffer;

        static int pyuvScale;
        static unsigned long int pyuvWidth;
        static unsigned long int pyuvHeight;
        static uint32_t pyuvWidthS;
        static uint32_t pyuvHeightS;

        static uint32_t pyuvArea;

        static bool pyuvAnamorphic;
        static double pyuvGamma;

        static int pyuvSamplebits;
        static bool pyuvSigned;
        static bool pyuvInterleaved;
        static bool pyuvEndianness;
        static int pyuvShowcomps;

        // Frame rates and number of frame in sequence
        static double pyuvRate;
        static uint32_t pyuvFramenumber;
        static uint32_t pyuvNumframes;
        static int pyuvFrameTime;
        static bool pyuvInterlaced;
        static bool pyuvDeinterlace;

        // Format shortcut
        static int pyuvFormat;

        // File related items
        static wxString pyuvFilename;
        static wxFile *pyuvFile;
        static wxFileOffset pyuvFilesize;
        static uint32_t pyuvFramesize;
        static uint32_t pyuvFramebytes;
        static unsigned long int pyuvHeadersize;

        // Visualization
        static wxColour pyuvBackColor;

        static bool pyuvDoublebuffer;
        static int pyuvEngine;

        // Timecode and identity boxes
        static wxString pyuvCurtime;
        static wxCoord tcwidth;
        static wxCoord tcheight;
        static wxCoord idwidth;
        static wxCoord idheight;
        static wxCoord tcx;
        static wxCoord tcy;
        static wxCoord idx;
        static wxCoord idy;
        static wxFont pyuvTimeFont;
        static wxFont pyuvIdFont;

        static bool pyuvPlay;
        static bool pyuvShowtimecode;
        static bool pyuvShowidentity;
        static bool pyuvLoop;

        static wxString pyuvTopbasetitle;
        static bool pyuvHelpsystem;
        static uint32_t pyuvSliderupdate;

        static wxImage *pyuvImg;

        // The animation timer
        wxTimer animTimer;

        // The help controller
        wxHtmlHelpController help;

        // This class handles events
        wxDECLARE_EVENT_TABLE();
};

