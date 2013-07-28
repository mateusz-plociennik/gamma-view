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
wxEND_EVENT_TABLE()

wxString formatReadableNumber(wxDouble value, bool bInteger)
{
	static const char siPrefix[] = { '\0', 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' };
	static const wxInt32 digits = 4;

	wxInt32 position, precision = 0;

	for(position = 0; 1 <= value / pow(10.0, position + 1); position++);

	if(!bInteger || 2 < position)
	{
		precision = (digits - 1) - position % 3;
	}
 
	return wxString::Format("%.*f%c", precision, 
		value / pow(1000.0, position / 3), siPrefix[position / 3]);
}

GammaSidePanel::GammaSidePanel(GammaFrame *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
		: wxPanel(parent, id, pos, size, style, name)
		, m_frame(parent)
{
	UNREFERENCED_PARAMETER(style);
	UNREFERENCED_PARAMETER(name);

	wxFlexGridSizer* sideSizer = new wxFlexGridSizer(0, 3, wxSize(0, 0));
	
	wxStaticText* frequencyLabel = new wxStaticText(this, wxID_ANY, _("Frequency:"));
	sideSizer->Add(frequencyLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_frequencyValue = new wxStaticText(this, wxID_ANY, formatReadableNumber(8888888, false), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_frequencyValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* frequencyUnit = new wxStaticText(this, wxID_ANY, _("/ s"));
	sideSizer->Add(frequencyUnit, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticText* eventAvgLabel = new wxStaticText(this, wxID_ANY, _("Average:"));
	sideSizer->Add(eventAvgLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_eventAvgValue = new wxStaticText(this, wxID_ANY, formatReadableNumber(8888888, false), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_eventAvgValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* eventAvgUnit = new wxStaticText(this, wxID_ANY, _("events"));
	sideSizer->Add(eventAvgUnit, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticText* eventMaxLabel = new wxStaticText(this, wxID_ANY, _("Maximum:"));
	sideSizer->Add(eventMaxLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_eventMaxValue = new wxStaticText(this, wxID_ANY, formatReadableNumber(8888888, true), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_eventMaxValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* eventMaxUnit = new wxStaticText(this, wxID_ANY, _("events"));
	sideSizer->Add(eventMaxUnit, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticText* eventSumLabel = new wxStaticText(this, wxID_ANY, _("Sum:"));
	sideSizer->Add(eventSumLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_eventSumValue = new wxStaticText(this, wxID_ANY, formatReadableNumber(8888888, true), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_eventSumValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* eventSumUnit = new wxStaticText(this, wxID_ANY, _("events"));
	sideSizer->Add(eventSumUnit, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticText* positionLabel = new wxStaticText(this, wxID_ANY, _("Position:"));
	sideSizer->Add(positionLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_positionValue = new wxStaticText(this, wxID_ANY, wxT("888, 888"), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_positionValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* positionUnit = new wxStaticText(this, wxID_ANY, wxT("px"));
	sideSizer->Add(positionUnit, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticText* countLabel = new wxStaticText(this, wxID_ANY, _("Count:"));
	sideSizer->Add(countLabel, 0, wxALIGN_RIGHT|wxALL, 5);
	m_countValue = new wxStaticText(this, wxID_ANY, formatReadableNumber(8888888, true), 
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	sideSizer->Add(m_countValue, 0, wxALIGN_RIGHT|wxALL, 5);
	wxStaticText* countUnit = new wxStaticText(this, wxID_ANY, wxT("events"));
	sideSizer->Add(countUnit, 0, wxALIGN_LEFT|wxALL, 5);

	SetSizerAndFit(sideSizer);
}


void GammaSidePanel::onPaint(wxPaintEvent& WXUNUSED(event))
{
	m_frequencyValue->SetLabel(formatReadableNumber(m_frequency, false));
	m_eventAvgValue->SetLabel(formatReadableNumber(m_eventAvg, false));
	m_eventMaxValue->SetLabel(formatReadableNumber(m_eventMax, true));
	m_eventSumValue->SetLabel(formatReadableNumber(m_eventSum, true));
}

GammaManager* GammaSidePanel::getManager()
{
	return m_frame->getManager();
}
