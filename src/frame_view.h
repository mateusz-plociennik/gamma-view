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

// Define a new frame type
class GammaFrame : public wxFrame
{
public:
	GammaFrame();
	~GammaFrame();

	bool SetParam(GammaParam_e name, void* value);

private:
	void OnClose(wxCloseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnHelpAbout(wxCommandEvent& event);
	void OnNewWindow(wxCommandEvent& event);
	void OnResizeWindow(wxCommandEvent& event);

	void SetImage(wxImage& image);

	GammaManager* m_pManager;

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

  DECLARE_EVENT_TABLE()
};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
