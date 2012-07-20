/////////////////////////////////////////////////////////////////////////////
// Name:        block.cpp
// Purpose:     Base class for blocks used to get / filter / view data
// Author:      Mateusz Plociennik
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#include "block.h"

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockAttach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.push_back(block_p);
	m_blockListMutex.Unlock();
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockDetach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.remove(block_p);
	m_blockListMutex.Unlock();
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockDataPop(DataIn* data_p)
{
	wxASSERT(!m_dataInListMutex.Lock());
	m_dataInList.push_back(data_p);
	m_dataInListMutex.Unlock();
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockDataPush(DataOut* data_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	for ( std::list<GammaBlock*>::iterator block_p=m_blockList.begin(); 
		block_p != m_blockList.end(); block_p++ )
	{
		(*block_p)->BlockDataPop(data_p);
	} 
	m_blockListMutex.Unlock();
}

template<typename DataIn, typename DataOut>
int GammaBlock<DataIn, DataOut>::BlockDataWaitingCount()
{
	wxASSERT(!m_dataInListMutex.Lock());
	int ret = m_dataInList.size();
	m_dataInListMutex.Unlock();
	
	return ret;
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockRun()
{
	CreateThread();
	GetThread()->SetPriority(m_priority);
	GetThread()->Run();
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockPause()
{
	GetThread()->Pause();
}

template<typename DataIn, typename DataOut>
void GammaBlock<DataIn, DataOut>::BlockStop()
{
	GetThread()->Wait();
}

template<typename DataIn, typename DataOut>
wxThread::ExitCode GammaBlock<DataIn, DataOut>::Entry()
{
	while (!GetThread->TestDestroy())
	{
		wxASSERT(!m_dataInListMutex.Lock());
		if (!m_dataInList.empty())
		{
			wxASSERT(typeid(DataIn) == typeid(DataOut));
			BlockDataPush(m_dataInList.front());
			m_dataInList.pop_front();
		}
		m_dataInListMutex.Unlock();
	}
	return 0;
}

template<typename DataIn, typename DataOut>
GammaBlock<DataIn, DataOut>::GammaBlock()
{
}

template<typename DataIn, typename DataOut>
GammaBlock<DataIn, DataOut>::~GammaBlock()
{
}
