// Name:    playuvSync.h
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

