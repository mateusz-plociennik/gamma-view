/**
 * @file	f_end_cond.cpp
 * @brief	End condition selection
 * @author	Mateusz Plociennik
 * @data	2013-06-11
 */

#include "f_end_cond.h"
#include "block_mgmt.h"

#include <wx/statbox.h>
#include <wx/datetime.h>
#include <wx/log.h>

enum
{
	ID_CHECKBOX_AVG = 10000, 
	ID_CHECKBOX_MAX,
	ID_CHECKBOX_SUM,
	ID_CHECKBOX_TIME,
	ID_CHECKBOX_USER,
};

wxBEGIN_EVENT_TABLE(GammaEndCondDialog, wxDialog)
	EVT_CLOSE(GammaEndCondDialog::onClose)
	EVT_COMMAND_RANGE(ID_CHECKBOX_AVG, ID_CHECKBOX_TIME, wxEVT_COMMAND_CHECKBOX_CLICKED, GammaEndCondDialog::onCheckBox)
	//EVT_CHECKBOX(id, GammaEndCondDialog::onCheckBox):
	EVT_BUTTON(wxID_OK, GammaEndCondDialog::onButtonOk)
	EVT_BUTTON(wxID_CANCEL, GammaEndCondDialog::onButtonCancel)
wxEND_EVENT_TABLE()

