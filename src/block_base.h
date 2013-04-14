/**
 * @file	block_base.h
 * @brief	The file contains GammaPipeSegment class template.
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
 * GammaPipeSegment class.
 */
class GammaPipeSegment
{
public:

	GammaPipeSegment(GammaManager* pManager) 
		: m_pManager(pManager)
	{
		wxLogStatus("%s - ctor", __FUNCTION__);
	}

	~GammaPipeSegment()
	{
		wxLogStatus("%s - dtor", __FUNCTION__);
	}

	/**
	 * This function adds block_p to internal list of blocks to which will 
	 * send data.
	 * @param[in] block_p Pointer to GammaPipeSegment to attach
	 */
	void connectSegment(GammaPipeSegment* pSegment)
	{
		wxMutexLocker locker(m_segmentListMutex);
		m_segmentList.push_back(pSegment);
	}

	GammaPipeSegment* operator+=(GammaPipeSegment* pSegment)
	{
		wxMutexLocker locker(m_segmentListMutex);
		m_segmentList.push_back(pSegment);

		return this;
	}

	/**
	 * This function removes block_p from internal list of blocks to which 
	 * will send data.
	 * @param[in] pBlock Pointer to GammaPipeSegment to detach
	 */
	void disconnectSegment(GammaPipeSegment* pSegment)
	{
		wxMutexLocker locker(m_segmentListMutex);
		m_segmentList.remove(pSegment);
	}

	/**
	 * This function pushes GammaBlockDataBase referenced by blockData_p 
	 * to attached blocks.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	inline void pushData(GammaDataBase* pDataOut)
	{
		wxMutexLocker locker(m_segmentListMutex);

		for(std::list<GammaPipeSegment*>::iterator iSegment = m_segmentList.begin(); 
			iSegment != m_segmentList.end(); iSegment++ )
		{
			(*iSegment)->processData(pDataOut);
		}
	}

	/**
	 * This function returns pointer to GammaManager
	 * @return Pointer to GammaManager.
	 */
	GammaManager* getManager()
	{
		return m_pManager;
	}

	/**
	 * Middleware sets params in this layer.
	 */
	virtual bool setParam(GammaParam_e param, void* value)
	{
		UNREFERENCED_PARAMETER(param);
		UNREFERENCED_PARAMETER(value);
		return false;
	}


	/**
	 * In this function there is thread execute code. It must be defined 
	 * in children classes.
	 */
	virtual void processData(GammaDataBase* pData) = 0;

	/**
	 * Processing mutex
	 */
	wxMutex m_processDataMutex;

protected:
	/**
	 * List of attached blocks.
	 */
	std::list<GammaPipeSegment*> m_segmentList;

	/**
	 * Mutex for list of attached blocks access.
	 */
	wxMutex m_segmentListMutex;

	GammaManager* m_pManager;

};

class GammaPipeHead : public GammaPipeSegment, public wxThreadHelper
{
public:
	GammaPipeHead(GammaManager* pManager) 
		: GammaPipeSegment(pManager)
	{
		wxLogStatus("%s - ctor", __FUNCTION__);
	}

	~GammaPipeHead()
	{
		wxLogStatus("%s - dtor", __FUNCTION__);
	}

	void processData(GammaDataBase* pData)
	{
		UNREFERENCED_PARAMETER(pData);
		wxASSERT_MSG(0, "It's pipeline head!");
	}

	bool shouldBeRunning()
	{
		return m_run && !GetThread()->TestDestroy();
	}

	void start()
	{
		CreateThread();
		m_run = true;
		GetThread()->Run();
	}

	void stop()
	{
		m_run = false;
		GetThread()->Wait();
	}

protected: 
	virtual wxThread::ExitCode Entry() = 0;

private:
	bool m_run;
};

#endif //_GAMMA_VIEW_BLOCK_BASE_H_
