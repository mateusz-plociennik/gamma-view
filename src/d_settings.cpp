/**
 * @file	d_settings.cpp
 * @brief	USB Settings Dialog
 * @author	Mateusz Plociennik
 * @data	2013-07-01
 */

#include "d_settings.h"
#include "data_types.h"
#include "frame_view.h"

#include <wx/log.h>
#include <wx/statbox.h>

enum
{
	ID_GAMMA_SET_ZOOM = 10100, 
	ID_GAMMA_SET_SHIFT_X,
	ID_GAMMA_SET_SHIFT_Y,
	ID_GAMMA_SET_TMARKER,
	ID_GAMMA_SET_GATE,
};

BEGIN_EVENT_TABLE(GammaUsbSettingsDialog, wxDialog)
	EVT_CLOSE(GammaUsbSettingsDialog::onClose)
	EVT_SPINCTRL(ID_GAMMA_SET_ZOOM, GammaUsbSettingsDialog::onSpinCtrl)
	EVT_SPINCTRL(ID_GAMMA_SET_SHIFT_X, GammaUsbSettingsDialog::onSpinCtrl)
	EVT_SPINCTRL(ID_GAMMA_SET_SHIFT_Y, GammaUsbSettingsDialog::onSpinCtrl)
	EVT_CHOICE(ID_GAMMA_SET_TMARKER, GammaUsbSettingsDialog::onChoice)
	EVT_CHOICE(ID_GAMMA_SET_GATE, GammaUsbSettingsDialog::onChoice)
	EVT_BUTTON(wxID_OK, GammaUsbSettingsDialog::onButtonOk)
	EVT_BUTTON(wxID_CANCEL, GammaUsbSettingsDialog::onButtonCancel)
END_EVENT_TABLE()

