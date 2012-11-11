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


wxBEGIN_EVENT_TABLE(GammaFrameView, wxFrame)
	EVT_CLOSE(GammaFrameView::OnClose)
    EVT_PAINT(GammaFrameView::OnPaint)

	EVT_LEFT_DOWN(GammaFrameView::OnMouse)
	EVT_MOTION(GammaFrameView::OnMouse)
wxEND_EVENT_TABLE()

GammaFrameView::GammaFrameView( GammaBlockBase* block, 
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
{
	SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC

	SetIcon(wxICON(gamma-view));
	CreateStatusBar();
	GetStatusBar()->SetFieldsCount(2);
	SetClientSize(512, 512);
	Show();
}

void GammaFrameView::OnClose(wxCloseEvent& WXUNUSED(event))
{
	Hide();
	m_block->Pause();
	//Destroy();
}

void GammaFrameView::OnMouse(wxMouseEvent& event)
{
	wxMutexLocker locker(m_mouseMutex);
	if (event.LeftDown())
	{
		m_startX = event.GetX();
		m_startY = event.GetY();
	}
	if (event.Dragging())
	{
		m_brightness += 0.001 * (event.GetX() - m_startX);
		m_contrast *= pow( 1.001, (m_startY - event.GetY()) );
		((GammaBlockTransMI*)m_block)->SetBC(m_brightness, m_contrast);
		m_startX = event.GetX();
		m_startY = event.GetY();
	}

	wxString string0, string1;
	string0 << "Brightness = " << m_brightness << ", Contrast = " << m_contrast;
	GetStatusBar()->SetStatusText(string0, 0);
	string1 << "(" << event.GetX() << ", " << event.GetY() << ")";
	GetStatusBar()->SetStatusText(string1, 1);

	event.Skip();
}

void GammaFrameView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxAutoBufferedPaintDC pdc(this);

	{
		wxMutexLocker locker(m_imageMutex);
		
		pdc.SetUserScale( (double)GetClientSize().GetWidth()/m_image.GetWidth(), 
			(double)GetClientSize().GetHeight()/m_image.GetHeight() );
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}
}

void GammaFrameView::SetImage(wxImage& image)
{
	wxMutexLocker locker(m_imageMutex);
	m_image = image;

	Refresh(false);
}
