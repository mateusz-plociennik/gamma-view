/**
 * @file	block_tr_sm.h
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_SM_H_
#define _GAMMA_VIEW_BLOCK_TRANS_SM_H_

#include "block_base.h"

class GammaBlockTransSM : 
	public GammaBlockBase
{
public:
//	GammaBlockFileSave();
//	~GammaBlockFileSave();

protected:
	wxThread::ExitCode Entry();
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
