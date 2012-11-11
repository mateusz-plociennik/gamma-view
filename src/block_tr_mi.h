/**
 * @file	block_tr_mi.h
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_MI_H_
#define _GAMMA_VIEW_BLOCK_TRANS_MI_H_

#include "block_base.h"
#include "frame_view.h"
#include <wx/image.h>


class GammaBlockTransMI : 
	public GammaBlockBase
{
public:
	GammaBlockTransMI(wxWindow* parent);
	void SetBC(double b, double c);

protected:
	wxThread::ExitCode Entry();

private:
	void SetColour(unsigned char colormap, unsigned int index, unsigned int max);
	

	double m_brightness;
	double m_contrast;
	
	wxColour m_colour;
	unsigned char m_colorMap;
	bool m_bInvert;

	//wxImage m_image;
	GammaFrameView m_frame;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_MI_H_
