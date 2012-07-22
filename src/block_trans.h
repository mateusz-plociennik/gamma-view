/////////////////////////////////////////////////////////////////////////////
// Name:        block_trans.h
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_TRANS_H_
#define _GAMMA_VIEW_BLOCK_TRANS_H_

#include "block_base.h"

class GammaBlockTrans : 
	public GammaBlockBase
{
public:
	GammaBlockTrans();
	~GammaBlockTrans();

	void SetTimeDiv(unsigned long timeDiv)
	{
		m_timeDiv = timeDiv;
	}

protected:
	wxThread::ExitCode Entry();

private:
	unsigned long m_timeCounter;
	unsigned long m_timeDiv;

};

#endif //_GAMMA_VIEW_BLOCK_TRANS_H_
