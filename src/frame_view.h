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
#include "panel_player.h"
#include "panel_side.h"

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/slider.h>

#include <wx/aui/framemanager.h>


// Define a new frame type
class GammaFrame : public wxFrame
{
	friend class GammaCanvas;
	friend class GammaPlayerPanel;
	friend class GammaSidePanel;

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

	void OnMenuMode(wxCommandEvent& event);
	void OnMenuResizeWindow(wxCommandEvent& event);
	void OnMenuSetColourmap(wxCommandEvent& event);
	void OnMenuSetIntegrate(wxCommandEvent& event);
	void OnMenuSetImgParams(wxCommandEvent& event);

	void onMenuSettings(wxCommandEvent& event);

	void OnMenuHelpAbout(wxCommandEvent& event);

	void OnSliderUpdate(wxCommandEvent &event);

	void SetImage(wxImage& image);

	//
	wxString openFile();
	wxString saveFile();

protected:
	GammaManager* m_pManager;

	GammaCanvas* m_canvas;
	GammaPlayerPanel* m_bottomPanel;

	wxBoxSizer* m_centerSizer;

	GammaSidePanel* m_sidePanel;

	wxBoxSizer* m_horizontalSizer;

	wxAuiManager m_mgr;

  DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_FRAME_VIEW_H_
