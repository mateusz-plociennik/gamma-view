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
#include "canvas.h"

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/slider.h>


// Define a new frame type
class GammaFrame : public wxFrame
{
public:
	GammaFrame();
	~GammaFrame();

	GammaManager* GetManager()
	{
		return m_pManager;
	}

	bool SetParam(GammaParam_e param, void* value);


private:
	void OnClose(wxCloseEvent& event);
	void OnMaximize(wxMaximizeEvent& event);
	void OnSize(wxSizeEvent& event);

	void OnMenuNewWindow(wxCommandEvent& event);
	void OnMenuCloseWindow(wxCommandEvent& event);

	void OnMenuResizeWindow(wxCommandEvent& event);

	void OnMenuSetColormap(wxCommandEvent& event);

	void OnMenuSetIntegrate(wxCommandEvent& event);

	void OnMenuHelpAbout(wxCommandEvent& event);

	void SetImage(wxImage& image);

	GammaManager* m_pManager;

	GammaCanvas *m_canvas;
	wxPanel *m_bottomPanel;

	wxBoxSizer *m_mainSizer;
	wxBoxSizer *m_bottomSizer;

	wxStaticText *m_timeNowLabel;
	wxSlider *m_bottomSlider;
	wxStaticText *m_timeEndLabel;

  DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
