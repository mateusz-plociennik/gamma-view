/////////////////////////////////////////////////////////////////////////////
// Name:        block.cpp
// Purpose:     Base class for blocks used to get / filter / view data
// Author:      Mateusz Plociennik
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#include "block.h"

void GammaBlock::BlockAttach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.push_back(block_p);
	m_blockListMutex.Unlock();
}

void GammaBlock::BlockDetach(GammaBlock* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.remove(block_p);
	m_blockListMutex.Unlock();
}

void GammaBlock::DataGet(gammaData* data_p)
{
	wxASSERT(!m_dataInListMutex.Lock());
	m_dataInList.push_back(data_p);
	m_dataInListMutex.Unlock();
}

int GammaBlock::DataWaitingCount()
{
	wxASSERT(!m_dataInListMutex.Lock());
	int ret = m_dataInList.size();
	m_dataInListMutex.Unlock();
	
	return ret;
}

void GammaBlock::DataSend()
{
	wxASSERT(!m_dataOutListMutex.Lock());
	for ( std::list<gammaData*>::iterator data_p=m_dataOutList.begin(); 
		data_p != m_dataOutList.end(); data_p++ )
	{
		wxASSERT(!m_blockListMutex.Lock());
		for ( std::list<GammaBlock*>::iterator block_p=m_blockList.begin(); 
			block_p != m_blockList.end(); block_p++ )
		{
			(*block_p)->DataGet(*data_p);
		} 
		m_dataOutList.remove(*data_p);
		m_blockListMutex.Unlock();
	}
	m_dataOutListMutex.Unlock();
}

void GammaBlock::BlockRun()
{
	m_running = TRUE;
	Create();
	SetPriority(m_priority);
	Run();
}

void GammaBlock::BlockStop()
{
	m_running = FALSE;
	Wait();
}

wxThread::ExitCode GammaBlock::Entry()
{
	while (m_running)
	{
		m_dataOutList.splice(m_dataInList.begin(),m_dataInList);
		DataSend();
	}
	return 0;
}

GammaBlock::GammaBlock()
{
}

GammaBlock::~GammaBlock()
{
}
