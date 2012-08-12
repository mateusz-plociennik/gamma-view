/**
 * @file	frame_view.cpp
 * @brief	View Frame
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#include "frame_view.h"
#include <wx/dcclient.h>

wxBEGIN_EVENT_TABLE(GammaFrameView, wxFrame)
    EVT_PAINT(GammaFrameView::OnPaint)
wxEND_EVENT_TABLE()

GammaFrameView::GammaFrameView( wxWindow* parent, 
		wxWindowID id, 
		const wxString &title, 
		const wxPoint &pos, 
		const wxSize &size, 
		long style, 
		const wxString &name )
		: wxFrame (parent, id, title, pos, size, style, name)
		, m_image(256, 256, true)
{
	SetIcon(wxICON(gamma-view));
	CreateStatusBar();
	SetClientSize(512, 512);
	Show();
}

void GammaFrameView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC pdc(this);

	{
		wxMutexLocker locker(m_imageMutex);
		pdc.DrawBitmap(wxBitmap(m_image), 0, 0);
	}
}

void GammaFrameView::SetImage(wxImage& image)
{
	wxSize size = GetClientSize();

	{
		wxMutexLocker locker(m_imageMutex);
		m_image = image.Scale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_NEAREST);
	}

	Refresh(false);
}
