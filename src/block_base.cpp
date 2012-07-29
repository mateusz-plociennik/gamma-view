/**
 * @file	block_base.cpp
 * @brief	GammaBlockBase class methods.
 * @author	Mateusz Plociennik
 * @data	2012-07-22
 */

#include "block_base.h"

GammaBlockDataBase* GammaBlockBase::BlockDataGet()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	while (m_blockDataInList.empty() && !GetThread()->TestDestroy())
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
