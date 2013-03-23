///////////////////////////////////////////////////////////////////////////////
// Name:				cube.cpp
// Purpose:		 wxGLCanvas demo program
// Author:			Mateusz Plociennik
// Licence:		 wxWindows licence
///////////////////////////////////////////////////////////////////////////////

//Standard wxWidgets headers...
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Let's rock:

#include "main.h"

#include <wx/app.h>
#include "frame_view.h"


// ----------------------------------------------------------------------------
// MyApp: the application object
// ----------------------------------------------------------------------------

IMPLEMENT_APP(GammaApp);

bool GammaApp::OnInit()
{
	if( !wxApp::OnInit() )
	{
		return false;
	}

	new GammaFrame();

	return true;
}

int GammaApp::OnExit()
{
	return wxApp::OnExit();
}

