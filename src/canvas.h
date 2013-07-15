/**
 * @file canvas.h
 * @brief Canvas for GammaView
 * @author Mateusz Plociennik
 * @data 2013-01-07
 */

#ifndef _GAMMA_VIEW_CANVAS_H_
#define _GAMMA_VIEW_CANVAS_H_

#include <wx/window.h>
#include <wx/panel.h>
#include <wx/gdicmn.h> // for wxSize
#include <wx/thread.h>
#include <wx/image.h>

#include "block_mgmt.h"

//#include "frame_view.h" Breaking cyclic dependency
class GammaFrame;

class GammaCanvas : public wxPanel //wxWindow
{
	friend class GammaFrame;

public:
	GammaCanvas(GammaFrame *parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);
	//~GammaCanavs();

	//bool SetParam(GammaParam_e param, void* value);
	virtual wxSize DoGetBestSize() const;
	virtual wxSize DoGetBestClientSize() const;
	void DoSetBestSize(const wxSize& size);
	GammaManager* GetManager();

private:
	void OnMouse(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);

	GammaFrame* m_frame;

	wxSize m_bestSize;

	// OnMouse variables
	wxMutex m_mouseMutex;
	wxDouble m_brightness;
	wxDouble m_contrast;
	wxDouble m_gamma;
	wxInt32 m_startX;
	wxInt32 m_startY;

	// OnPaint variables
	wxMutex m_paintMutex;
	wxImage m_image;
	wxDouble m_scaleX;
	wxDouble m_scaleY;
	wxDouble m_scaleXY;

  DECLARE_EVENT_TABLE();
};

#endif //_GAMMA_VIEW_CANVAS_H_
