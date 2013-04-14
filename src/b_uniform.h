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

	void processData(GammaDataBase* pData);
	bool setParam(GammaParam_e param, void* value);

private:
	int corrCount(uint8_t x, uint8_t y);
	void setMatrix(uint32_t* matrix, uint32_t norm);

	int32_t m_matrix[256*256];

};

#endif //_GAMMA_VIEW_BLOCK_UNIFORMITY_H_
