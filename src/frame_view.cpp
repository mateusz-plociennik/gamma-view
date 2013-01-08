/**
 * @file	frame_view.cpp
 * @brief	View Frame
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#include "frame_view.h"

#include "canvas.h"

#include <algorithm>

#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include "block_tr_mi.h"
#include <wx/aboutdlg.h>

#include <wx/config.h>
#include <wx/fileconf.h>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/slider.h>

enum
{
	Menu_View_Zoom = 200,
	Menu_View_Zoom_100,
	Menu_View_Zoom_200,
	Menu_View_Zoom_300,
	Menu_View_Zoom_Max,

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
	EVT_MAXIMIZE(GammaFrame::OnMaximize)
	EVT_SIZE(GammaFrame::OnSize)

	//Menu events
	EVT_MENU(wxID_NEW, GammaFrame::OnMenuNewWindow)
	EVT_MENU(wxID_CLOSE, GammaFrame::OnMenuCloseWindow)
	
	EVT_MENU_RANGE(Menu_View_Zoom_100, Menu_View_Zoom_Max, GammaFrame::OnMenuResizeWindow)

	EVT_MENU_RANGE(Menu_View_Colormap_AUTUMN, Menu_View_Colormap_INVERT, GammaFrame::OnMenuSetColormap)

	EVT_MENU_RANGE(Menu_View_Integrate_Enabled, Menu_View_Integrate_Enabled, GammaFrame::OnMenuSetIntegrate)

	EVT_MENU(wxID_ABOUT, GammaFrame::OnMenuHelpAbout)
wxEND_EVENT_TABLE()

GammaFrame::GammaFrame() 
		: 
		wxFrame(NULL, wxID_ANY, wxT("gamma-view")), 
		m_pManager(new GammaManager(this)),
		m_fineScale(true),
		m_brightness(0.0),
		m_contrast(1.0)
{
	wxConfigBase* config = new wxFileConfig( "gamma-view", "MP", 
		"./gamma-view.ini", wxEmptyString, 
		wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH );
	config->SetRecordDefaults();
	wxConfigBase::Set(config);

	//SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC

	SetIcon(wxICON(gamma-view));

	// Make a menubar
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE);

	wxMenu *viewZoomMenu = new wxMenu;
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_100, 
		wxT("&100%\tAlt+1"), wxT("Resize window to 100%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_200, 
		wxT("&200%\tAlt+2"), wxT("Resize window to 200%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_300, 
		wxT("&300%\tAlt+3"), wxT("Resize window to 300%"));

	wxMenu *viewColormapMenu = new wxMenu;
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_GRAY, 
		wxT("&Gray\tCtrl+`"), wxT("Change colormap to Gray"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_AUTUMN, 
		wxT("&Autumn\tCtrl+1"), wxT("Change colormap to Autumn"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_BONE, 
		wxT("&Bone\tCtrl+2"), wxT("Change colormap to Bone"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_COOL, 
		wxT("&Cool\tCtrl+3"), wxT("Change colormap to Cool"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_HOT, 
		wxT("&Hot\tCtrl+4"), wxT("Change colormap to Hot"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_HSV, 
		wxT("&HSV\tCtrl+5"), wxT("Change colormap to HSV"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_JET, 
		wxT("&Jet\tCtrl+6"), wxT("Change colormap to Jet"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_OCEAN, 
		wxT("&Ocean\tCtrl+7"), wxT("Change colormap to Ocean"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_PINK, 
		wxT("&Pink\tCtrl+8"), wxT("Change colormap to Pink"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_RAINBOW, 
		wxT("&Rainbow\tCtrl+9"), wxT("Change colormap to Rainbow"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_SPRING, 
		wxT("&Spring\tCtrl+0"), wxT("Change colormap to Spring"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_SUMMER, 
		wxT("&Summer\tCtrl+-"), wxT("Change colormap to Summer"));
	viewColormapMenu->AppendRadioItem(Menu_View_Colormap_WINTER, 
		wxT("&Winter\tCtrl+="), wxT("Change colormap to Winter"));
	viewColormapMenu->AppendSeparator();
	viewColormapMenu->AppendCheckItem(Menu_View_Colormap_INVERT,
		wxT("&Invert\tCtrl+I"), wxT("Invert colormap"));

	wxMenu *viewIntegrateMenu = new wxMenu;
	viewIntegrateMenu->AppendCheckItem(Menu_View_Integrate_Enabled, 
		wxT("&Integrate\tSpace"), wxT("Image integrate"));
	
	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append(Menu_View_Zoom, wxT("Window Size"), viewZoomMenu);
	viewMenu->Append(Menu_View_Colormap, wxT("Colormap"), viewColormapMenu);
	viewMenu->Append(Menu_View_Integrate, wxT("Integrate"), viewIntegrateMenu);

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT);

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

	wxLogWindow* log = new wxLogWindow(this, "Log Window");
	log->GetFrame()->SetIcon(wxICON(gamma-view));
	wxLog::SetTimestamp("%H:%M:%S,%l");
	wxLog::SetActiveTarget(log);

	m_pManager->SetMode(GAMMA_MODE_FILE_2_IMAGE);

////////////////////////////////////////////////////////////////////////////////

	m_mainSizer = new wxBoxSizer(wxVERTICAL);

	m_canvas = new GammaCanvas(this, wxID_ANY);
	m_mainSizer->Add(m_canvas, 1, /*wxEXPAND|*/wxSHAPED|wxALIGN_CENTER|wxADJUST_MINSIZE);

	m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	m_bottomPanel = new wxPanel(this, wxID_ANY);
	m_timeNowLabel = new wxStaticText(m_bottomPanel, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeNowLabel, 0, wxALIGN_CENTER|wxALL, 2);
	m_bottomSlider = new wxSlider(m_bottomPanel, wxID_ANY, 0, 0, 1000);
	m_bottomSizer->Add(m_bottomSlider, 1, wxEXPAND|wxALIGN_CENTER);
	m_timeEndLabel = new wxStaticText(m_bottomPanel, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeEndLabel, 0, wxALIGN_CENTER|wxALL, 2);
	m_bottomPanel->SetSizerAndFit(m_bottomSizer);
	m_mainSizer->Add(m_bottomPanel, 0, wxEXPAND);


	//this->SetSizer();
	this->SetSizerAndFit(m_mainSizer);

	m_canvas->SetClientSize(512, 512);
	
	Show();
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

void GammaFrame::OnMaximize(wxMaximizeEvent& event)
{
	event.Skip();
}

void GammaFrame::OnSize(wxSizeEvent& event)
{
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
	if ( IsMaximized() )
	{
		Maximize(false);
	}

	switch( commandEvent.GetId() )
	{
		case Menu_View_Zoom_100:
			m_canvas->DoSetBestSize(wxSize(256,256));	break;
		case Menu_View_Zoom_200:
			m_canvas->DoSetBestSize(wxSize(512,512)); break;
		case Menu_View_Zoom_300:
			m_canvas->DoSetBestSize(wxSize(768,768)); break;
	}

	Fit();


	
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

void GammaFrame::OnMenuSetIntegrate(wxCommandEvent& event)
{
		switch ( event.GetId() )
		{
			case Menu_View_Integrate_Enabled:
				{
					bool bIntegrate = event.IsChecked();
					GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_INTEGRATE_ENABLED, (void*)&bIntegrate);
					break;
				}
			default:
				{
					break;
				}
		}
}

bool GammaFrame::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_DATA:
		{
			wxMutexLocker locker(m_canvas->m_paintMutex);
			m_canvas->m_image = *static_cast<wxImage*>(value);
			m_canvas->Refresh(false);
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}
