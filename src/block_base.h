/**
 * @file	block_base.h
 * @brief	The file contains GammaBlockBase class template.
 * @author	Mateusz Plociennik
 * @date	2012-07-16
 */

#ifndef _GAMMA_VIEW_BLOCK_BASE_H_
#define _GAMMA_VIEW_BLOCK_BASE_H_

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
 * GammaBlockBase class.
 */
class GammaBlockBase : 
	public wxThreadHelper
{
public:
	/*
	GammaBlockBase();
	~GammaBlockBase();
*/
	/**
	 * This function adds block_p to internal list of blocks to which will 
	 * send data.
	 * @param[in] block_p Pointer to GammaBlockBase to attach
	 */
	void BlockAttach(GammaBlockBase* block_p)
	{
		wxMutexLocker locker(m_blockListMutex);
		m_blockList.push_back(block_p);
	}

	/**
	 * This function removes block_p from internal list of blocks to which 
	 * will send data.
	 * @param[in] block_p Pointer to GammaBlockBase to detach
	 */
	void BlockDetach(GammaBlockBase* block_p)
	{
		wxMutexLocker locker(m_blockListMutex);
		m_blockList.remove(block_p);
	}

	/**
	 * This function gets GammaBlockDataBase object from in-queue.
	 * If in-queue is empty, it will put the calling thread to sleep 
	 * for GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME miliseconds.
	 * @return Pointer to GammaBlockDataBase from in-queue
	 */
	GammaBlockDataBase* DataGet()
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

	/**
	 * This function is called by other blocks to add GammaBlockDataBase 
	 * referenced by blockData_p to in-queue.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	virtual void DataPop(GammaBlockDataBase* blockData_p)
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

	/**
	 * This function pushes GammaBlockDataBase referenced by blockData_p 
	 * to attached blocks.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	void DataPush(GammaBlockDataBase* blockData_p)
	{
		wxMutexLocker locker(m_blockListMutex);
		for ( std::list<GammaBlockBase*>::iterator block_p=m_blockList.begin(); 
			block_p != m_blockList.end(); block_p++ )
		{
			(*block_p)->DataPop(blockData_p);
		}
	}

	/**
	 * This function returns count of items in in-queue.
	 * @return Count of items in in-queue.
	 */
	int DataWaitingCount()
	{
		wxMutexLocker locker(m_blockDataInListMutex);
	
		return m_blockDataInList.size();
	}
	
	/**
	 * This function creates, sets priority and run block thread.
	 */
	void Run()
	{
		CreateThread();
		GetThread()->Run();
	}

	/**
	 * This function pauses block thread.
	 */
	void Pause()
	{
		GetThread()->Pause();
	}

	/**
	 * This function waits for thread end and deletes thread.
	 */
	void Stop()
	{
		GetThread()->Delete();
	}

//	virtual void FrameShow() = 0;
//	virtual void MenuShow() = 0;

protected:
	/**
	 * In this function there is thread execute code. It must be defined 
	 * in children classes.
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
	std::list<GammaBlockDataBase*> m_blockDataInList;

	/**
	 * Mutex for in-queue access.
	 */
	wxMutex m_blockDataInListMutex;
	
	/**
	 * Priority parameter (0 - 100).
	 */
	unsigned int m_priority;
};

#endif //_GAMMA_VIEW_BLOCK_BASE_H_
