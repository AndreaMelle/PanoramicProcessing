//
//  main.hpp
//  minimal_cocoa
//
//  Created by Andrea Melle on 07/10/2015.
//
//

#ifndef main_hpp
#define main_hpp

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

#endif /* main_hpp */
