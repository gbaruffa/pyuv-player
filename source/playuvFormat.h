// Name:    playuvFormat.h
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

//////////////////////////////
// Video format sheet dialog
//////////////////////////////
class pyuvFormatDialog : public wxPropertySheetDialog
{
    wxDECLARE_CLASS(pyuvFormatDialog);

public:
    pyuvFormatDialog(wxWindow *parent);

    wxPanel *CreateMainSettingsPage(wxWindow *parent);
    wxPanel *CreateExtraSettingsPage(wxWindow *parent);

    void iratespinHandler(wxSpinEvent& e);
    void fratespinHandler(wxSpinEvent& e);
    void ratetextHandler(wxCommandEvent& e);
    void formatApplyClick(wxCommandEvent& e);
    void sizechoiceHandler(wxCommandEvent &event);
    void sizetextHandler(wxCommandEvent& e);
    void interleavedcheckHandler(wxCommandEvent& e);

private:
    static pyuvFrame *frame;

    // File related items
    static wxFileOffset pyuvFilesize;
    static uint32_t pyuvFramesize;
    static uint32_t pyuvFramebytes;

    static const wxString pyuvResolutions[];
    static const wxString pyuvComponents[];
    static const wxString pyuvSampling[];
    static const wxString pyuvFourcc[];

    static const uint32_t pyuvWidths[];
    static const uint32_t pyuvHeights[];

    wxTextCtrl *framerateText;
    wxSpinButton *iframerateSpin;
    wxSpinButton *fframerateSpin;
    wxTextCtrl *customwidthText;
    wxTextCtrl *customheightText;
    wxTextCtrl *headersizeText;
    wxSpinCtrl *bitpersampleText;
    wxCheckBox *signedCheck;
    wxChoice *standardsizeChoice;
    wxChoice *componentChoice;
    wxChoice *samplingChoice;
    wxChoice *orderChoice;
    wxCheckBox *interleaveCheck;
    wxCheckBox *anamorphCheck;
    wxCheckBox *endianCheck;
    wxCheckBox *interlaceCheck;
    wxTextCtrl *gammaText;

protected:
    enum
    {
        ID_FRACRATESPIN = 100,
        ID_INTRATESPIN,
        ID_RATETEXT,
        ID_SIZECHOICE,
        ID_WIDTHTEXT,
        ID_HEIGHTTEXT,
        ID_INTERLEAVED,
        ID_GAMMATEXT
    };

    wxDECLARE_EVENT_TABLE();
};

