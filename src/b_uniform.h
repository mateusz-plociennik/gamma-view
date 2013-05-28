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

	void processData(GammaData* pData);
	bool setParam(GammaParam_e param, void* value);

private:
	int corrCount(wxUint32 x, wxUint32 y);
	void setMatrix(wxUint32* matrix);

	wxUint32 m_matrix[256*256];

};

#endif //_GAMMA_VIEW_BLOCK_UNIFORMITY_H_
