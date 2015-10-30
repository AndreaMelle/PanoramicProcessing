//
//  panels.h
//  minimal_cocoa
//
//  Created by Andrea Melle on 07/10/2015.
//
//

#ifndef panels_h
#define panels_h

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/panel.h"

class LeftPanel : public wxPanel
{
public:
    LeftPanel(wxPanel *parent);
    
    void OnPlus(wxCommandEvent& event);
    void OnMinus(wxCommandEvent& event);
    
    wxButton *mPlus;
    wxButton *mMinus;
    wxPanel *mParent;
    int count;
    
};

class RightPanel : public wxPanel
{
public:
    RightPanel(wxPanel *parent);
    void OnSetText(wxCommandEvent& event);
    wxStaticText* mText;
    
};

const int ID_PLUS = 101;
const int ID_MINUS = 102;

#endif /* panels_h */
