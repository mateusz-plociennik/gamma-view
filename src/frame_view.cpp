/**
 * @file	frame_view.cpp
 * @brief	View Frame
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#include "frame_view.h"
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include "block_tr_mi.h"
#include "wx/aboutdlg.h"

wxBEGIN_EVENT_TABLE(GammaFrameView, wxFrame)
	EVT_CLOSE(GammaFrameView::OnClose)
	EVT_PAINT(GammaFrameView::OnPaint)

	EVT_LEFT_DOWN(GammaFrameView::OnMouse)
	EVT_LEFT_UP(GammaFrameView::OnMouse)
	EVT_MOTION(GammaFrameView::OnMouse)

	//
	EVT_CLOSE(MyFrame::OnCloseWindow)

	EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
	EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
	
	EVT_MENU(Menu_View_Zoom_50, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_100, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_200, MyFrame::OnResizeWindow)
	EVT_MENU(Menu_View_Zoom_300, MyFrame::OnResizeWindow)

	EVT_MENU(Menu_Help_About, MyFrame::OnHelpAbout)
wxEND_EVENT_TABLE()

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

GammaFrameView::GammaFrameView( 
	GammaBlockBase* block, 
	wxWindow* parent, 
	wxWindowID id, 
	const wxString &title, 
	const wxPoint &pos, 
	const wxSize &size, 
	long style, 
	const wxString &name )
	: wxFrame(parent, id, title, pos, size, style, name)
	, m_block(block)
	, m_image(256, 256, true)
	, m_fineScale(true)
	, m_brightness(0.0)
	, m_contrast(1.0)

	
	SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC

	SetIcon(wxICON(gamma-view));
	CreateStatusBar();
	GetStatusBar()->SetFieldsCount(2);
	SetClientSize(512, 512);
	Show();

MyFrame::MyFrame() : 
		wxFrame(NULL, wxID_ANY, wxT("gamma-view")), 
		m_pManager(new GammaManager) 
{
	wxConfigBase* config = new wxFileConfig( "gamma-view", "MP", 
		"./gamma-view.ini", wxEmptyString, 
		wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_RELATIVE_PATH );
	config->SetRecordDefaults();
	wxConfigBase::Set(config);

	SetIcon(wxICON(gamma-view));

	// Make a menubar
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE);
	//printf("hello world\n");

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
	//SetClientSize(512, 512);
	Maximize(true);

	Show();

	wxLogWindow* log = new wxLogWindow(this, "Log Window");
	log->GetFrame()->SetIcon(wxICON(gamma-view));
	wxLog::SetTimestamp("%H:%M:%S,%l");
	wxLog::SetActiveTarget(log);

	m_pManager->SetMode(GAMMA_MODE_FILE_2_IMAGE);

}

MyFrame::~MyFrame()
{
	delete wxConfigBase::Set(NULL);
}

bool MyFrame::SetParam(GammaParam_e name, void* value)
{
	//todo
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MyFrame::OnCloseWindow(wxCloseEvent& event)
{
	GammaBlockManager::getInstance().SetMode(GAMMA_MODE_NONE);

	event.Skip();
}



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
		new GammaFrame();
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

void GammaFrameView::OnMouse(wxMouseEvent& event)
{
	wxMutexLocker locker(m_mouseMutex);
	wxString status;

	if (event.Dragging())
	{
		m_brightness += 0.001 * (event.GetX() - m_startX);
		m_contrast *= pow( 1.001, -(event.GetY() - m_startY) );
		//((GammaBlockTransMI*)m_block)->SetBC(m_brightness, m_contrast);
		status += "D";
	}
	if (event.LeftDown())
	{
		status += "Ld";
	}
	if (event.LeftUp())
	{
		status += "Lu";
	}

	m_startX = event.GetX();
	m_startY = event.GetY();

	wxString string0, string1;
	string0 << "Brightness = " << m_brightness << ", Contrast = " << m_contrast;
	GetStatusBar()->SetStatusText(string0, 0);
	string1 << "(" << (int)(m_startX/m_scaleX) << ", " << (int)(m_startY/m_scaleY) << ")" << status;
	GetStatusBar()->SetStatusText(string1, 1);

	event.Skip();
}

void GammaFrameView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxAutoBufferedPaintDC pdc(this);

	{
		wxMutexLocker locker(m_imageMutex);
		
		m_scaleX = (double)GetClientSize().GetWidth()/m_image.GetWidth();
		m_scaleY = (double)GetClientSize().GetHeight()/m_image.GetHeight();
		
		pdc.SetUserScale( min(m_scaleX, m_scaleY), min(m_scaleX, m_scaleY) );
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}
}

/*
void GammaFrameView::SetImage(wxImage& image)
{
	wxMutexLocker locker(m_imageMutex);
	m_image = image;

	Refresh(false);
}
*/