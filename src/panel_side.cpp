/**
 * @file panel_side.cpp
 * @brief Side Panel for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#include "panel_side.h"
#include "frame_view.h"

#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>
#include <wx/string.h>

enum
{
	ID_BUTTON = 100,
};

wxBEGIN_EVENT_TABLE(GammaSidePanel, wxPanel)
	EVT_PAINT(GammaSidePanel::onPaint)
	EVT_BUTTON(ID_BUTTON, GammaSidePanel::onButton)
wxEND_EVENT_TABLE()

GammaSidePanel::GammaSidePanel(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
	: wxPanel(parent, id, pos, size, wxBORDER_THEME, wxPanelNameStr)
	, m_frame(parent)
{
	m_sideSizer = new wxFlexGridSizer(0, 2, wxSize(2,2));
	
	m_frequencyLabel = new wxStaticText(this, wxID_ANY, wxT("Frequency:"));
	m_sideSizer->Add(m_frequencyLabel, 0, wxALIGN_RIGHT|wxALL, 2);
	m_frequencyValue = new wxStaticText(this, wxID_ANY, wxT("0,000e+001 / s"));
	m_sideSizer->Add(m_frequencyValue, 0, wxALIGN_LEFT|wxALL, 2);

	m_positionLabel = new wxStaticText(this, wxID_ANY, wxT("Position:"));
	m_sideSizer->Add(m_positionLabel, 0, wxALIGN_RIGHT|wxALL, 2);
	m_positionValue = new wxStaticText(this, wxID_ANY, wxT("000, 000px"));
	m_sideSizer->Add(m_positionValue, 0, wxALIGN_LEFT|wxALL, 2);

	m_setButton = new wxButton(this, ID_BUTTON, wxT("Set"));
	m_sideSizer->Add(m_setButton, 0, wxALIGN_CENTER|wxALL, 2);
	
	SetSizerAndFit(m_sideSizer);
}

void GammaSidePanel::onPaint(wxPaintEvent& WXUNUSED(event))
{
	wxString frequencyString;
	frequencyString.Printf(wxT("%.3e / s"), m_frequency);
	m_frequencyValue->SetLabel(frequencyString);
}

void GammaSidePanel::onButton(wxCommandEvent& WXUNUSED(event))
{
	wxLogStatus("Button");
	GetManager()->DataTierSetParam(GAMMA_PARAM_BUTTON_SET, NULL);
}

GammaManager* GammaSidePanel::GetManager()
{
	return m_frame->GetManager();
}


