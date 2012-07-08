///////////////////////////////////////////////////////////////////////////////
// Name:        cube.cpp
// Purpose:     wxGLCanvas demo program
// Author:      Mateusz Plociennik
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "main.h"
#include "glview.h"

#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "../../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// MyApp: the application object
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    new MyFrame();

    return true;
}

int MyApp::OnExit()
{
    delete m_glContext;

    return wxApp::OnExit();
}

TestGLContext& MyApp::GetContext(wxGLCanvas *canvas)
{
    if ( !m_glContext )
    {
        // Create the OpenGL context for the first window which needs it:
        // subsequently created windows will all share the same context.
        m_glContext = new TestGLContext(canvas);
    }

    m_glContext->SetCurrent(*canvas);

    return *m_glContext;
}

// ----------------------------------------------------------------------------
// MyFrame: main application window
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum
{
	Menu_View_Zoom = 200,
	Menu_ViewZoomMenu_50,
    Menu_ViewZoomMenu_100,
	Menu_ViewZoomMenu_200,
	Menu_ViewZoomMenu_300,
	Menu_ViewZoomMenu_400,
//	Menu_ViewZoomMenu_Other
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
    EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
	EVT_MENU(Menu_ViewZoomMenu_50, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_ViewZoomMenu_100, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_ViewZoomMenu_200, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_ViewZoomMenu_300, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_ViewZoomMenu_400, MyFrame::OnResizeWindow)
//	EVT_MENU(Menu_ViewZoomMenu_Other, MyFrame::OnResizeWindow)
END_EVENT_TABLE()

MyFrame::MyFrame()
       : wxFrame(NULL, wxID_ANY, wxT("gamma-view"))
{
    new TestGLCanvas(this);

    SetIcon(wxICON(gamma-view));

    // Make a menubar
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_NEW);
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_CLOSE);

	wxMenu *viewZoomMenu = new wxMenu;
	viewZoomMenu->AppendRadioItem(Menu_ViewZoomMenu_50, 
		wxT("&50%\tAlt-1"), wxT("Resize window to 50%"));
	viewZoomMenu->AppendRadioItem(Menu_ViewZoomMenu_100, 
		wxT("&100%\tAlt-2"), wxT("Resize window to 100%"));
	viewZoomMenu->AppendRadioItem(Menu_ViewZoomMenu_200, 
		wxT("&200%\tAlt-3"), wxT("Resize window to 200%"));
	viewZoomMenu->AppendRadioItem(Menu_ViewZoomMenu_300, 
		wxT("&300%\tAlt-4"), wxT("Resize window to 300%"));
/*	viewZoomMenu->Append(Menu_ViewZoomMenu_Other, 
		wxT("&Other...\tAlt-5"), wxT("Resize window to..."));
*/
	
	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append(Menu_View_Zoom, wxT("Window Size"), viewZoomMenu);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(viewMenu, wxT("&View"));

    SetMenuBar(menuBar);

    CreateStatusBar();

	viewZoomMenu->Check(Menu_ViewZoomMenu_200, true);
    SetClientSize(512, 512);

    Show();

    // test IsDisplaySupported() function:
    static const int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
    wxLogStatus("Double-buffered display %s supported",
                wxGLCanvas::IsDisplaySupported(attribs) ? "is" : "not");
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnNewWindow( wxCommandEvent& WXUNUSED(event) )
{
    new MyFrame();
}

#include "wx/numdlg.h"

void MyFrame::OnResizeWindow( wxCommandEvent& commandEvent )
{
	switch( commandEvent.GetId() )
	{
		case Menu_ViewZoomMenu_50:
		{
			SetClientSize(128, 128);
			break;
		}
		case Menu_ViewZoomMenu_100:
		{
			SetClientSize(256, 256);
			break;
		}
		case Menu_ViewZoomMenu_200:
		{
			SetClientSize(512, 512);
			break;
		}
		case Menu_ViewZoomMenu_300:
		{
			SetClientSize(768, 768);
			break;
		}
/*		case Menu_ViewZoomMenu_Other:
		{
			long res = wxGetNumberFromUser( 
				wxT("Enter size in percent.\n"),
				wxT("Enter a number:"), 
				wxT("Window Resize"),
				100, 50, 1000, this );

			if ( res == -1 )
			{
				wxMessageBox(wxT("Invalid number entered or dialog cancelled."), 
					wxT("Window resize result"), wxOK | wxICON_HAND, this);
			}
			else
			{
				SetClientSize(256 * ((float)res/100), 256 * ((float)res/100));
			}

			break;
		}
*/
	}
}
