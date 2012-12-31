/**
 * @file	frame_view.cpp
 * @brief	View Frame
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#include "frame_view.h"

#include <algorithm>

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include "block_tr_mi.h"
#include <wx/aboutdlg.h>

#include <wx/config.h>
#include <wx/fileconf.h>

enum
{
	Menu_View_Zoom = 200,
	Menu_View_Zoom_50,
	Menu_View_Zoom_100,
	Menu_View_Zoom_200,
	Menu_View_Zoom_300,

	Menu_View_Colormap = 300,
	Menu_View_Colormap_AUTUMN,
	Menu_View_Colormap_BONE,
	Menu_View_Colormap_COOL,
	Menu_View_Colormap_COPPER,
	Menu_View_Colormap_GRAY,
	Menu_View_Colormap_HOT,
	Menu_View_Colormap_HSV,
	Menu_View_Colormap_JET,
	Menu_View_Colormap_OCEAN,
	Menu_View_Colormap_PINK,
	Menu_View_Colormap_RAINBOW,
	Menu_View_Colormap_SPRING,
	Menu_View_Colormap_SUMMER,
	Menu_View_Colormap_WINTER,
	Menu_View_Colormap_INVERT,

	Menu_View_Integrate = 400,
	Menu_View_Integrate_Enabled,
	
	Menu_Help_About = 500
};

wxBEGIN_EVENT_TABLE(GammaFrame, wxFrame)
	EVT_CLOSE(GammaFrame::OnClose)
	EVT_PAINT(GammaFrame::OnPaint)

	EVT_LEFT_DOWN(GammaFrame::OnMouse)
	EVT_LEFT_UP(GammaFrame::OnMouse)
	EVT_MOTION(GammaFrame::OnMouse)

	//Menu events
	EVT_MENU(wxID_NEW, GammaFrame::OnMenuNewWindow)
	EVT_MENU(wxID_CLOSE, GammaFrame::OnMenuCloseWindow)
	
	EVT_MENU_RANGE(Menu_View_Zoom_50, Menu_View_Zoom_300, GammaFrame::OnMenuResizeWindow)

	EVT_MENU_RANGE(Menu_View_Colormap_AUTUMN, Menu_View_Colormap_INVERT, GammaFrame::OnMenuSetColormap)

	EVT_MENU_RANGE(Menu_View_Integrate_Enabled, Menu_View_Integrate_Enabled, GammaFrame::OnMenuSetIntegrate)

	EVT_MENU(Menu_Help_About, GammaFrame::OnMenuHelpAbout)
wxEND_EVENT_TABLE()

GammaFrame::GammaFrame() 
		: 
		wxFrame(NULL, wxID_ANY, wxT("gamma-view")), 
		m_pManager(new GammaManager(this)),
		m_image(256, 256, true),
		m_fineScale(true),
		m_brightness(0.0),
		m_contrast(1.0)
{
	wxConfigBase* config = new wxFileConfig( "gamma-view", "MP", 
		"./gamma-view.ini", wxEmptyString, 
		wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH );
	config->SetRecordDefaults();
	wxConfigBase::Set(config);

	SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC

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

	wxMenu *viewColormapMenu = new wxMenu;
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_GRAY, 
		wxT("&Gray\tCtrl-0"), wxT("Change colormap to Gray"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_AUTUMN, 
		wxT("&Autumn\tCtrl-F1"), wxT("Change colormap to Autumn"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_BONE, 
		wxT("&Bone\tCtrl-F2"), wxT("Change colormap to Bone"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_COOL, 
		wxT("&Cool\tCtrl-F3"), wxT("Change colormap to Cool"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_HOT, 
		wxT("&Hot\tCtrl-F4"), wxT("Change colormap to Hot"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_HSV, 
		wxT("&HSV\tCtrl-F5"), wxT("Change colormap to HSV"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_JET, 
		wxT("&Jet\tCtrl-F6"), wxT("Change colormap to Jet"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_OCEAN, 
		wxT("&Ocean\tCtrl-F7"), wxT("Change colormap to Ocean"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_PINK, 
		wxT("&Pink\tCtrl-F8"), wxT("Change colormap to Pink"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_RAINBOW, 
		wxT("&Rainbow\tCtrl-F9"), wxT("Change colormap to Rainbow"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_SPRING, 
		wxT("&Spring\tCtrl-F10"), wxT("Change colormap to Spring"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_SUMMER, 
		wxT("&Summer\tCtrl-F11"), wxT("Change colormap to Summer"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_WINTER, 
		wxT("&Winter\tCtrl-F12"), wxT("Change colormap to Winter"));
	viewColormapMenu->AppendSeparator();
	viewColormapMenu->AppendCheckItem(Menu_View_Colormap_INVERT,
		wxT("&Invert\tCtrl-I"), wxT("Invert colormap"));

	wxMenu *viewIntegrateMenu = new wxMenu;
	viewIntegrateMenu->AppendCheckItem(Menu_View_Integrate_Enabled, 
		wxT("&Integrate\tSpace"), wxT("Image integrated"));
	
	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append(Menu_View_Zoom, wxT("Window Size"), viewZoomMenu);
	viewMenu->Append(Menu_View_Colormap, wxT("Colormap"), viewColormapMenu);
	viewMenu->Append(Menu_View_Integrate, wxT("Integrate"), viewIntegrateMenu);

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Menu_Help_About, 
		wxT("About..."), wxT("About the program"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(viewMenu, wxT("&View"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	CreateStatusBar();
	GetStatusBar()->SetFieldsCount(2);

	wxString status;
	status.Printf("Brightness = %.3f, Contrast = %.3f", m_brightness, m_contrast);
	GetStatusBar()->SetStatusText(status, 0);

	viewZoomMenu->Check(Menu_View_Zoom_200, true);
	SetClientSize(512, 512);
	//Maximize(true);

	Show();

	wxLogWindow* log = new wxLogWindow(this, "Log Window");
	log->GetFrame()->SetIcon(wxICON(gamma-view));
	wxLog::SetTimestamp("%H:%M:%S,%l");
	wxLog::SetActiveTarget(log);

	m_pManager->SetMode(GAMMA_MODE_FILE_2_IMAGE);

	
}

GammaFrame::~GammaFrame()
{
	delete wxConfigBase::Set(NULL);
	delete GetManager();
}

void GammaFrame::OnClose(wxCloseEvent& event)
{
	GetManager()->SetMode(GAMMA_MODE_NONE);

	event.Skip();
}

void GammaFrame::OnMenuCloseWindow(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void GammaFrame::OnMenuHelpAbout(wxCommandEvent& WXUNUSED(event))
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

void GammaFrame::OnMenuNewWindow(wxCommandEvent& WXUNUSED(event))
{
		new GammaFrame();
}

void GammaFrame::OnMenuResizeWindow(wxCommandEvent& commandEvent)
{
	switch( commandEvent.GetId() )
	{
		case Menu_View_Zoom_50:
			SetClientSize(128, 128); break;
		case Menu_View_Zoom_100:
			SetClientSize(256, 256); break;
		case Menu_View_Zoom_200:
			SetClientSize(512, 512); break;
		case Menu_View_Zoom_300:
			SetClientSize(768, 768); break;
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

void GammaFrame::OnMenuSetColormap(wxCommandEvent& commandEvent)
{
	if ( commandEvent.GetId() == Menu_View_Colormap_INVERT )
	{
		bool invert = commandEvent.IsChecked();

		GetManager()->DataTierSetParam(GAMMA_PARAM_COLORMAP_INVERT, (void*)&invert);
	}
	else
	{
		GammaColormap_e colormap;

		switch ( commandEvent.GetId() )
		{
			case Menu_View_Colormap_AUTUMN:
				colormap = GAMMA_COLORMAP_AUTUMN; break;
			case Menu_View_Colormap_BONE:
				colormap = GAMMA_COLORMAP_BONE; break;
			case Menu_View_Colormap_COOL:
				colormap = GAMMA_COLORMAP_COOL; break;
			case Menu_View_Colormap_COPPER:
				colormap = GAMMA_COLORMAP_COPPER; break;
			default:
			case Menu_View_Colormap_GRAY:
				colormap = GAMMA_COLORMAP_GRAY; break;
			case Menu_View_Colormap_HOT:
				colormap = GAMMA_COLORMAP_HOT; break;
			case Menu_View_Colormap_HSV:
				colormap = GAMMA_COLORMAP_HSV; break;
			case Menu_View_Colormap_JET:
				colormap = GAMMA_COLORMAP_JET; break;
			case Menu_View_Colormap_OCEAN:
				colormap = GAMMA_COLORMAP_OCEAN; break;
			case Menu_View_Colormap_PINK:
				colormap = GAMMA_COLORMAP_PINK; break;
			case Menu_View_Colormap_RAINBOW:
				colormap = GAMMA_COLORMAP_RAINBOW; break;
			case Menu_View_Colormap_SPRING:
				colormap = GAMMA_COLORMAP_SPRING; break;
			case Menu_View_Colormap_SUMMER:
				colormap = GAMMA_COLORMAP_SUMMER; break;
			case Menu_View_Colormap_WINTER:
				colormap = GAMMA_COLORMAP_WINTER; break;
		}

		GetManager()->DataTierSetParam(GAMMA_PARAM_COLORMAP, (void*)&colormap);
	}
}

void GammaFrame::OnMenuSetIntegrate(wxCommandEvent& commandEvent)
{
		switch ( commandEvent.GetId() )
		{
			case Menu_View_Integrate_Enabled:
				{
					bool bIntegrate = commandEvent.IsChecked();
					GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_INTEGRATE_ENABLED, (void*)&bIntegrate);
					break;
				}
			default:
				break;
		}
}

void GammaFrame::OnMouse(wxMouseEvent& event)
{
	wxMutexLocker locker(m_mouseMutex);

	if ( event.Dragging() )
	{
		m_brightness += ( 0.001 * (event.GetX() - m_startX) );
		m_contrast *= pow( 1.001, -(event.GetY() - m_startY) );

		GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_BRIGHTNESS, &m_brightness);
		GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_CONTRAST, &m_contrast);

		wxString status;
		status.Printf("Brightness = %.3f, Contrast = %.3f", m_brightness, m_contrast);
		GetStatusBar()->SetStatusText(status, 0);
	}
	if ( event.LeftDown() )
	{
		//status += "Ld";
	}
	if ( event.LeftUp() )
	{
		//status += "Lu";
	}

	m_startX = event.GetX();
	m_startY = event.GetY();

	{
		wxString status;
		status.Printf("(%.0f,%.0f)", (m_startX/m_imgScale), (m_startY/m_imgScale));
		GetStatusBar()->SetStatusText(status, 1);
	}

	event.Skip();
}

void GammaFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxAutoBufferedPaintDC pdc(this);

	{
		wxMutexLocker locker(m_imageMutex);
		
		double scaleX = (double)GetClientSize().GetWidth()/m_image.GetWidth();
		double scaleY = (double)GetClientSize().GetHeight()/m_image.GetHeight();
		m_imgScale = std::min(scaleX, scaleY);
		
		pdc.SetUserScale(m_imgScale, m_imgScale);
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}
}

bool GammaFrame::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_DATA:
		{
			wxMutexLocker locker(m_imageMutex);
			m_image = *static_cast<wxImage*>(value);

			Refresh(false);
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}
