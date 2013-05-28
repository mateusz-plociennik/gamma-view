/**
 * @file canvas.h
 * @brief Canvas for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#ifndef _GAMMA_VIEW_PLAYER_PANEL_H_
#define _GAMMA_VIEW_PLAYER_PANEL_H_

#include <wx/panel.h>
#include <wx/window.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "block_mgmt.h"

//#include "frame_view.h" Breaking cyclic dependency
class GammaFrame;

class GammaPlayerPanel : public wxPanel
{
	friend class GammaFrame;

public:
	GammaPlayerPanel(GammaFrame *parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

protected:
	GammaManager* getManager();
	void onPaint(wxPaintEvent& event);
	void onSliderUpdate(wxCommandEvent& event);

private:
	GammaFrame* m_frame;

	wxBoxSizer *m_bottomSizer;

	wxStaticText *m_timeNowLabel;
	wxTimeSpan m_timeNow;

	wxSlider *m_bottomSlider;

	wxStaticText *m_timeEndLabel;
	wxTimeSpan m_timeEnd;

  DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_PLAYER_PANEL_H_