GammaUsbSettingsDialog::GammaUsbSettingsDialog(GammaFrame* pParent) 
	: wxDialog(pParent, wxID_ANY, _("USB Settings"))
{
	wxLogStatus("%s()", __FUNCTION__); 

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// USB Settings

	wxStaticBox* settingsBox = new wxStaticBox(this, wxID_ANY, _("USB Settings"));	
	wxStaticBoxSizer* settingsBoxSizer = new wxStaticBoxSizer(settingsBox, wxVERTICAL);
	
	wxFlexGridSizer* settingsBoxGridSizer = new wxFlexGridSizer(0, 2, wxSize(0, 0));
	settingsBoxGridSizer->AddGrowableCol(1);


	wxStaticText* zoomLabel = new wxStaticText(this, wxID_ANY, wxT("Zoom: "));
	settingsBoxGridSizer->Add(zoomLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_zoomSpinCtrl = new wxSpinCtrl(this, ID_GAMMA_SET_ZOOM, wxT("127"));
	m_zoomSpinCtrl->SetMax(255);
	settingsBoxGridSizer->Add(m_zoomSpinCtrl, 0, wxALL|wxEXPAND, 5);


	wxStaticText* shiftXLabel = new wxStaticText(this, wxID_ANY, wxT("Shift X: "));
	settingsBoxGridSizer->Add(shiftXLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_shiftXSpinCtrl = new wxSpinCtrl(this, ID_GAMMA_SET_SHIFT_X, wxT("127"));
	m_shiftXSpinCtrl->SetMax(255);
	settingsBoxGridSizer->Add(m_shiftXSpinCtrl, 0, wxALL|wxEXPAND, 5);


	wxStaticText* shiftYLabel = new wxStaticText(this, wxID_ANY, wxT("Shift Y: "));
	settingsBoxGridSizer->Add(shiftYLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_shiftYSpinCtrl = new wxSpinCtrl(this, ID_GAMMA_SET_SHIFT_Y, wxT("127"));
	m_shiftYSpinCtrl->SetMax(255);
	settingsBoxGridSizer->Add(m_shiftYSpinCtrl, 0, wxALL|wxEXPAND, 5);

	
	wxStaticText* timeMarkerLabel = new wxStaticText(this, wxID_ANY, wxT("Time Marker: "));
	settingsBoxGridSizer->Add(timeMarkerLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_timeMarkerChoice = new wxChoice(this, ID_GAMMA_SET_TMARKER);
	m_timeMarkerChoice->Insert(wxT("Off"), GAMMA_TMARKER_OFF);
	m_timeMarkerChoice->Insert(wxT("1 ms"), GAMMA_TMARKER_1MS);
	m_timeMarkerChoice->Insert(wxT("10 ms"), GAMMA_TMARKER_10MS);
	m_timeMarkerChoice->Insert(wxT("100 ms"), GAMMA_TMARKER_100MS);
	m_timeMarkerChoice->Insert(wxT("240 ms"), GAMMA_TMARKER_240MS);
	m_timeMarkerChoice->Select(GAMMA_TMARKER_10MS);
	settingsBoxGridSizer->Add(m_timeMarkerChoice, 0, wxALL|wxEXPAND, 5);


	wxStaticText* gateLabel = new wxStaticText(this, wxID_ANY, wxT("Gate: "));
	settingsBoxGridSizer->Add(gateLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_gateChoice = new wxChoice(this, ID_GAMMA_SET_GATE);
	m_gateChoice->Insert(wxT("Off"), 0);
	m_gateChoice->Insert(wxT("On"), 1);
	m_gateChoice->Select(0);
	settingsBoxGridSizer->Add(m_gateChoice, 0, wxALL|wxEXPAND, 5);

	settingsBoxSizer->Add(settingsBoxGridSizer, 0, wxALL|wxEXPAND, 5);
	mainSizer->Add(settingsBoxSizer, 0, wxALL|wxEXPAND, 5);


	wxStdDialogButtonSizer* sdbSizer = new wxStdDialogButtonSizer();
	sdbSizer->AddButton(new wxButton(this, wxID_OK));
	sdbSizer->AddButton(new wxButton(this, wxID_CANCEL));
	sdbSizer->Realize();
	
	mainSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);
	
	this->SetSizerAndFit(mainSizer);

	Show();
}

GammaUsbSettingsDialog::~GammaUsbSettingsDialog()
{
}

void GammaUsbSettingsDialog::onClose(wxCloseEvent& WXUNUSED(event))
{
	this->Destroy();
}

void GammaUsbSettingsDialog::onButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	wxLogStatus("%s()", __FUNCTION__); 
	this->Close(true);
}

void GammaUsbSettingsDialog::onButtonOk(wxCommandEvent& WXUNUSED(event))
{
	wxLogStatus("%s()", __FUNCTION__); 

	// TODO

	this->Close(true);
}

void GammaUsbSettingsDialog::onSpinCtrl(wxSpinEvent& event)
{
	wxUint8 value = event.GetValue();
/*
	switch(event.GetId())
	{
	case ID_GAMMA_SET_ZOOM:
		getManager()->DataTierSetParam(GAMMA_PARAM_SET_ZOOM, &value);
		break;
	case ID_GAMMA_SET_SHIFT_X:
		getManager()->DataTierSetParam(GAMMA_PARAM_SET_SHIFT_X, &value);
		break;
	case ID_GAMMA_SET_SHIFT_Y:
		getManager()->DataTierSetParam(GAMMA_PARAM_SET_SHIFT_Y, &value);
		break;
	default:
		return;
	}
	*/
}

void GammaUsbSettingsDialog::onChoice(wxCommandEvent& event)
{
	wxLogStatus("%s()", __FUNCTION__); 

/*	

	switch(event.GetId())
	{
	case ID_GAMMA_SET_TMARKER:
		{
			GammaSettingTmarker_e tMarker = static_cast<GammaSettingTmarker_e>(event.GetSelection());
			getManager()->DataTierSetParam(GAMMA_PARAM_SET_TMARKER, &tMarker);
			break;
		}
	case ID_GAMMA_SET_GATE:
		{
			wxUint8 value = event.GetSelection();
			getManager()->DataTierSetParam(GAMMA_PARAM_SET_GATE, &value);
		}
		break;
	default:
		return;
	}
	*/
}