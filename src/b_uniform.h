/**
 * @file	b_uniform.h
 * @brief	uniformity filter
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_UNIFORMITY_H_
#define _GAMMA_VIEW_BLOCK_UNIFORMITY_H_

#include "block_base.h"

class GammaUniformity : public GammaPipeSegment
{
public:
	GammaUniformity(GammaManager* pManager);

	void processData(wxSharedPtr<GammaData> pData);
	wxInt32 setParam(GammaParam_e param, void* value);

	void onThreadEvent(wxThreadEvent& event);

private:
	int corrCount(wxUint32 x, wxUint32 y);
	void setMatrix(wxUint32* matrix);

	wxInt32 m_matrix[256*256];
	wxInt32 m_r;

	bool m_bInitalized;

};

#endif //_GAMMA_VIEW_BLOCK_UNIFORMITY_H_
