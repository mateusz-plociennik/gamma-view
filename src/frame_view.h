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
#include "block_base.h"



class GammaFrameView : 
	public wxFrame
{
public:
	GammaFrameView(	GammaBlockBase* block, 
		wxWindow *parent, 
		wxWindowID id, 
		const wxString &title, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxDEFAULT_FRAME_STYLE, 
		const wxString &name=wxFrameNameStr );

	void SetImage(wxImage& image);

protected:

private:
	void OnClose(wxCloseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);

	int m_startX;
	int m_startY;

	double m_scaleX;
	double m_scaleY;

	wxMutex m_mouseMutex;

	bool m_fineScale;

	double m_brightness;
	double m_contrast;

	wxImage m_image;
	wxMutex m_imageMutex;

	GammaBlockBase* m_block;

	wxDECLARE_EVENT_TABLE();

};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
