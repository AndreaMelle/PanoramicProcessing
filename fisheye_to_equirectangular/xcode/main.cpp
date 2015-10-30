//
//  main.cpp
//  minimal_cocoa
//
//  Created by Andrea Melle on 07/10/2015.
//
//

#include "main.h"
#include "Communicate.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    
    Communicate *communicate = new Communicate(wxT("Widgets communicate"));
    communicate->Show(true);
    
    return true;
}
