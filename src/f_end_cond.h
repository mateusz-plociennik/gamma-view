/**
 * @file	f_end_cond.h
 * @brief	End condition selection
 * @author	Mateusz Plociennik
 * @data	2013-06-11
 */

#ifndef _GAMMA_VIEW_FRAME_END_CONDITION_H_
#define _GAMMA_VIEW_FRAME_END_CONDITION_H_

#include <wx/dialog.h>
#include "block_mgmt.h"

#include <wx/sizer.h>

#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/timectrl.h>

#include <wx/stattext.h>
#include <wx/choice.h>

#include <wx/button.h>


// Define a new frame type
class GammaEndCondDialog : public wxDialog
{

public:
	GammaEndCondDialog(GammaManager* pManager);
	~GammaEndCondDialog();

	GammaManager* getManager()
	{
		return m_pManager;
	}

private:
	void onClose(wxCloseEvent& event);
	void onMaximize(wxMaximizeEvent& event);
	void onCheckBox(wxCommandEvent& event);
	void onButtonCancel(wxCommandEvent& event);
	void onButtonOk(wxCommandEvent& event);
	

	GammaManager* m_pManager;

	wxSpinCtrl* m_energySpinCtrl;

	wxCheckBox* m_avgCheckBox;
	wxSpinCtrl* m_avgSpinCtrl;

	wxCheckBox* m_maxCheckBox;
	wxSpinCtrl* m_maxSpinCtrl;

	wxCheckBox* m_sumCheckBox;
	wxSpinCtrl* m_sumSpinCtrl;

	wxChoice* m_areaChoice;

	wxCheckBox* m_timeCheckBox;
	wxTimePickerCtrl* m_timePickerCtrl;

	wxCheckBox* m_gateCheckBox;
	wxSpinCtrl* m_gateSpinCtrl;

	wxCheckBox* m_userCheckBox;

	DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_FRAME_END_CONDITION_H_
