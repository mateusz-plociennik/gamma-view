/**
 * @file block_base.cpp
 * @brief GammaBlockBase class methods.
 * @author Mateusz Plociennik
 * @data 2012-07-22
 */

#include "block_base.h"

void GammaBlockBase::BlockAttach(GammaBlockBase* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.push_back(block_p);
	m_blockListMutex.Unlock();
}

void GammaBlockBase::BlockDetach(GammaBlockBase* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.remove(block_p);
	m_blockListMutex.Unlock();
}

GammaBlockDataBase* GammaBlockBase::BlockDataGet()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	while (m_blockDataInList.empty());
	{
		m_blockDataInListMutex.Unlock();
		GetThread()->Sleep(GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME);
		m_blockDataInListMutex.Lock();
	}
	GammaBlockDataBase* blockBlockDataIn = m_blockDataInList.front();
	m_blockDataInList.pop_front();
	m_blockDataInListMutex.Unlock();

	return blockBlockDataIn;
}

void GammaBlockBase::BlockDataPop(GammaBlockDataBase* blockData_p)
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	while (m_blockDataInList.size() == GAMMA_BLOCK_QUEUE_MAX);
	{
		m_blockDataInListMutex.Unlock();
		GetThread()->Sleep(GAMMA_BLOCK_QUEUE_FULL_SLEEP_TIME);
		wxASSERT(!m_blockDataInListMutex.Lock());
	}
	m_blockDataInList.push_back(blockData_p);
	blockData_p->Subscribe();
	m_blockDataInListMutex.Unlock();
}

void GammaBlockBase::BlockDataPush(GammaBlockDataBase* blockData_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	for ( std::list<GammaBlockBase*>::iterator block_p=m_blockList.begin(); 
		block_p != m_blockList.end(); block_p++ )
	{
		(*block_p)->BlockDataPop(blockData_p);
	} 
	m_blockListMutex.Unlock();
}

int GammaBlockBase::BlockDataWaitingCount()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	int ret = m_blockDataInList.size();
	m_blockDataInListMutex.Unlock();
	
	return ret;
}

void GammaBlockBase::BlockRun()
{
	CreateThread();
	GetThread()->SetPriority(m_priority);
	GetThread()->Run();
}

void GammaBlockBase::BlockPause()
{
	GetThread()->Pause();
}

void GammaBlockBase::BlockStop()
{
	GetThread()->Wait();
}

GammaBlockBase::GammaBlockBase()
{
}

GammaBlockBase::~GammaBlockBase()
{
}
