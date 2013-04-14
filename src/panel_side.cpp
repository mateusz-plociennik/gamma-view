/**
 * @file panel_player.cpp
 * @brief Player Panel for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#include "panel_side.h"
#include "frame_view.h"

#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>

enum
{
	//ID_SLIDER = 100,
};

wxBEGIN_EVENT_TABLE(GammaSidePanel, wxPanel)
	//EVT_SLIDER(ID_SLIDER, GammaSidePanel::OnSliderUpdate)
wxEND_EVENT_TABLE()

GammaSidePanel::GammaSidePanel(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
		:
		wxPanel(parent, id, pos, size, style, name), 
		m_frame(parent)
{
	m_sideSizer = new wxBoxSizer(wxHORIZONTAL);
	
	m_timeNowLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_sideSizer->Add(m_timeNowLabel, 0, wxALIGN_CENTER|wxALL, 2);
	m_timeEndLabel = new wxStaticText(this, wxID_ANY, "00:00:00,000");
	m_sideSizer->Add(m_timeEndLabel, 0, wxALIGN_CENTER|wxALL, 2);
	
	SetSizerAndFit(m_sideSizer);
}

GammaManager* GammaSidePanel::GetManager()
{
	return m_frame->GetManager();
}


