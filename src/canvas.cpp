/**
 * @file canvas.cpp
 * @brief Canvas for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#include "canvas.h"
#include "frame_view.h"

#include <wx/window.h>
#include <wx/gdicmn.h> // for wxSize
#include <wx/dcbuffer.h> // for wxAutoBufferedPaintDC

wxBEGIN_EVENT_TABLE(GammaCanvas, wxWindow)
	EVT_PAINT(GammaCanvas::OnPaint)
	EVT_SIZE(GammaCanvas::OnSize)
	EVT_LEFT_DOWN(GammaCanvas::OnMouse)
	EVT_LEFT_UP(GammaCanvas::OnMouse)
	EVT_MOTION(GammaCanvas::OnMouse)
wxEND_EVENT_TABLE()

GammaCanvas::GammaCanvas(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
		:
		wxWindow(parent, id, pos, size, style, name), 
		m_frame(parent),
		m_bestSize(256, 256),
		m_brightness(0.0),
		m_contrast(1.0),
		m_image(256, 256, true)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC
}

wxSize GammaCanvas::DoGetBestSize() const
{
	wxLogStatus("%s: (%d,%d)", __FUNCTION__, 
		m_bestSize.GetWidth(), 
		m_bestSize.GetHeight());
	return m_bestSize;
}

wxSize GammaCanvas::DoGetBestClientSize() const
{
	wxLogStatus("%s: (%d,%d)", __FUNCTION__, 
		m_bestSize.GetWidth(), 
		m_bestSize.GetHeight());
	return m_bestSize;
}

void GammaCanvas::DoSetBestSize(const wxSize& size)
{
	InvalidateBestSize();
	m_bestSize = size;
}

GammaManager* GammaCanvas::GetManager()
{
	return m_frame->GetManager();
}

void GammaCanvas::OnMouse(wxMouseEvent& event)
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
		m_frame->GetStatusBar()->SetStatusText(status, 0);
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
		status.Printf("(%.0f,%.0f)", (m_startX/m_scaleX), (m_startY/m_scaleY));
		m_frame->GetStatusBar()->SetStatusText(status, 1);
	}

	event.Skip();
}

void GammaCanvas::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC pdc(this);

	{
		wxMutexLocker locker(m_paintMutex);
		
		m_scaleX = (double)GetClientSize().GetWidth()/m_image.GetWidth();
		m_scaleY = (double)GetClientSize().GetHeight()/m_image.GetHeight();
		
		pdc.SetUserScale(m_scaleX,m_scaleY);
		pdc.DrawBitmap( wxBitmap(m_image), 0, 0 );
	}

	wxLogStatus("%s: (%d,%d)", __FUNCTION__, 
		GetClientSize().GetWidth(), 
		GetClientSize().GetHeight());

	event.Skip();
}

void GammaCanvas::OnSize(wxSizeEvent& event)
{
	Refresh(false);
	event.Skip();
}
