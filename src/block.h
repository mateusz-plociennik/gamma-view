/////////////////////////////////////////////////////////////////////////////
// Name:        block.h
// Purpose:     Base class for blocks used to get / filter / view data
// Author:      Mateusz Plociennik
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_H_
#define _GAMMA_VIEW_BLOCK_H_

#define GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME 20
#define GAMMA_BLOCK_QUEUE_FULL_SLEEP_TIME 20
#define GAMMA_BLOCK_QUEUE_MAX 256

#include <list>
#include <wx/thread.h>
#include <wx/wx.h>
#include <time.h>

#include "data_types.h"
#include "block_data.h"

template<typename BlockDataIn, typename BlockDataOut>
class GammaBlock : 
	public wxThreadHelper
{
public:
	void BlockAttach(GammaBlock* block_p);
	void BlockDetach(GammaBlock* block_p);

	BlockDataIn* BlockDataGet();
	virtual void BlockDataPop(BlockDataIn* data_p);
	void BlockDataPush(BlockDataOut* data_p);
	int BlockDataWaitingCount();

	void BlockRun();
	void BlockPause();
	void BlockStop();

	virtual void FrameShow() = 0;
	virtual void MenuShow() = 0;

	GammaBlock<BlockDataIn, BlockDataOut>();
	~GammaBlock<BlockDataIn, BlockDataOut>();

protected:
	wxThread::ExitCode Entry();

public:
	std::list<GammaBlock*> m_blockList;
	std::list<BlockDataIn*> m_blockDataInList;

	wxMutex m_blockListMutex;
	wxMutex m_blockDataInListMutex;

	unsigned int m_priority;
};

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockAttach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.push_back(block_p);
	m_blockListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockDetach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.remove(block_p);
	m_blockListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
BlockDataIn* BlockDataGet()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	while (m_blockDataInList.empty());
	{
		m_blockDataInListMutex.Unlock();
		GetThread()->Sleep(GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME);
		m_blockDataInListMutex.Lock();
	}
	BlockDataIn* blockBlockDataIn = m_blockDataInList.front();
	m_blockDataInList.pop_front();
	m_blockDataInListMutex.Unlock();

	return blockBlockDataIn;
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockDataPop(BlockDataIn* data_p)
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	while (m_blockDataInList.size() == GAMMA_BLOCK_QUEUE_MAX);
	{
		m_blockDataInListMutex.Unlock();
		GetThread()->Sleep(GAMMA_BLOCK_QUEUE_FULL_SLEEP_TIME);
		wxASSERT(!m_blockDataInListMutex.Lock());
	}
	m_blockDataInList.push_back(data_p);
	m_blockDataInListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockDataPush(BlockDataOut* data_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	for ( std::list<GammaBlock*>::iterator block_p=m_blockList.begin(); 
		block_p != m_blockList.end(); block_p++ )
	{
		(*block_p)->BlockDataPop(data_p);
	} 
	m_blockListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
int GammaBlock<BlockDataIn, BlockDataOut>::BlockDataWaitingCount()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	int ret = m_blockDataInList.size();
	m_blockDataInListMutex.Unlock();
	
	return ret;
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockRun()
{
	CreateThread();
	GetThread()->SetPriority(m_priority);
	GetThread()->Run();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockPause()
{
	GetThread()->Pause();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlock<BlockDataIn, BlockDataOut>::BlockStop()
{
	GetThread()->Wait();
}

template<typename BlockDataIn, typename BlockDataOut>
wxThread::ExitCode GammaBlock<BlockDataIn, BlockDataOut>::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		wxASSERT(!m_blockDataInListMutex.Lock());
		if (!m_blockDataInList.empty())
		{
			wxASSERT(typeid(BlockDataIn) == typeid(BlockDataOut));
			//BlockDataPush(m_blockDataInList.front());
			m_blockDataInList.pop_front();
		}
		m_blockDataInListMutex.Unlock();
	}
	return 0;
}

template<typename BlockDataIn, typename BlockDataOut>
GammaBlock<BlockDataIn, BlockDataOut>::GammaBlock()
{
}

template<typename BlockDataIn, typename BlockDataOut>
GammaBlock<BlockDataIn, BlockDataOut>::~GammaBlock()
{
}

#endif //_GAMMA_VIEW_BLOCK_H_
