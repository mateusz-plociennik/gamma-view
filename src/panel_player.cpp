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
	EVT_SLIDER(ID_SLIDER, GammaPlayerPanel::OnSliderUpdate)
wxEND_EVENT_TABLE()

GammaPlayerPanel::GammaPlayerPanel(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
		:
		wxPanel(parent, id, pos, size, style, name), 
		m_frame(parent)
{
	m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	
	m_timeNowLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeNowLabel, 0, wxALIGN_CENTER|wxALL, 2);
	m_bottomSlider = new wxSlider(this, ID_SLIDER, 0, 0, 100);
	m_bottomSizer->Add(m_bottomSlider, 1, wxEXPAND|wxALIGN_CENTER);
	m_timeEndLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_bottomSizer->Add(m_timeEndLabel, 0, wxALIGN_CENTER|wxALL, 2);
	
	SetSizerAndFit(m_bottomSizer);
}

GammaManager* GammaPlayerPanel::GetManager()
{
	return m_frame->GetManager();
}

void GammaPlayerPanel::OnSliderUpdate(wxCommandEvent &WXUNUSED(event))
{
	wxString message;
	message.Printf("Slider = %d", m_bottomSlider->GetValue());
	wxLogStatus(message);
	m_timeNowLabel->SetLabel( 
		wxTimeSpan(0, 0, 0, m_bottomSlider->GetValue()).Format("%H:%M:%S,%l") );
}
