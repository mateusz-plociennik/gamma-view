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
#include "block_data.h"

template<typename DataIn, typename DataOut>
class GammaBlock : public wxThreadHelper
{
public:
	void BlockAttach(GammaBlock* block_p);
	void BlockDetach(GammaBlock* block_p);

	void BlockDataPop(DataIn* data_p);
	void BlockDataPush(DataOut* data_p);
	int BlockDataWaitingCount();

	virtual void BlockRun();
	virtual void BlockPause();
	virtual void BlockStop();

	virtual void FrameShow() = 0;
	virtual void MenuShow() = 0;

	GammaBlock();
	~GammaBlock();

protected:
	virtual wxThread::ExitCode Entry();

public:
	std::list<GammaBlock*> m_blockList;
	std::list<DataIn*> m_dataInList;

	wxMutex m_blockListMutex;
	wxMutex m_dataInListMutex;

	unsigned int m_priority;
};

#endif //_GAMMA_VIEW_BLOCK_H_
