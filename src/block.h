/**
 * @file	block.h
 * @brief	The file contains GammaBlockBase class template.
 * @author	Mateusz Plociennik
 * @date	2012-07-16
 */

#ifndef _GAMMA_VIEW_BLOCK_H_
#define _GAMMA_VIEW_BLOCK_H_

/** 
 * Sleep time for thread, which called BlockDataGet(), when in-queue is empty. 
 */
#define GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME 20 

/** 
 * Sleep time for thread which called BlockDataPop(), when in-queue is full. 
 */
#define GAMMA_BLOCK_QUEUE_FULL_SLEEP_TIME 20

/**
 * Maximum size of in-queue. 
 * It is used by BlockDataPop() function to determine whether in-queue is full.
 */
#define GAMMA_BLOCK_QUEUE_MAX 256

#include <list>
#include <wx/thread.h>
#include <wx/wx.h>
#include <time.h>

#include "data_types.h"
#include "block_data.h"

/**
 * Template for GammaBlockBase class.
 * @tparam BlockDataIn In data type.
 * @tparam BlockDataOut Out data type.
 */
template<typename BlockDataIn, typename BlockDataOut>
class GammaBlockBase : 
	public wxThreadHelper
{
public:
	GammaBlockBase<BlockDataIn, BlockDataOut>();
	~GammaBlockBase<BlockDataIn, BlockDataOut>();

	/**
	 * This function adds block_p to internal list of blocks to which will send data.
	 * @param[in] block_p Pointer to GammaBlockBase to attach
	 */
	void BlockAttach(GammaBlockBase* block_p);

	/**
	 * This function removes block_p from internal list of blocks to which will send data.
	 * @param[in] block_p Pointer to GammaBlockBase to detach
	 */
	void BlockDetach(GammaBlockBase* block_p);

	/**
	 * This function gets GammaBlockData object from in-queue.
	 * If in-queue is empty, it will put the calling thread to sleep for GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME miliseconds.
	 * @return Pointer to GammaBlockData from in-queue
	 */
	BlockDataIn* BlockDataGet();

	/**
	 * This function is called by other blocks to add GammaBlockData referenced by data_p to in-queue.
	 * @param[in] data_p Pointer to GammaBlockData
	 */
	virtual void BlockDataPop(BlockDataIn* data_p);

	/**
	 * This function pushes GammaBlockData referenced by data_p to attached blocks.
	 * @param[in] data_p Pointer to GammaBlockData
	 */
	void BlockDataPush(BlockDataOut* data_p);

	/**
	 * This function returns count of items in in-queue.
	 * @return Count of items in in-queue.
	 */
	int BlockDataWaitingCount();
	
	/**
	 * This function creates, sets priority and run block thread.
	 */
	void BlockRun();

	/**
	 * This function pauses block thread.
	 */
	void BlockPause();

	/**
	 * This function waits for thread end and deletes thread.
	 */
	void BlockStop();

//	virtual void FrameShow() = 0;
//	virtual void MenuShow() = 0;

protected:
	/**
	 * In this function there is thread execute code. It must be defined in children classes.
	 */
	virtual wxThread::ExitCode Entry() = 0;

public:
	/**
	 * List of attached blocks.
	 */
	std::list<GammaBlockBase*> m_blockList;

	/**
	 * Mutex for list of attached blocks access.
	 */
	wxMutex m_blockListMutex;

	/**
	 * In-queue.
	 */
	std::list<BlockDataIn*> m_blockDataInList;

	/**
	 * Mutex for in-queue access.
	 */
	wxMutex m_blockDataInListMutex;
	
	/**
	 * Priority parameter (0 - 100).
	 */
	unsigned int m_priority;
};

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockAttach(GammaBlockBase* block_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	m_blockList.push_back(block_p);
	m_blockListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockDetach(GammaBlockBase* block_p)
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
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockDataPop(BlockDataIn* data_p)
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
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockDataPush(BlockDataOut* data_p)
{
	wxASSERT(!m_blockListMutex.Lock());
	for ( std::list<GammaBlockBase*>::iterator block_p=m_blockList.begin(); 
		block_p != m_blockList.end(); block_p++ )
	{
		//(*block_p)->BlockDataPop(data_p); //Doesn't work!!!
	} 
	m_blockListMutex.Unlock();
}

template<typename BlockDataIn, typename BlockDataOut>
int GammaBlockBase<BlockDataIn, BlockDataOut>::BlockDataWaitingCount()
{
	wxASSERT(!m_blockDataInListMutex.Lock());
	int ret = m_blockDataInList.size();
	m_blockDataInListMutex.Unlock();
	
	return ret;
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockRun()
{
	CreateThread();
	GetThread()->SetPriority(m_priority);
	GetThread()->Run();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockPause()
{
	GetThread()->Pause();
}

template<typename BlockDataIn, typename BlockDataOut>
void GammaBlockBase<BlockDataIn, BlockDataOut>::BlockStop()
{
	GetThread()->Wait();
}

template<typename BlockDataIn, typename BlockDataOut>
GammaBlockBase<BlockDataIn, BlockDataOut>::GammaBlockBase()
{
}

template<typename BlockDataIn, typename BlockDataOut>
GammaBlockBase<BlockDataIn, BlockDataOut>::~GammaBlockBase()
{
}

#endif //_GAMMA_VIEW_BLOCK_H_
