/**
 * @file	frame_view.h
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-08-11
 */

#ifndef _GAMMA_VIEW_FRAME_VIEW_H_
#define _GAMMA_VIEW_FRAME_VIEW_H_

#include <wx/frame.h>
#include <wx/image.h>
#include <wx/thread.h>


class GammaFrameView : 
	public wxFrame
{
public:
	GammaFrameView(	wxWindow *parent, 
		wxWindowID id, 
		const wxString &title, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxDEFAULT_FRAME_STYLE, 
		const wxString &name=wxFrameNameStr );

	void SetImage(wxImage& image);

protected:

private:
	void OnPaint(wxPaintEvent &event);

	wxImage m_image;
	wxMutex m_imageMutex;

	wxDECLARE_EVENT_TABLE();

};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
