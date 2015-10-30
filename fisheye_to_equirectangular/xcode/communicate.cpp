//
//  communicate.cpp
//  minimal_cocoa
//
//  Created by Andrea Melle on 07/10/2015.
//
//

#include "communicate.h"

Communicate::Communicate(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(290, 150))
{
    mParent = new wxPanel(this, wxID_ANY);
    
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    
    mLP = new LeftPanel(mParent);
    mRP = new RightPanel(mParent);
    
    hbox->Add(mLP, 1, wxEXPAND | wxALL, 5);
    hbox->Add(mRP, 1, wxEXPAND | wxALL, 5);
    
    mParent->SetSizer(hbox);
    
    this->Centre();
}
