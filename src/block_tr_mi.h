/**
 * @file	block_tr_mi.h
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_MI_H_
#define _GAMMA_VIEW_BLOCK_TRANS_MI_H_

#include "block_base.h"
#include "block_mgmt.h"
#include "frame_view.h"
#include <wx/image.h>
#include <deque>
//#include <map>

class GammaTransMI : public GammaPipeSegment
{
public:
	GammaTransMI(GammaManager* pManager);
	
	void processData(GammaData* pDataIn);
	wxInt32 setParam(GammaParam_e param, void* value);

private:
	void calcColour(wxDouble index);
	void calcMax(wxUint32 eventMax);
	
	wxDouble m_brightness;
	wxDouble m_contrast;
	wxDouble m_gamma;
	wxDouble m_max;

	bool m_bInvert;
	bool m_bSetUniformMatrix;
	
	GammaColourmap_e m_colourmap;
	wxColour m_colour;

	std::deque<wxUint32> m_eventMaxTable;

//	std::map<wxUint64, wxColour> m_colourLut;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_MI_H_
