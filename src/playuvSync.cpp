// Name:    playuvSync.cpp
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

// turn off 'warning C4355: 'this' : used in base member initializer list'
#if defined(_WIN32_)
    #pragma warning ( disable:4355 )
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "playuvsync.cpp"
    #pragma interface "playuvSync.cpp"
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

#include <wx/bitmap.h>
#include <wx/cmdline.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/html/helpctrl.h>
#include <wx/ipc.h>
#include <wx/rawbmp.h>
#include <wx/statline.h>

#include <stdint.h>

// Inclusion of the main header file
#include "playuvFrame.h"
#include "playuvApp.h"
#include "playuvSync.h"

/////////////////////////////////////////////////////////
// Synchronization dialog methods
/////////////////////////////////////////////////////////
wxIMPLEMENT_CLASS(pyuvSyncDialog, wxDialog)

BEGIN_EVENT_TABLE(pyuvSyncDialog, wxDialog)
    // Button events
    EVT_BUTTON(Search_Button_ID, pyuvSyncDialog::OnSearchApps) 
    EVT_BUTTON(Identify_Button_ID, pyuvSyncDialog::OnIdentifyApps) 
    EVT_BUTTON(Unlink_Button_ID, pyuvSyncDialog::OnUnlinkApps) 
    EVT_BUTTON(wxID_OK, pyuvSyncDialog::syncApplyClick)
    EVT_BUTTON(wxID_APPLY, pyuvSyncDialog::syncApplyClick)
END_EVENT_TABLE()

