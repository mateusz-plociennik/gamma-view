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

class GammaBlockTransMI : 
	public GammaBlockBase
{
public:
	GammaBlockTransMI(GammaManager* pManager);
	bool SetParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
	void SetColour(GammaColormap_e colormap, unsigned int index, unsigned int max);

	double m_brightness;
	double m_contrast;
	double m_gamma;
	
	wxColour m_colour;
	GammaColormap_e m_colormap;
	bool m_bInvert;

};

#endif //_GAMMA_VIEW_BLOCK_TRANS_MI_H_
