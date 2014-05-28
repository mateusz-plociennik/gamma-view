/**
 * @file panel_player.cpp
 * @brief Player Panel for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#include "panel_player.h"
#include "frame_view.h"

#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>

enum
{
	ID_SLIDER = 100,
};

wxBEGIN_EVENT_TABLE(GammaPlayerPanel, wxPanel)
	EVT_PAINT(GammaPlayerPanel::onPaint)
	EVT_SLIDER(ID_SLIDER, GammaPlayerPanel::onSliderUpdate)
wxEND_EVENT_TABLE()

GammaPlayerPanel::GammaPlayerPanel(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
		:
		wxPanel(parent, id, pos, size, wxBORDER_THEME), 
		m_frame(parent)
{
	wxUnusedVar(style);
	wxUnusedVar(name);

	m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	
	m_timeNowLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeNowLabel, 0, wxALIGN_CENTER|wxALL, 2);
	m_bottomSlider = new wxSlider(this, ID_SLIDER, 0, 0, 100);
	m_bottomSizer->Add(m_bottomSlider, 1, wxEXPAND|wxALIGN_CENTER);
	m_timeEndLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeEndLabel, 0, wxALIGN_CENTER|wxALL, 2);
	
	SetSizerAndFit(m_bottomSizer);
}

GammaManager* GammaPlayerPanel::getManager()
{
	return m_frame->getManager();
}

void GammaPlayerPanel::onPaint(wxPaintEvent& WXUNUSED(event))
{
	m_timeNowLabel->SetLabel(m_timeNow.Format("%H:%M:%S,%l"));
	m_bottomSlider->SetValue(m_timeNow.GetValue().GetLo());

	m_timeEndLabel->SetLabel(m_timeEnd.Format("%H:%M:%S,%l"));
	m_bottomSlider->SetMax(m_timeEnd.GetValue().GetLo());
}

void GammaPlayerPanel::onSliderUpdate(wxCommandEvent& WXUNUSED(event))
{
	wxTimeSpan reqTime(0, 0, 0, m_bottomSlider->GetValue());

	//wxLogStatus("%s: bottomSlider = %d", __FUNCTION__, m_bottomSlider->GetValue());
	m_timeNowLabel->SetLabel(reqTime.Format("%H:%M:%S,%l"));

	getManager()->DataTierSetParam(GAMMA_PARAM_FILE_SET_TIME, &reqTime);
}
