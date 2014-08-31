// Name:    playuvSync.h
// Purpose: Main window of the YUV player application
// Author:  Giuseppe Baruffa

//////////////////////////////
// Synchronization dialog
//////////////////////////////
class pyuvSyncDialog : public wxDialog
{
    wxDECLARE_CLASS(pyuvSyncDialog);
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

    wxListBox *appListbox;
    wxWindow *parent;
    pyuvFrame *frame;

protected:
    wxDECLARE_EVENT_TABLE();
};

