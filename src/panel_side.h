/**
 * @file canvas.h
 * @brief Canvas for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#ifndef _GAMMA_VIEW_SIDE_PANEL_H_
#define _GAMMA_VIEW_SIDE_PANEL_H_

#include <wx/panel.h>
#include <wx/window.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>

#include "block_mgmt.h"

//#include "frame_view.h" Breaking cyclic dependency
class GammaFrame;

class GammaSidePanel : public wxPanel
{
	friend class GammaFrame;

	friend class GammaCanvas;
	friend class GammaPlayerPanel;

public:
	GammaSidePanel(GammaFrame *parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

protected:
	void onPaint(wxPaintEvent& event);
	GammaManager* GetManager();

private:
	GammaFrame* m_frame;

	wxDouble m_frequency;
	wxStaticText* m_frequencyValue;

	wxDouble m_eventAvg;
	wxStaticText* m_eventAvgValue;

	wxUint32 m_eventMax;
	wxStaticText* m_eventMaxValue;

	wxUint64 m_eventSum;
	wxStaticText* m_eventSumValue;
	
	GammaTrig_e m_trig;

	wxStaticText* m_positionValue;

	wxStaticText* m_countValue;

	DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_SIDE_PANEL_H_
