/////////////////////////////////////////////////////////////////////////////
// Name:        block_trans.h
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_TRANS_H_
#define _GAMMA_VIEW_BLOCK_TRANS_H_

#include "block.h"

class GammaBlockTrans : 
	public GammaBlockBase< GammaBlockData<unsigned char*> , GammaBlockData<std::list<GammaItem>> >
{
public:
	GammaBlockTrans();
	~GammaBlockTrans();

protected:
	wxThread::ExitCode Entry();

private:

};

#endif //_GAMMA_VIEW_BLOCK_TRANS_H_
