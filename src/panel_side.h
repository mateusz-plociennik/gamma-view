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
	void onButton(wxCommandEvent& event);
	GammaManager* GetManager();

private:
	GammaFrame* m_frame;

	wxFlexGridSizer *m_sideSizer;

	wxDouble m_frequency;
	wxStaticText *m_frequencyLabel;
	wxStaticText *m_frequencyValue;

	wxStaticText *m_positionLabel;
	wxStaticText *m_positionValue;

	wxButton* m_setButton;

	DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_SIDE_PANEL_H_
