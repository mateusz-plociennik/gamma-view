/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     gamma-view main
// Author:      Mateusz Plociennik
// Created:     04/01/98
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAIN_H_
#define _WX_MAIN_H_

#include <wx/app.h>
#include <wx/frame.h>

// Define a new application type
class GammaApp : public wxApp
{
public:
	GammaApp() 
	{ 
	}

	// virtual wxApp methods
	virtual bool OnInit();
	virtual int OnExit();

private:
	wxFrame* m_pFrame;

};

#endif // _WX_MAIN_H_
