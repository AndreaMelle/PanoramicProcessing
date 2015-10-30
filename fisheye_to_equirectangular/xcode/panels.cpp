#include "wx/stattext.h"
#include "panels.h"
#include "communicate.h"

LeftPanel::LeftPanel(wxPanel *parent)
: wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
    count = 0;
    mParent = parent;
    mPlus = new wxButton(this, ID_PLUS, wxT("+"), wxPoint(10, 10));
    mMinus = new wxButton(this, ID_MINUS, wxT("-"), wxPoint(10, 60));
    
    Connect(ID_PLUS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LeftPanel::OnPlus));
    Connect(ID_MINUS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LeftPanel::OnMinus));
}

void LeftPanel::OnPlus(wxCommandEvent& WXUNUSED(event))
{
    count++;
    Communicate *comm = (Communicate*) mParent->GetParent();
    comm->mRP->mText->SetLabel(wxString::Format(wxT("%d"), count));
}

void LeftPanel::OnMinus(wxCommandEvent& WXUNUSED(event))
{
    count--;
    Communicate *comm = (Communicate*) mParent->GetParent();
    comm->mRP->mText->SetLabel(wxString::Format(wxT("%d"), count));
}

RightPanel::RightPanel(wxPanel* parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150), wxBORDER_SUNKEN)
{
    mText = new wxStaticText(this, -1, wxT("0"), wxPoint(40, 60));
}
