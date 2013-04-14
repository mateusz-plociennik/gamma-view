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

class GammaTransMI : public GammaPipeSegment
{
public:
	GammaTransMI(GammaManager* pManager);
	
	void processData(GammaDataBase* pDataIn);
	bool setParam(GammaParam_e param, void* value);

private:
	void setColor(wxUint32 index, wxUint32 max);

	double m_brightness;
	double m_contrast;
	double m_gamma;
	bool m_invert;
	
	GammaColormap_e m_colormap;
	wxColour m_color;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_MI_H_