// Ctor
pyuvSyncDialog::pyuvSyncDialog(wxWindow* win) : parent(win)
{
    frame = (pyuvFrame *)win;

    // Transfer all data from window children
    // Activate a contextual help
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    // Create the window
    Create(win, wxID_ANY, _("Command synchronization"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE);

    // Create the top sizer
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(topSizer); 

    // A description message
    wxString description;
    if (pyuvApp::GetMyMaster() >= 0)
        description = wxString::Format(_("Mastered by %02d"), pyuvApp::GetMyMaster());
    else
        description = wxString::Format(_("List of instances available to me (%02d)\n(select to command them)"), pyuvApp::GetServerNo());
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
    if (pyuvApp::GetMyMaster() >= 0)
        appListbox->Enable(false);

    // Create the server operations sizer
    wxBoxSizer* opsSizer = new wxBoxSizer(wxVERTICAL);
    appmagSizer->Add(opsSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);

    // Create the look for apps button
    wxButton* searchButton = new wxButton(this,
                                          Search_Button_ID,
                                          _("&Search"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxBU_EXACTFIT
                                          );
    opsSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);

    // Create the identify apps button
    wxButton* identifyButton = new wxButton(this,
                                          Identify_Button_ID,
                                          _("&Identify"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxBU_EXACTFIT
                                          );
    opsSizer->Add(identifyButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);

    // Create the unlink apps button
    wxButton* unlinkButton = new wxButton(this,
                                          Unlink_Button_ID,
                                          _("&Unlink"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxBU_EXACTFIT
                                          );
    opsSizer->Add(unlinkButton, 0, wxALIGN_CENTER_VERTICAL|wxGROW|wxALL, 5);
    if (pyuvApp::GetMyMaster() >= 0)
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
        if (pyuvApp::GetMyClient()[m])
            appListbox->Select(n);
    }
}

// Search and display other applications
void pyuvSyncDialog::OnSearchApps(wxCommandEvent& event)
{
    wxConnectionBase* connection = NULL;    
    pyuvClient* client = new pyuvClient;

    client->SetFrame(frame);

    int servno;
    wxString command;
    //char command[255];

    // Look for existing clients and set up an IPC server
    appListbox->Clear();
    for (servno = 0; servno < MAX_NO_SERVERS; servno++) {
        // Suppress the log messages from MakeConnection()
        wxLogNull noLog;

        // Skip yourself
        if (servno == pyuvApp::GetServerNo())
            continue;

        // Look for available servers
        if ((connection = client->MakeConnection(wxEmptyString, 
                            wxString::Format(wxT("/tmp/PYUV%02d"), servno),
                            wxT("avail")))) {
            // Ask the other instance
            command = wxString::Format(wxT("master%02d"), pyuvApp::GetServerNo());
            //sprintf(command, "master%02d", pyuvApp::GetServerNo());
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

    client->SetFrame(frame);

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
    pyuvApp::GetMyMaster() = -1;

    // Disable unlink button
    FindWindow(Unlink_Button_ID)->Enable(false);

    // Rewrite top description
    wxString description;
    description = wxString::Format(_("List of instances available to me (%02d)\n(select to command them)"), pyuvApp::GetServerNo());
    FindWindow(Serverlist_Description_ID)->SetLabel(wxString(description));

    // Reenable and refill server list
    FindWindow(Server_Listbox_ID)->Enable(true);
    OnSearchApps(e);

    // Revert background
    frame->SetOwnBackgroundColour(parent->GetBackgroundColour());
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
    if (pyuvApp::GetMyMaster() >= 0) {
        // Check if OK is pushed
        if (e.GetId() == wxID_OK) {
            Destroy();
            return;

        } else
            return;
    };

    // First, reset all slaves
    for (servno = 0; servno < MAX_NO_SERVERS; servno++)
        (*pyuvApp::GetMyClient())[servno] = false;

    // Now, load the requested slaves
    pyuvApp::GetHaveClientFlag() = false;
    for (n = 0; n < (int) appListbox->GetCount(); n++) {
        appListbox->GetString(n).ToLong((long *)&servno);
        if (appListbox->IsSelected(n)) {
            pyuvApp::GetHaveClientFlag() = true;
            (*pyuvApp::GetMyClient())[servno] = true;
        }
    }

    // Call the selected slaves and book'em
    client = new pyuvClient;
    client->SetFrame(frame);

    for (servno = 0; servno < MAX_NO_SERVERS; servno++) {

        // Disable screen warnings of MakeConnection()
        wxLogNull noLog;

        // Skip yourself and nonselected ones
        if (servno == pyuvApp::GetServerNo())
            continue;

        // Look for available server
        if ((connection = client->MakeConnection(wxEmptyString, 
                            wxString::Format(wxT("/tmp/PYUV%02d"), servno),
                            wxT("book")))) {
            // Ask the other instance, booking in case
            if ((*pyuvApp::GetMyClient())[servno]) {
                command = wxString::Format(wxT("master%02d"), pyuvApp::GetServerNo());
                //sprintf(command, "master%02d", pyuvApp::GetServerNo());
                if (!connection->Execute(command))
                    (*pyuvApp::GetMyClient())[servno] = false;
                else
                    (*pyuvApp::GetMyClient())[servno] = true;
            } else {
                command = wxString::Format(wxT("unmaster%02d"), pyuvApp::GetServerNo());
                //sprintf(command, "unmaster%02d", pyuvApp::GetServerNo());
                if (!connection->Execute(command))
                    (*pyuvApp::GetMyClient())[servno] = true;
                else
                    (*pyuvApp::GetMyClient())[servno] = false;
            };
            connection->Disconnect();

        } else
            (*pyuvApp::GetMyClient())[servno] = false;

        delete connection;

    }
    delete client;

    // Re-highlight all confirmed slaves
    pyuvApp::GetHaveClientFlag() = false;
    for (n = 0; n < (int) appListbox->GetCount(); n++) {
        appListbox->GetString(n).ToLong((long *)&servno);
        if ((*pyuvApp::GetMyClient())[servno]) {
            pyuvApp::GetHaveClientFlag() = true;
            appListbox->Select(n);
        }
    }

    // Check if OK is pushed
    if (e.GetId() == wxID_OK)
        Destroy();
}

