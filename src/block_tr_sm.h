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
	GammaBlockTransSM(unsigned long int timeDiff = 100, bool integrate = false)
	{
		m_timeDiff = timeDiff;
		m_integrate = integrate;
	}
//	~GammaBlockFileSave();

protected:
	wxThread::ExitCode Entry();

private:
	unsigned long int m_timeDiff;
	bool m_integrate;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
