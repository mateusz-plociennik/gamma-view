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
#include "block_mgmt.h"

// Define a new frame type
class GammaFrame : public wxFrame
{
public:
	GammaFrame();
	~GammaFrame();

	bool SetParam(GammaParam_e param, void* value);

private:
	void OnClose(wxCloseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);

	void OnMenuNewWindow(wxCommandEvent& event);
	void OnMenuCloseWindow(wxCommandEvent& event);

	void OnMenuResizeWindow(wxCommandEvent& event);

	void OnMenuSetColormap(wxCommandEvent& event);

	void OnMenuSetIntegrate(wxCommandEvent& event);

	void OnMenuHelpAbout(wxCommandEvent& event);

	void SetImage(wxImage& image);

	GammaManager* GetManager()
	{
		return m_pManager;
	}

	GammaManager* m_pManager;

	int m_startX;
	int m_startY;

	double m_imgScale;

	wxMutex m_mouseMutex;

	bool m_fineScale;

	double m_brightness;
	double m_contrast;

	wxImage m_image;
	wxMutex m_imageMutex;

  DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
