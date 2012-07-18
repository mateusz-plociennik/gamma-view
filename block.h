/////////////////////////////////////////////////////////////////////////////
// Name:        block.h
// Purpose:     Base class for blocks used to get / filter / view data
// Author:      Mateusz Plociennik
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_H_
#define _GAMMA_VIEW_BLOCK_H_

#include <list>
#include <wx/thread.h>
#include "data_types.h"

class GammaBlock : wxThread
{
public:
	void BlockAttach(GammaBlock* block_p);
	void BlockDetach(GammaBlock* block_p);
	void DataGet(gammaData* data_p);
	void DataSend();
	virtual void FrameShow() = 0;
	virtual wxThread::ExitCode Entry();

	GammaBlock();
	~GammaBlock();

	std::list<GammaBlock*> m_blockList;
	std::list<gammaData*> m_dataInList;
	std::list<gammaData*> m_dataOutList;

	wxMutex m_blockListMutex;
	wxMutex m_dataInListMutex;
	wxMutex m_dataOutListMutex;

	bool m_running;
	unsigned int m_priority;
};

#endif //_GAMMA_VIEW_BLOCK_H_
