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
 * It is used by BlockDataPush() function to determine whether in-queue is full.                   
 */
#define GAMMA_BLOCK_QUEUE_MAX 256

#include <list>
#include <time.h>

#include <wx/sharedptr.h>
#include <wx/thread.h>
#include <wx/wx.h>

#include "block_mgmt.h"

#include "data_types.h"
#include "block_data.h"

/**
 * GammaBlockBase class.
 */
class GammaBlockBase : public wxThreadHelper
{
public:

	GammaBlockBase(GammaManager* pManager, uint32_t queueSize) 
			: 
			m_pManager(pManager),
			m_queueSize(queueSize),
			m_bRun(false),
			m_dataInListConditionNotEmpty(m_dataInListMutex),
			m_dataInListConditionNotFull(m_dataInListMutex)
	{
		wxLogStatus("%s - ctor", __FUNCTION__);
	}

	virtual ~GammaBlockBase()
	{
		wxLogStatus("%s - dtor", __FUNCTION__);
	}

	/**
	 * This function adds block_p to internal list of blocks to which will 
	 * send data.
	 * @param[in] block_p Pointer to GammaBlockBase to attach
	 */
	void BlockAttach(GammaBlockBase* pBlock)
	{
		wxMutexLocker locker(m_blockOutListMutex);
		m_blockOutList.push_back(pBlock);
	}

	/**
	 * This function removes block_p from internal list of blocks to which 
	 * will send data.
	 * @param[in] pBlock Pointer to GammaBlockBase to detach
	 */
	void BlockDetach(GammaBlockBase* pBlock)
	{
		wxMutexLocker locker(m_blockOutListMutex);
		m_blockOutList.remove(pBlock);
	}

	/**
	 * This function gets GammaBlockDataBase object from in-queue.
	 * If in-queue is empty, it will put the calling thread to sleep 
	 * for GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME miliseconds.
	 * @return Pointer to GammaBlockDataBase from in-queue
	 */
	wxSharedPtr<GammaDataBase> DataGet()
	{
		wxMutexLocker locker(m_dataInListMutex);

		wxSharedPtr<GammaDataBase> dataIn(m_dataInList.front());
		m_dataInList.pop_front();

		m_dataInListConditionNotFull.Signal();

		return dataIn;
	}

	/**
	 * This function is called by other blocks to add GammaBlockDataBase 
	 * referenced by blockData_p to in-queue.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	void DataPop(wxSharedPtr<GammaDataBase>& dataIn)
	{
		wxMutexLocker locker(m_dataInListMutex);

		while(ShouldBeRunning() && m_dataInList.size() >= m_queueSize)
		{
			m_dataInListConditionNotFull.Wait();
		}

		m_dataInList.push_back(dataIn);
		m_dataInListConditionNotEmpty.Signal();
	}

	/**
	 * This function pushes GammaBlockDataBase referenced by blockData_p 
	 * to attached blocks.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	void DataPush(GammaDataBase* pDataOut)
	{
		wxSharedPtr<GammaDataBase> dataOut(pDataOut);
		wxMutexLocker locker(m_blockOutListMutex);

		for( std::list<GammaBlockBase*>::iterator iBlock = m_blockOutList.begin(); 
			iBlock != m_blockOutList.end(); iBlock++ )
		{
			(*iBlock)->DataPop(dataOut);
		}
	}

	/**
	 * If there are no data in in-queue, than make the thread sleep 
	 * for GAMMA_BLOCK_QUEUE_EMPTY_SLEEP_TIME miliseconds.
	 * @return Bool if in-queue has data.
	 */
	bool DataReady()
	{
		wxMutexLocker locker(m_dataInListMutex);

		while( ShouldBeRunning() && m_dataInList.empty() )
		{
			m_dataInListConditionNotEmpty.Wait();
		}

		return ShouldBeRunning();
	}

	/**
	 * This function returns count of items in in-queue.
	 * @return Count of items in in-queue.
	 */
	unsigned int DataWaitingCount()
	{
		wxMutexLocker locker(m_dataInListMutex);
	
		return m_dataInList.size();
	}

	/**
	 * This function returns pointer to GammaManager
	 * @return Pointer to GammaManager.
	 */
	GammaManager* GetManager()
	{
		return m_pManager;
	}
	
	/**
	 * This function returns count of items in in-queue.
	 * @return Count of items in in-queue.
	 */
	bool ShouldBeRunning()
	{
		return m_bRun && !GetThread()->TestDestroy();
	}

	/**
	 * This function creates, sets priority and run block thread.
	 */
	void Run()
	{
		CreateThread();
		m_bRun = true;
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
		/*
		if( GetThread()->IsPaused() )
		{
			GetThread()->Resume();
		}
		*/
		m_bRun = false;
		/*
		m_dataInListConditionNotFull.Signal();
		m_dataInListConditionNotEmpty.Signal();

		wxMutexLocker locker(m_threadRunMutex);
		
		if( GetThread() && GetThread()->IsRunning() )
		{
			GetThread()->Delete();
		}
		*/
	}

	/**
	 * Middleware sets params in this layer.
	 */
	virtual bool SetParam(GammaParam_e param, void* value)
	{
		UNREFERENCED_PARAMETER(param);
		UNREFERENCED_PARAMETER(value);

		return false;
	}

protected:
	/**
	 * In this function there is thread execute code. It must be defined 
	 * in children classes.
	 */
	virtual wxThread::ExitCode Entry() = 0;

	uint32_t m_queueSize;

	/**
	 * Bool to stop thread execution.
	 */
	bool m_bRun;

	/**
	 * List of attached blocks.
	 */
	std::list<GammaBlockBase*> m_blockOutList;

	/**
	 * Mutex for list of attached blocks access.
	 */
	wxMutex m_blockOutListMutex;

	/**
	 * In-queue.
	 */
	std::list<wxSharedPtr<GammaDataBase>> m_dataInList;

	/**
	 * Mutex for in-queue access.
	 */
	wxMutex m_dataInListMutex;

	/**
	 *
	 */
	wxCondition m_dataInListConditionNotEmpty;

	/**
	 *
	 */
	wxCondition m_dataInListConditionNotFull;

	wxMutex m_threadRunMutex;
	
	/**
	 * Priority parameter (0 - 100).
	 */
	//unsigned int m_priority;
	GammaManager* m_pManager;

};

#endif //_GAMMA_VIEW_BLOCK_BASE_H_
