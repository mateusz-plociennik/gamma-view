/////////////////////////////////////////////////////////////////////////////
// Name:        block_trans.h
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_TRANS_H_
#define _GAMMA_VIEW_BLOCK_TRANS_H_

#include "block.h"

class GammaBlockTrans : public GammaBlock
{
public:
	GammaBlockTrans();
	~GammaBlockTrans();

	void BlockRun();
	void BlockStop();

private:
	wxThread::ExitCode Entry();

};

#endif //_GAMMA_VIEW_BLOCK_TRANS_H_
