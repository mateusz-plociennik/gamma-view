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
	EVT_LEFT_UP(GammaFrameView::OnMouse)
	EVT_MOTION(GammaFrameView::OnMouse)
wxEND_EVENT_TABLE()

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
	wxString status;

	if (event.Dragging())
	{
		m_brightness += 0.001 * (event.GetX() - m_startX);
		m_contrast *= pow( 1.001, -(event.GetY() - m_startY) );
		((GammaBlockTransMI*)m_block)->SetBC(m_brightness, m_contrast);
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
		pdc.SetUserScale(m_scaleX, m_scaleY);
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}
}

void GammaFrameView::SetImage(wxImage& image)
{
	wxMutexLocker locker(m_imageMutex);
	m_image = image;

	Refresh(false);
}
