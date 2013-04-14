/**
 * @file	block_tr_us.h
 * @brief	translator for RAW data from USB
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_US_H_
#define _GAMMA_VIEW_BLOCK_TRANS_US_H_

#include "block_base.h"

class GammaBlockTransUS : public GammaPipeSegment
{
public:
	GammaBlockTransUS(GammaManager* pManager);

	void processData(GammaDataBase* pData);

private:
	unsigned long m_timeCounter;
	unsigned long m_timeDiv;

};

#endif //_GAMMA_VIEW_BLOCK_TRANS_H_
