/**
 * @file	block_tr_sm.h
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_SM_H_
#define _GAMMA_VIEW_BLOCK_TRANS_SM_H_

#include "block_base.h"

class GammaBlockTransSM : public GammaPipeSegment
{
public:
	GammaBlockTransSM(GammaManager* pManager);
	~GammaBlockTransSM();

	void processData(GammaDataBase* pData);
	bool setParam(GammaParam_e param, void* value);

private:
	unsigned int m_intgTime;
	bool m_intgEnabled;

	unsigned long int timeCounter;
	unsigned long int timeSend;
	uint32_t* t_matrix;
	uint32_t t_max;
	uint64_t t_sum;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
