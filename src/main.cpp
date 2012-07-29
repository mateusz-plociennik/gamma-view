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
#include "block_mgmt.h"
#include <wx/fileconf.h>

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

	wxConfigBase* config= new wxFileConfig( "gamma-view", "MP", 
		"./gamma-view.ini", wxEmptyString, 
		wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH );
	config->SetRecordDefaults();
	wxConfigBase::Set(config);

    new MyFrame();

	GammaBlockManager::getInstance().SetMode(GAMMA_MODE_USB_2_FILE);

    return true;
}

int MyApp::OnExit()
{
	GammaBlockManager::getInstance().SetMode(GAMMA_MODE_NONE);
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
	Menu_View_Zoom_50,
    Menu_View_Zoom_100,
	Menu_View_Zoom_200,
	Menu_View_Zoom_300,
	Menu_View_Zoom_400,
	
	Menu_Help_About = 300
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
    EVT_MENU(wxID_CLOSE, MyFrame::OnClose)

	EVT_MENU(Menu_View_Zoom_50, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_100, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_200, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_300, MyFrame::OnResizeWindow)

	EVT_MENU(Menu_Help_About, MyFrame::OnHelpAbout)
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
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_50, 
		wxT("&50%\tAlt-1"), wxT("Resize window to 50%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_100, 
		wxT("&100%\tAlt-2"), wxT("Resize window to 100%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_200, 
		wxT("&200%\tAlt-3"), wxT("Resize window to 200%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_300, 
		wxT("&300%\tAlt-4"), wxT("Resize window to 300%"));
	
	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append(Menu_View_Zoom, 
		wxT("Window Size"), viewZoomMenu);

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Menu_Help_About, 
		wxT("About..."), wxT("About the program"));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(viewMenu, wxT("&View"));
	menuBar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menuBar);

    CreateStatusBar();

	viewZoomMenu->Check(Menu_View_Zoom_200, true);
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

#include "wx/aboutdlg.h"

void MyFrame::OnHelpAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;
    
	info.SetName(wxT("gamma-view"));
    info.SetVersion(wxT("0.1"));

	wxDateTime dt;
	wxString::const_iterator dEnd, tEnd;
	dt.ParseDate(__DATE__, &dEnd);
	dt.ParseTime(__TIME__, &tEnd);

	info.SetDescription(
		"\nMade with " + wxString(wxVERSION_STRING) + 
		"\nDate: " + dt.FormatDate() +
		"\nTime: " + dt.FormatTime() +
		"\n");
    info.SetCopyright(wxT("(C) 2012 Mateusz Plociennik"));
    //info.AddDeveloper(wxT("Mateusz Plociennik"));

	info.SetWebSite(wxT("http://github.com/mateusz-plociennik/gamma-view"), wxT("Web Site"));

    wxAboutBox(info, this);
}

void MyFrame::OnNewWindow(wxCommandEvent& WXUNUSED(event))
{
    new MyFrame();
}

void MyFrame::OnResizeWindow(wxCommandEvent& commandEvent)
{
	switch( commandEvent.GetId() )
	{
		case Menu_View_Zoom_50:
		{
			SetClientSize(128, 128);
			break;
		}
		case Menu_View_Zoom_100:
		{
			SetClientSize(256, 256);
			break;
		}
		case Menu_View_Zoom_200:
		{
			SetClientSize(512, 512);
			break;
		}
		case Menu_View_Zoom_300:
		{
			SetClientSize(768, 768);
			break;
		}
/*		case Menu_View_Zoom_Other:
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
