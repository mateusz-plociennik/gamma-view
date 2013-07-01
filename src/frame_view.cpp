/**
 * @file	frame_view.cpp
 * @brief	View Frame
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#include "frame_view.h"

#include "canvas.h"
#include "panel_player.h"

#include "f_end_cond.h"

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

#include <wx/datetime.h>
#include <wx/filedlg.h>

#include <wx/aui/aui.h>

enum
{
	MENU_MODE = 100,
	MENU_MODE_LIVE,
	MENU_MODE_LIVE_UNI,
	MENU_MODE_PLAYBACK,
	MENU_MODE_PLAYBACK_UNI,
	MENU_MODE_UNIFORMITY,

	Menu_View_Zoom = 200,
	Menu_View_Zoom_100,
	Menu_View_Zoom_200,
	Menu_View_Zoom_300,
	Menu_View_Zoom_Max,

	Menu_View_Colourmap = 300,
	Menu_View_Colourmap_AUTUMN,
	Menu_View_Colourmap_BONE,
	Menu_View_Colourmap_COOL,
	Menu_View_Colourmap_COPPER,
	Menu_View_Colourmap_GRAY,
	Menu_View_Colourmap_HOT,
	Menu_View_Colourmap_HSV,
	Menu_View_Colourmap_JET,
	Menu_View_Colourmap_OCEAN,
	Menu_View_Colourmap_PINK,
	Menu_View_Colourmap_RAINBOW,
	Menu_View_Colourmap_SPRING,
	Menu_View_Colourmap_SUMMER,
	Menu_View_Colourmap_WINTER,
	Menu_View_Colourmap_INVERT,

	Menu_View_Integrate = 400,
	Menu_View_Integrate_Time_1_1000,
	Menu_View_Integrate_Time_1_500,
	Menu_View_Integrate_Time_1_250,
	Menu_View_Integrate_Time_1_125,
	Menu_View_Integrate_Time_1_60,
	Menu_View_Integrate_Time_1_30,
	Menu_View_Integrate_Time_1_15,
	Menu_View_Integrate_Time_1_8,
	Menu_View_Integrate_Time_1_4,
	Menu_View_Integrate_Time_1_2,
	Menu_View_Integrate_Time_1,
	Menu_View_Integrate_Time_2,
	Menu_View_Integrate_Time_4,
	Menu_View_Integrate_Time_8,
	Menu_View_Integrate_Time_16,
	Menu_View_Integrate_Time_32,
	Menu_View_Integrate_Enabled,

	Menu_View_ImgParams = 500,
	Menu_View_ImgParams_Brightness,
	Menu_View_ImgParams_Contrast,
	Menu_View_ImgParams_Gamma,
	Menu_View_ImgParams_All,

	Menu_Help_About = 1000,
};

wxBEGIN_EVENT_TABLE(GammaFrame, wxFrame)
	EVT_CLOSE(GammaFrame::OnClose)
	EVT_MAXIMIZE(GammaFrame::OnMaximize)
	EVT_SIZE(GammaFrame::OnSize)

	//Menu events
	//EVT_MENU(wxID_NEW, GammaFrame::OnMenuNewWindow)
	//EVT_MENU(wxID_CLOSE, GammaFrame::OnMenuCloseWindow)

	EVT_MENU_RANGE(MENU_MODE, MENU_MODE_UNIFORMITY, 
		GammaFrame::OnMenuMode)
	EVT_MENU_RANGE(Menu_View_Zoom_100, Menu_View_Zoom_Max, 
		GammaFrame::OnMenuResizeWindow)
	EVT_MENU_RANGE(Menu_View_Colourmap_AUTUMN, Menu_View_Colourmap_INVERT, 
		GammaFrame::OnMenuSetColourmap)
	EVT_MENU_RANGE(Menu_View_Integrate_Time_1_1000, Menu_View_Integrate_Enabled, 
		GammaFrame::OnMenuSetIntegrate)
	EVT_MENU_RANGE(Menu_View_ImgParams_Brightness, Menu_View_ImgParams_All, 
		GammaFrame::OnMenuSetImgParams)

	EVT_MENU(wxID_ABOUT, GammaFrame::OnMenuHelpAbout)

wxEND_EVENT_TABLE()

GammaFrame::GammaFrame() 
		: 
		wxFrame(NULL, wxID_ANY, wxT("gamma-view")), 
		m_pManager(new GammaManager(this))
{
	m_mgr.SetManagedWindow(this);

	wxConfigBase* config = new wxFileConfig( "gamma-view", "MP", 
		"./gamma-view.ini", wxEmptyString, 
		wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH );
	config->SetRecordDefaults();
	wxConfigBase::Set(config);

	//SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC

	SetIcon(wxICON(gamma-view));

	// Make a menubar

	wxMenu *modeMenu = new wxMenu;
	modeMenu->AppendRadioItem(MENU_MODE_LIVE, 
		wxT("Live"), wxT("Live mode"));
	modeMenu->Check(MENU_MODE_LIVE, true);
	modeMenu->AppendRadioItem(MENU_MODE_LIVE_UNI, 
		wxT("Live (uniform)"), wxT("Live mode with uniformity"));
	modeMenu->AppendRadioItem(MENU_MODE_PLAYBACK, 
		wxT("Playback"), wxT("Playback mode"));
	modeMenu->AppendRadioItem(MENU_MODE_PLAYBACK_UNI, 
		wxT("Playback (uniform)"), wxT("Playback mode with unformity"));
	modeMenu->AppendRadioItem(MENU_MODE_UNIFORMITY, 
		wxT("Uniform matrix"), wxT("Aquisition of uniformity matrix"));

	wxMenu *viewZoomMenu = new wxMenu;
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_100, 
		wxT("&100%\tAlt+1"), wxT("Resize window to 100%"));
	viewZoomMenu->Check(Menu_View_Zoom_100, true);
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_200, 
		wxT("&200%\tAlt+2"), wxT("Resize window to 200%"));
	viewZoomMenu->AppendRadioItem(Menu_View_Zoom_300, 
		wxT("&300%\tAlt+3"), wxT("Resize window to 300%"));

	wxMenu *viewColourmapMenu = new wxMenu;
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_GRAY, 
		wxT("&Gray\tCtrl+`"), wxT("Change colourmap to Gray"));
	viewColourmapMenu->Check(Menu_View_Colourmap_GRAY, true);
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_AUTUMN, 
		wxT("&Autumn\tCtrl+1"), wxT("Change colourmap to Autumn"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_BONE, 
		wxT("&Bone\tCtrl+2"), wxT("Change colourmap to Bone"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_COOL, 
		wxT("&Cool\tCtrl+3"), wxT("Change colourmap to Cool"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_HOT, 
		wxT("&Hot\tCtrl+4"), wxT("Change colourmap to Hot"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_HSV, 
		wxT("&HSV\tCtrl+5"), wxT("Change colourmap to HSV"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_JET, 
		wxT("&Jet\tCtrl+6"), wxT("Change colourmap to Jet"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_OCEAN, 
		wxT("&Ocean\tCtrl+7"), wxT("Change colourmap to Ocean"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_PINK, 
		wxT("&Pink\tCtrl+8"), wxT("Change colourmap to Pink"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_RAINBOW, 
		wxT("&Rainbow\tCtrl+9"), wxT("Change colourmap to Rainbow"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_SPRING, 
		wxT("&Spring\tCtrl+0"), wxT("Change colourmap to Spring"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_SUMMER, 
		wxT("&Summer\tCtrl+-"), wxT("Change colourmap to Summer"));
	viewColourmapMenu->AppendRadioItem(Menu_View_Colourmap_WINTER, 
		wxT("&Winter\tCtrl+="), wxT("Change colourmap to Winter"));
	viewColourmapMenu->AppendSeparator();
	viewColourmapMenu->AppendCheckItem(Menu_View_Colourmap_INVERT,
		wxT("&Invert\tCtrl+I"), wxT("Invert colourmap"));

	wxMenu *viewIntegrateMenu = new wxMenu;
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_1000, 
		wxT("1/1000 s"), wxT("Change integrate time to 1/1000 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_500, 
		wxT("1/500 s"), wxT("Change integrate time to 1/500 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_250,
		wxT("1/250 s"), wxT("Change integrate time to 1/250 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_125,
		wxT("1/125 s"), wxT("Change integrate time to 1/125 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_60,
		wxT("1/60 s"), wxT("Change integrate time to 1/60 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_30,
		wxT("1/30 s"), wxT("Change integrate time to 1/30 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_15,
		wxT("1/15 s"), wxT("Change integrate time to 1/15 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_8,
		wxT("1/8 s"), wxT("Change integrate time to 1/8 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_4,
		wxT("1/4 s"), wxT("Change integrate time to 1/4 s"));
	viewIntegrateMenu->Check(Menu_View_Integrate_Time_1_4, true);
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1_2,
		wxT("1/2 s"), wxT("Change integrate time to 1/2 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_1,
		wxT("1 s"), wxT("Change integrate time to 1 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_2,
		wxT("2 s"), wxT("Change integrate time to 2 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_4,
		wxT("4 s"), wxT("Change integrate time to 4 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_8,
		wxT("8 s"), wxT("Change integrate time to 8 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_16,
		wxT("16 s"), wxT("Change integrate time to 16 s"));
	viewIntegrateMenu->AppendRadioItem(Menu_View_Integrate_Time_32,
		wxT("32 s"), wxT("Change integrate time to 32 s"));
	viewIntegrateMenu->AppendSeparator();
	viewIntegrateMenu->AppendCheckItem(Menu_View_Integrate_Enabled, 
		wxT("&Integrate\tSpace"), wxT("Image integrate"));

	wxMenu *viewImgParamsMenu = new wxMenu;
	viewImgParamsMenu->Append(Menu_View_ImgParams_Brightness, 
		wxT("Reset Brightness"), wxT("Reset Brightness to 0.00"));
	viewImgParamsMenu->Append(Menu_View_ImgParams_Contrast, 
		wxT("Reset Constrast"), wxT("Reset Contrast to 1.00"));
	viewImgParamsMenu->Append(Menu_View_ImgParams_Gamma, 
		wxT("Reset Gamma"), wxT("Reset Gamma to 1.00"));
	viewImgParamsMenu->Append(Menu_View_ImgParams_All, 
		wxT("Reset All"), wxT("Reset All to defaults"));

	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append(Menu_View_Zoom, wxT("Window Size"), viewZoomMenu);
	viewMenu->Append(Menu_View_Colourmap, wxT("Colourmap"), viewColourmapMenu);
	viewMenu->Append(Menu_View_Integrate, wxT("Integrate"), viewIntegrateMenu);
	viewMenu->Append(Menu_View_ImgParams, wxT("Brightness/Contrast/Gamma"), viewImgParamsMenu);

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(modeMenu, wxT("&Mode"));
	menuBar->Append(viewMenu, wxT("&View"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	CreateStatusBar();
	int width_fields[] = {-1,66};
	GetStatusBar()->SetFieldsCount(2, width_fields);

	wxString status;
	status.Printf("B=0.00; C=1.00; G=1.00");
	GetStatusBar()->SetStatusText(status, 0);

	wxLogWindow* log = new wxLogWindow(this, "Log Window");
	log->GetFrame()->SetIcon(wxICON(gamma-view));
	wxLog::SetTimestamp("%H:%M:%S,%l");
	wxLog::SetActiveTarget(log);

////////////////////////////////////////////////////////////////////////////////

	m_centerSizer = new wxBoxSizer(wxVERTICAL);

	m_canvas = new GammaCanvas(this, wxID_ANY);
	//m_centerSizer->Add(m_canvas, 1, wxSHAPED|wxALIGN_CENTER|wxADJUST_MINSIZE);
	m_mgr.AddPane(m_canvas, wxAuiPaneInfo().
                  Caption(_("Statistics")).Center().Dockable(false).BestSize(256,256));

	m_bottomPanel = new GammaPlayerPanel(this, wxID_ANY);
	m_centerSizer->Add(m_bottomPanel, 0, wxEXPAND);

	m_horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	m_horizontalSizer->Add(m_centerSizer, 1, wxEXPAND);

	m_sidePanel = new GammaSidePanel(this, wxID_ANY);
	//m_horizontalSizer->Add(m_sidePanel, 0, wxEXPAND);
	m_mgr.AddPane(m_sidePanel, wxAuiPaneInfo().
                  Caption(_("Statistics")).Right().Resizable(false));
	
	SetSizerAndFit(m_horizontalSizer);

	m_mgr.Update();

	Show();

	

	//m_pManager->setMode(GAMMA_MODE_USB_2_IMAGE_UNI);
	m_pManager->setMode(GAMMA_MODE_FILE_2_IMAGE);
	//m_pManager->setMode(GAMMA_MODE_FAKE_2_IMAGE);
}

GammaFrame::~GammaFrame()
{
	m_mgr.UnInit();

	GetManager()->setMode(GAMMA_MODE_NONE);

	delete wxConfigBase::Set(NULL);
	delete GetManager();
}

void GammaFrame::OnClose(wxCloseEvent& event)
{
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
	info.SetVersion(wxT("1.0"));

	wxDateTime dt;
	wxString::const_iterator dEnd, tEnd;
	dt.ParseDate(__DATE__, &dEnd);
	dt.ParseTime(__TIME__, &tEnd);

	info.SetDescription(
		_("\nMade with ") + wxString(wxVERSION_STRING) + 
		_("\nCompile time: ") + dt.FormatDate() + " " + dt.FormatTime() +
		wxT("\n"));

	info.SetCopyright(wxT("(C) ") 
		+ wxString::Format(wxT("%d"), dt.GetYear()) 
		+ wxT(" Mateusz Plociennik"));
	//info.AddDeveloper(wxT("Mateusz Plociennik"));

	info.SetWebSite(wxT("http://github.com/mateusz-plociennik/gamma-view"), _("GitHub Web Site"));

	wxAboutBox(info, this);
}

void GammaFrame::OnMenuMode(wxCommandEvent& commandEvent)
{
	switch(commandEvent.GetId())
	{
	default:
	case MENU_MODE_LIVE:
	case MENU_MODE_LIVE_UNI:
	case MENU_MODE_PLAYBACK:
		openFile(); break;
	case MENU_MODE_PLAYBACK_UNI:
	case MENU_MODE_UNIFORMITY:
		{
			new GammaEndCondDialog(m_pManager);
		}
		break;
	}
}

wxString GammaFrame::openFile()
{
	if(true)
	{
		if(wxMessageBox(_("Current content has not been saved! Proceed?"), 
			_("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
		{
			return wxString("");
			//else: proceed asking to the user the new file to open
		}
	}

	wxFileDialog openFileDialog(this, _("Open GVB file"), "", "", 
		_("GVB files (*.gvb)|*.gvb"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if(openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return wxString(""); // the user changed idea...
	}

	return openFileDialog.GetPath();
}

wxString GammaFrame::saveFile()
{
	wxFileDialog saveFileDialog(this, _("Save GVB file"), "", "",
		_("GVB files (*.gvb)|*.gvb"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return wxString(""); // the user changed idea...
	}
	
	return saveFileDialog.GetPath();
}

void GammaFrame::OnMenuNewWindow(wxCommandEvent& WXUNUSED(event))
{
	new GammaFrame();
}

void GammaFrame::OnMenuResizeWindow(wxCommandEvent& commandEvent)
{
	if( IsMaximized() )
	{
		Maximize(false);
	}

	switch(commandEvent.GetId())
	{
	case Menu_View_Zoom_100:
		m_canvas->DoSetBestSize(wxSize(256,256)); break;
	case Menu_View_Zoom_200:
		m_canvas->DoSetBestSize(wxSize(512,512)); break;
	case Menu_View_Zoom_300:
		m_canvas->DoSetBestSize(wxSize(768,768)); break;
	}

	Fit();
}

void GammaFrame::OnMenuSetColourmap(wxCommandEvent& event)
{
	if( event.GetId() == Menu_View_Colourmap_INVERT )
	{
		bool invert = event.IsChecked();
		GetManager()->DataTierSetParam(GAMMA_PARAM_COLOURMAP_INVERT, (void*)&invert);
	}
	else
	{
		GammaColourmap_e colourmap;
		switch ( event.GetId() )
		{
		case Menu_View_Colourmap_AUTUMN:
			colourmap = GAMMA_COLOURMAP_AUTUMN; break;
		case Menu_View_Colourmap_BONE:
			colourmap = GAMMA_COLOURMAP_BONE; break;
		case Menu_View_Colourmap_COOL:
			colourmap = GAMMA_COLOURMAP_COOL; break;
		case Menu_View_Colourmap_COPPER:
			colourmap = GAMMA_COLOURMAP_COPPER; break;
		default:
		case Menu_View_Colourmap_GRAY:
			colourmap = GAMMA_COLOURMAP_GRAY; break;
		case Menu_View_Colourmap_HOT:
			colourmap = GAMMA_COLOURMAP_HOT; break;
		case Menu_View_Colourmap_HSV:
			colourmap = GAMMA_COLOURMAP_HSV; break;
		case Menu_View_Colourmap_JET:
			colourmap = GAMMA_COLOURMAP_JET; break;
		case Menu_View_Colourmap_OCEAN:
			colourmap = GAMMA_COLOURMAP_OCEAN; break;
		case Menu_View_Colourmap_PINK:
			colourmap = GAMMA_COLOURMAP_PINK; break;
		case Menu_View_Colourmap_RAINBOW:
			colourmap = GAMMA_COLOURMAP_RAINBOW; break;
		case Menu_View_Colourmap_SPRING:
			colourmap = GAMMA_COLOURMAP_SPRING; break;
		case Menu_View_Colourmap_SUMMER:
			colourmap = GAMMA_COLOURMAP_SUMMER; break;
		case Menu_View_Colourmap_WINTER:
			colourmap = GAMMA_COLOURMAP_WINTER; break;
		}
		GetManager()->DataTierSetParam(GAMMA_PARAM_COLOURMAP, (void*)&colourmap);
	}
}

void GammaFrame::OnMenuSetIntegrate(wxCommandEvent& event)
{
	if( event.GetId() == Menu_View_Integrate_Enabled )
	{
		bool bIntegrate = event.IsChecked();
		GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_INTEGRATE_ENABLED, (void*)&bIntegrate);
	}
	else
	{
		wxTimeSpan intTime;
		switch ( event.GetId() )
		{
		case Menu_View_Integrate_Time_1_1000:
			intTime = wxTimeSpan::Milliseconds(1); break;
		case Menu_View_Integrate_Time_1_500:
			intTime = wxTimeSpan::Milliseconds(2); break;
		case Menu_View_Integrate_Time_1_250:
			intTime = wxTimeSpan::Milliseconds(4); break;
		case Menu_View_Integrate_Time_1_125:
			intTime = wxTimeSpan::Milliseconds(8); break;
		case Menu_View_Integrate_Time_1_60:
			intTime = wxTimeSpan::Milliseconds(17); break;
		case Menu_View_Integrate_Time_1_30:
			intTime = wxTimeSpan::Milliseconds(33); break;
		case Menu_View_Integrate_Time_1_15:
			intTime = wxTimeSpan::Milliseconds(67); break;
		case Menu_View_Integrate_Time_1_8:
			intTime = wxTimeSpan::Milliseconds(125); break;
		default:
		case Menu_View_Integrate_Time_1_4:
			intTime = wxTimeSpan::Milliseconds(250); break;
		case Menu_View_Integrate_Time_1_2:
			intTime = wxTimeSpan::Milliseconds(500); break;
		case Menu_View_Integrate_Time_1:
			intTime = wxTimeSpan::Milliseconds(1000); break;
		case Menu_View_Integrate_Time_2:
			intTime = wxTimeSpan::Milliseconds(2000); break;
		case Menu_View_Integrate_Time_4:
			intTime = wxTimeSpan::Milliseconds(4000); break;
		case Menu_View_Integrate_Time_8:
			intTime = wxTimeSpan::Milliseconds(8000); break;
		case Menu_View_Integrate_Time_16:
			intTime = wxTimeSpan::Milliseconds(16000); break;
		case Menu_View_Integrate_Time_32:
			intTime = wxTimeSpan::Milliseconds(32000); break;
		}
		GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_INTEGRATE_TIME, (void*)&intTime);
	}
}

void GammaFrame::OnMenuSetImgParams(wxCommandEvent& event)
{
	switch(event.GetId())
	{
	case Menu_View_ImgParams_Brightness:
		m_canvas->m_brightness = 0.0; break;
	case Menu_View_ImgParams_Contrast:
		m_canvas->m_contrast = 1.0; break;
	case Menu_View_ImgParams_Gamma:
		m_canvas->m_gamma = 1.0; break;
	case Menu_View_ImgParams_All:
		m_canvas->m_brightness = 0.0;
		m_canvas->m_contrast = 1.0;
		m_canvas->m_gamma = 1.0; break;
	}

	GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_BRIGHTNESS, &m_canvas->m_brightness);
	GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_CONTRAST, &m_canvas->m_contrast);
	GetManager()->DataTierSetParam(GAMMA_PARAM_IMG_GAMMA, &m_canvas->m_gamma);

	GetStatusBar()->SetStatusText(wxString::Format(wxT("B=%.2f; C=%.2f; G=%.2f"), 
		m_canvas->m_brightness, m_canvas->m_contrast, m_canvas->m_gamma), 0);
}

bool GammaFrame::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	//case GAMMA_PARAM_MATRIX_DATA:
	case GAMMA_PARAM_IMG_DATA:
		{
			wxMutexLocker locker(m_canvas->m_paintMutex);
			m_canvas->m_image = *static_cast<wxImage*>(value);
			m_canvas->Refresh(false);
			break;
		}
	case GAMMA_PARAM_TIME_NOW:
		{
			m_bottomPanel->m_timeNow = *static_cast<wxTimeSpan*>(value);
			m_bottomPanel->Refresh(false);
			break;
		}
	case GAMMA_PARAM_TIME_END:
		{
			m_bottomPanel->m_timeEnd = *static_cast<wxTimeSpan*>(value);
			m_bottomPanel->Refresh(false);
			break;
		}
	case GAMMA_PARAM_DATA_TYPE_MATRIX:
		{
			GammaMatrix* pMatrix = static_cast<GammaMatrix*>(value);
			m_sidePanel->m_frequency = (double)1000 * pMatrix->eventSum / pMatrix->span.GetValue().GetValue();
			m_sidePanel->m_eventAvg = (double)pMatrix->eventSum / GetManager()->getConfig()->getFieldOfView()->getPointCount();
			m_sidePanel->m_eventMax = pMatrix->eventMax;
			m_sidePanel->m_eventSum = pMatrix->eventSum;
			m_sidePanel->m_trig = pMatrix->trig;
			
			m_sidePanel->Refresh(false);
		}
		break;
	case GAMMA_PARAM_TRIG_TYPE:
		{
			GammaTrig_e trig = *static_cast<GammaTrig_e*>(value);
			wxLogStatus("%s() trig = %d", __FUNCTION__, trig);
			break;
		}
	default:
		return false;
	}

	return true;
}
