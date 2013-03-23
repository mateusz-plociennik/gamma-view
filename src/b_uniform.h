/**
 * @file	b_uniform.h
 * @brief	uniformity filter
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_UNIFORMITY_H_
#define _GAMMA_VIEW_BLOCK_UNIFORMITY_H_

#include "block_base.h"

class GammaUniformity : public GammaBlockBase
{
public:
	GammaUniformity(GammaManager* pManager);

	bool SetParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
	int CorrCount(uint8_t x, uint8_t y);
	void SetMatrix(uint32_t* matrix, uint32_t norm);
	int32_t m_matrix[256*256];

};

#endif //_GAMMA_VIEW_BLOCK_UNIFORMITY_H_