GammaEndCondDialog::GammaEndCondDialog(GammaManager* pManager) 
	: wxDialog(NULL, wxID_ANY, wxT("Uniformity Matrix Acquisition"))
	, m_pManager(pManager)
{
	wxLogStatus("%s()", __FUNCTION__); 

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// Source

	wxStaticBox* sourceBox = new wxStaticBox(this, wxID_ANY, wxT("Source"));	
	wxStaticBoxSizer* sourceBoxSizer = new wxStaticBoxSizer(sourceBox, wxVERTICAL);
	
	wxFlexGridSizer* sourceBoxGridSizer = new wxFlexGridSizer(0, 3, wxSize(0, 0));
	sourceBoxGridSizer->AddGrowableCol(1);

	wxStaticText* sourceLabel = new wxStaticText(this, wxID_ANY, wxT("Energy: "));
	sourceBoxGridSizer->Add(sourceLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_energySpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("140"));
	m_energySpinCtrl->SetMax(511);
	sourceBoxGridSizer->Add(m_energySpinCtrl, 0, wxALL|wxEXPAND, 5);

	wxStaticText* unitLabel = new wxStaticText(this, wxID_ANY, wxT("keV"));
	sourceBoxGridSizer->Add(unitLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sourceBoxSizer->Add(sourceBoxGridSizer, 0, wxALL|wxEXPAND, 5);
	mainSizer->Add(sourceBoxSizer, 0, wxALL|wxEXPAND, 5);

	// End Condition

	wxStaticBox* endBox = new wxStaticBox(this, wxID_ANY, wxT("End Condition"));
	wxStaticBoxSizer* endBoxSizer = new wxStaticBoxSizer(endBox, wxVERTICAL);

	//// Event Count

	wxStaticBox* eventBox = new wxStaticBox(this, wxID_ANY, wxT("Event Count"));
	wxStaticBoxSizer* eventBoxSizer = new wxStaticBoxSizer(eventBox, wxVERTICAL);
		
	wxFlexGridSizer* endBoxGridSizer = new wxFlexGridSizer(0, 2, wxSize(0, 0));
	endBoxGridSizer->AddGrowableCol(1);

	m_avgCheckBox = new wxCheckBox(this, ID_CHECKBOX_AVG, wxT("Average: "));
	m_avgCheckBox->SetValue(false);
	endBoxGridSizer->Add(m_avgCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_avgSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("1000"));
	m_avgSpinCtrl->SetMax(wxINT32_MAX - 1);
	m_avgSpinCtrl->Enable(m_avgCheckBox->IsChecked());
	endBoxGridSizer->Add(m_avgSpinCtrl, 0, wxALL|wxEXPAND, 5);

	m_maxCheckBox = new wxCheckBox(this, ID_CHECKBOX_MAX, wxT("Maximum: "));
	m_maxCheckBox->SetValue(true);
	endBoxGridSizer->Add(m_maxCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("1000"));
	m_maxSpinCtrl->SetMax(wxINT32_MAX - 1);
	m_maxSpinCtrl->Enable(m_maxCheckBox->IsChecked());
	endBoxGridSizer->Add(m_maxSpinCtrl, 0, wxALL|wxEXPAND, 5);

	m_sumCheckBox = new wxCheckBox(this, ID_CHECKBOX_SUM, wxT("Sum: "));
	m_sumCheckBox->SetValue(false);
	endBoxGridSizer->Add(m_sumCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_sumSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("25000000"));
	m_sumSpinCtrl->SetMax(wxINT32_MAX - 1);
	m_sumSpinCtrl->Enable(m_sumCheckBox->IsChecked());
	endBoxGridSizer->Add(m_sumSpinCtrl, 0, wxALL|wxEXPAND, 5);

	wxStaticText* areaLabel = new wxStaticText(this, wxID_ANY, wxT("Area: "));
	endBoxGridSizer->Add(areaLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_areaChoice = new wxChoice(this, wxID_ANY);
	m_areaChoice->Insert(wxT("Field of View (FOV)"), GAMMA_AREA_FOV);
	m_areaChoice->Insert(wxT("Useful Field of View (UFOV)"), GAMMA_AREA_UFOV);
	m_areaChoice->Insert(wxT("Central Field of View (CFOV)"), GAMMA_AREA_CFOV);
	m_areaChoice->Select(GAMMA_AREA_CFOV);
	endBoxGridSizer->Add(m_areaChoice, 0, wxALL|wxEXPAND, 5);
	
	eventBoxSizer->Add(endBoxGridSizer, 0, wxALL|wxEXPAND, 5);
	endBoxSizer->Add(eventBoxSizer, 0, wxALL|wxEXPAND, 5);

	//// Other

	wxStaticBox* otherBox = new wxStaticBox(this, wxID_ANY, wxT("Other"));
	wxStaticBoxSizer* otherBoxSizer = new wxStaticBoxSizer(otherBox, wxVERTICAL);

	wxFlexGridSizer* otherBoxGridSizer = new wxFlexGridSizer(0, 2, wxSize(0, 0));
	otherBoxGridSizer->AddGrowableCol(1);

	m_timeCheckBox = new wxCheckBox(this, ID_CHECKBOX_TIME, wxT("Time Span: "));
	m_timeCheckBox->SetValue(false);
	otherBoxGridSizer->Add(m_timeCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_timePickerCtrl = new wxTimePickerCtrl(this, wxID_ANY, wxDateTime(0, 0));
	m_timePickerCtrl->Enable(m_timeCheckBox->IsChecked());
	otherBoxGridSizer->Add(m_timePickerCtrl, 0, wxALL|wxALIGN_RIGHT, 5);

	m_gateCheckBox = new wxCheckBox(this, ID_CHECKBOX_AVG, wxT("Gate: "));
	m_gateCheckBox->SetValue(false);
	otherBoxGridSizer->Add(m_gateCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gateSpinCtrl = new wxSpinCtrl(this, wxID_ANY, wxT("1000"));
	m_gateSpinCtrl->SetMax(wxINT32_MAX - 1);
	m_gateSpinCtrl->Enable(m_avgCheckBox->IsChecked());
	otherBoxGridSizer->Add(m_gateSpinCtrl, 0, wxALL|wxEXPAND, 5);

	m_userCheckBox = new wxCheckBox(this, ID_CHECKBOX_USER, wxT("User Stop"));
	m_userCheckBox->SetValue(true);
	m_userCheckBox->Enable(false);
	otherBoxGridSizer->Add(m_userCheckBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	otherBoxSizer->Add(otherBoxGridSizer, 0, wxALL|wxEXPAND, 5);
	endBoxSizer->Add(otherBoxSizer, 0, wxALL|wxEXPAND, 5);
	mainSizer->Add(endBoxSizer, 0, wxALL|wxEXPAND, 5);

	wxStdDialogButtonSizer* sdbSizer = new wxStdDialogButtonSizer();
	sdbSizer->AddButton(new wxButton(this, wxID_OK));
	sdbSizer->AddButton(new wxButton(this, wxID_CANCEL));
	sdbSizer->Realize();
	
	mainSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);
	
	this->SetSizerAndFit(mainSizer);

	Show();
}

GammaEndCondDialog::~GammaEndCondDialog()
{
	wxLogStatus("%s()", __FUNCTION__); 
}

void GammaEndCondDialog::onClose(wxCloseEvent& WXUNUSED(event))
{
	wxLogStatus("%s()", __FUNCTION__); 
	this->Destroy();
}

void GammaEndCondDialog::onCheckBox(wxCommandEvent& event)
{
	wxLogStatus("%s()", __FUNCTION__); 
	
	switch(event.GetId())
	{
	case ID_CHECKBOX_AVG:
		m_avgSpinCtrl->Enable(event.IsChecked()); break;
	case ID_CHECKBOX_MAX:
		m_maxSpinCtrl->Enable(event.IsChecked()); break;
	case ID_CHECKBOX_SUM:
		m_sumSpinCtrl->Enable(event.IsChecked()); break;
	case ID_CHECKBOX_TIME:
		m_timePickerCtrl->Enable(event.IsChecked()); break;
	default:
		break;
	}
}

void GammaEndCondDialog::onButtonCancel(wxCommandEvent& WXUNUSED(event))
{
	wxLogStatus("%s()", __FUNCTION__); 
	this->Close(true);
}

void GammaEndCondDialog::onButtonOk(wxCommandEvent& WXUNUSED(event))
{
	wxLogStatus("%s()", __FUNCTION__); 

	if(m_avgCheckBox->IsChecked())
	{
		wxUint32 avgTrig = m_avgSpinCtrl->GetValue();
		getManager()->DataTierSetParam(GAMMA_PARAM_TRIG_AVG, &avgTrig);
	}
	if(m_maxCheckBox->IsChecked())
	{
		wxUint32 maxTrig = m_maxSpinCtrl->GetValue();
		getManager()->DataTierSetParam(GAMMA_PARAM_TRIG_MAX, &maxTrig);
	}
	if(m_sumCheckBox->IsChecked())
	{
		wxUint64 sumTrig = m_sumSpinCtrl->GetValue();
		getManager()->DataTierSetParam(GAMMA_PARAM_TRIG_SUM, &sumTrig);
	}
	if(m_timeCheckBox->IsChecked())
	{
		wxTimeSpan timeTrig = m_timePickerCtrl->GetValue() - wxDateTime(0, 0, 0, 0);
		getManager()->DataTierSetParam(GAMMA_PARAM_TRIG_TIME, &timeTrig);
	}
	if(m_gateCheckBox->IsChecked())
	{
		wxUint32 gateTrig = m_gateSpinCtrl->GetValue();
		getManager()->DataTierSetParam(GAMMA_PARAM_TRIG_GATE, &gateTrig);
	}

	this->Close(true);
}