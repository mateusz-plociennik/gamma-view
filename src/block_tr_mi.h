/**
 * @file	block_tr_mi.h
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_MI_H_
#define _GAMMA_VIEW_BLOCK_TRANS_MI_H_

#include "block_base.h"
#include <wx/image.h>
#include "frame_view.h"

class GammaBlockTransMI : 
	public GammaBlockBase
{
public:
	GammaBlockTransMI(wxWindow* parent);

	wxImage* GetImage();

protected:
	wxThread::ExitCode Entry();

private:
	//wxImage* m_image;

	GammaFrameView m_frame;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_MI_H_
