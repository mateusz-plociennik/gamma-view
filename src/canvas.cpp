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
#include <wx/cursor.h>
#include <wx/log.h>

wxBEGIN_EVENT_TABLE(GammaCanvas, wxWindow)
	EVT_PAINT(GammaCanvas::OnPaint)
	EVT_SIZE(GammaCanvas::OnSize)

	EVT_LEAVE_WINDOW(GammaCanvas::OnMouse)
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
		wxPanel(parent, id, pos, size, style, name), 
		m_frame(parent),
		m_bestSize(256, 256),
		m_brightness(0.0),
		m_contrast(1.0),
		m_gamma(1.0),
		m_image(256, 256, true)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT); // for wxAutoBufferedPaintDC
}

wxSize GammaCanvas::DoGetBestSize() const
{
	wxLogStatus(__FUNCTION__);
	return m_bestSize;
}

wxSize GammaCanvas::DoGetBestClientSize() const
{
	wxLogStatus(__FUNCTION__);
	return m_bestSize;
}

void GammaCanvas::DoSetBestSize(const wxSize& size)
{
	wxLogStatus(__FUNCTION__);
	InvalidateBestSize();
	m_bestSize = size;
}

GammaManager* GammaCanvas::getManager()
{
	return m_frame->getManager();
}

void GammaCanvas::OnMouse(wxMouseEvent& event)
{
	wxMutexLocker locker(m_mouseMutex);

	if( event.Dragging() )
	{
		if( event.LeftIsDown() )
		{
			m_brightness += ( 0.001 * (event.GetX() - m_startX) );
			getManager()->DataTierSetParam(GAMMA_PARAM_IMG_BRIGHTNESS, &m_brightness);

			m_contrast *= pow( 1.001, -(event.GetY() - m_startY) );
			getManager()->DataTierSetParam(GAMMA_PARAM_IMG_CONTRAST, &m_contrast);
		}
		else
		{
			m_gamma *= pow( 1.001, -(event.GetY() - m_startY) );
			getManager()->DataTierSetParam(GAMMA_PARAM_IMG_GAMMA, &m_gamma);
		}

		//wxString status;
		//status.Printf("B=%.2f; C=%.2f; G=%.2f", m_brightness, m_contrast, m_gamma);
		//m_frame->GetStatusBar()->SetStatusText(status, 0);
		m_frame->m_sidePanel->m_brightnessValue->SetLabel(wxString::Format("%.2f", m_brightness));
		m_frame->m_sidePanel->m_contrastValue->SetLabel(wxString::Format("%.2f", m_contrast));
		m_frame->m_sidePanel->m_gammaValue->SetLabel(wxString::Format("%.2f", m_gamma));
	}

	/*
	if( event.LeftDown() )
	{
		//status += "Ld";
	}
	if( event.LeftUp() )
	{
		//status += "Lu";
	}
	*/

	m_startX = event.GetX();
	m_startY = event.GetY();

	if(!event.Leaving())
	{
		m_frame->m_sidePanel->m_positionValue->SetLabel(
			wxString::Format("%03.0f, %03.0f", (m_startX/m_scaleXY), (m_startY/m_scaleXY)));

		wxWindow::SetCursor(wxCursor(wxCURSOR_CROSS));
	}
	else
	{
		m_frame->m_sidePanel->m_positionValue->SetLabel(wxT("888, 888"));

		wxWindow::SetCursor(wxNullCursor);
	}

	event.Skip();
}

void GammaCanvas::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC pdc(this);

	{
		wxMutexLocker locker(m_paintMutex);
		
		m_scaleX = (double)GetClientSize().GetWidth() / m_image.GetWidth();
		m_scaleY = (double)GetClientSize().GetHeight() / m_image.GetHeight();
		m_scaleXY = std::min(m_scaleX, m_scaleY);
		
		pdc.SetUserScale(m_scaleXY, m_scaleXY);
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}

	event.Skip();
}

void GammaCanvas::OnSize(wxSizeEvent& event)
{
	Refresh(false);
	event.Skip();
}

