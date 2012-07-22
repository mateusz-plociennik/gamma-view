/////////////////////////////////////////////////////////////////////////////
// Name:        block.h
// Purpose:     Base class for blocks used to get / filter / view data
// Author:      Mateusz Plociennik
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_H_
#define _GAMMA_VIEW_BLOCK_H_

#define GAMMA_BLOCK_SLEEP 100


#include <list>
#include <wx/thread.h>
#include <wx/wx.h>
#include <time.h>

#include "data_types.h"
#include "block_data.h"

template<typename DataIn, typename DataOut>
class GammaBlock : 
	public wxThreadHelper
{
public:
	void BlockAttach(GammaBlock* block_p);
	void BlockDetach(GammaBlock* block_p);

	void BlockDataPop(DataIn* data_p);
	void BlockDataPush(DataOut* data_p);
	int BlockDataWaitingCount();

	void BlockRun();
	void BlockPause();
	void BlockStop();

	virtual void FrameShow() = 0;
	virtual void MenuShow() = 0;

	GammaBlock<DataIn, DataOut>();
	~GammaBlock<DataIn, DataOut>();

protected:
	wxThread::ExitCode Entry();

public:
	std::list<GammaBlock*> m_blockList;
	std::list<DataIn*> m_dataInList;

	wxMutex m_blockListMutex;
	wxMutex m_dataInListMutex;

	unsigned int m_priority;
};

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
	while (!GetThread()->TestDestroy())
	{
		wxASSERT(!m_dataInListMutex.Lock());
		if (!m_dataInList.empty())
		{
			wxASSERT(typeid(DataIn) == typeid(DataOut));
			//BlockDataPush(m_dataInList.front());
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

#endif //_GAMMA_VIEW_BLOCK_H_
