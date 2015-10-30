//
//  communicate.hpp
//  minimal_cocoa
//
//  Created by Andrea Melle on 07/10/2015.
//
//

#ifndef communicate_hpp
#define communicate_hpp

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "panels.h"

class Communicate : public wxFrame
{
public:
    Communicate(const wxString& title);
    
    LeftPanel *mLP;
    RightPanel *mRP;
    wxPanel *mParent;
    
};

#endif /* communicate_hpp */
