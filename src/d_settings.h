/**
 * @file	d_settings.h
 * @brief	USB Settings Dialog
 * @author	Mateusz Plociennik
 * @data	2013-07-01
 */

#ifndef _GAMMA_VIEW_DIALOG_USB_SETTINGS_H_
#define _GAMMA_VIEW_DIALOG_USB_SETTINGS_H_

#include <wx/dialog.h>
#include "block_base.h"

#include <wx/sizer.h>

#include <wx/checkbox.h>
#include <wx/spinctrl.h>

#include <wx/stattext.h>
#include <wx/choice.h>

#include <wx/button.h>


// Define a new frame type
class GammaUsbSettingsDialog : public GammaPipeMgmt, public wxDialog
{

public:
	GammaUsbSettingsDialog(GammaManager* pManager);
	~GammaUsbSettingsDialog();

private:
	void onClose(wxCloseEvent& event);
	void onMaximize(wxMaximizeEvent& event);
	void onSpinCtrl(wxSpinEvent& event);
	void onChoice(wxCommandEvent& event);
	void onButtonCancel(wxCommandEvent& event);
	void onButtonOk(wxCommandEvent& event);

	wxSpinCtrl* m_zoomSpinCtrl;

	wxSpinCtrl* m_shiftXSpinCtrl;
	wxSpinCtrl* m_shiftYSpinCtrl;

	wxChoice* m_timeMarkerChoice;

	wxChoice* m_gateChoice;

	DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_DIALOG_USB_SETTINGS_H_
