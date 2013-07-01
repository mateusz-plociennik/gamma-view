/**
 * @file	block_base.h
 * @brief	The file contains GammaPipeBase class template.
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
#include <set>
#include <time.h>

#include <wx/sharedptr.h>
#include <wx/thread.h>
#include <wx/wx.h>
#include <wx/string.h>

#include "block_mgmt.h"

#include "data_types.h"
#include "block_data.h"

class GammaPipeFrontEnd;
class GammaPipeBackEnd;

/**
 * GammaPipeBase class.
 */
class GammaPipeBase
{
public:

	GammaPipeBase()
		: m_bStarted(false)
		, m_bPaused(false)
	{
	}

	virtual void start()
	{
		m_bStarted = true;
	}

	virtual void stop()
	{
		m_bStarted = false;
	}

	virtual void pause()
	{
		m_bPaused = true;
	}

	virtual void resume()
	{
		m_bPaused = false;
	}

	/**
	 * Set params in this layer.
	 */
	virtual wxInt32 setParam(GammaParam_e param, void* value)
	{
		UNREFERENCED_PARAMETER(param);
		UNREFERENCED_PARAMETER(value);
		return 0;
	}

protected:
	bool m_bStarted;
	bool m_bPaused;
};

class GammaPipeMgmt
{
public:
	GammaPipeMgmt(GammaManager* pManager, wxString name = wxT("no_name")) 
		: m_pManager(pManager)
		, m_name(name)
	{
	}

	/**
	 * This function returns pointer to GammaManager
	 * @return Pointer to GammaManager.
	 */
	GammaManager* getManager()
	{
		return m_pManager;
	}

	wxString getName()
	{
		return m_name;
	}

private:
	GammaManager* m_pManager;
	wxString m_name;
};


class GammaPipeFrontEnd : public GammaPipeBase
{
public:

	/**
	 * In this function there is thread execute code. It must be defined 
	 * in children classes.
	 */
	virtual void processData(GammaData* pData) = 0;

	GammaPipeFrontEnd& operator+=(GammaPipeFrontEnd& second);

protected:
	/**
	 * Processing mutex
	 */
	wxMutex m_processDataMutex;
};

class GammaPipeBackEnd : public GammaPipeBase
{
public:
	virtual ~GammaPipeBackEnd()
	{
	}

	/**
	 * This function adds block_p to internal list of blocks to which will 
	 * send data.
	 * @param[in] block_p Pointer to GammaPipeBase to attach
	 */
	void connectSegment(GammaPipeFrontEnd* pSegment)
	{
		wxMutexLocker locker(m_segmentSetMutex);
		m_segmentSet.insert(pSegment);
	}

	GammaPipeBackEnd& operator+=(GammaPipeFrontEnd& segment)
	{
		wxMutexLocker locker(m_segmentSetMutex);
		m_segmentSet.insert(&segment);

		return *this;
	}

	/**
	 * This function removes block_p from internal list of blocks to which 
	 * will send data.
	 * @param[in] pBlock Pointer to GammaPipeBase to detach
	 */
	void disconnectSegment(GammaPipeFrontEnd* pSegment)
	{
		wxMutexLocker locker(m_segmentSetMutex);
		m_segmentSet.erase(pSegment);
	}

	/**
	 * This function pushes GammaBlockDataBase referenced by blockData_p 
	 * to attached blocks.
	 * @param[in] blockData_p Pointer to GammaBlockDataBase
	 */
	inline void pushData(GammaData* pDataOut)
	{
		//m_processDataMutex.Unlock();

		wxMutexLocker locker(m_segmentSetMutex);
		for(std::set<GammaPipeFrontEnd*>::iterator iSegment = m_segmentSet.begin(); 
			iSegment != m_segmentSet.end(); iSegment++)
		{
			(*iSegment)->processData(pDataOut);
		}

		//m_processDataMutex.Lock();
	}

	void startAll()
	{
		start();

		wxMutexLocker locker(m_segmentSetMutex);
		for(std::set<GammaPipeFrontEnd*>::iterator iSegment = m_segmentSet.begin(); 
			iSegment != m_segmentSet.end(); iSegment++)
		{
			if(dynamic_cast<GammaPipeBackEnd*>(*iSegment))
			{
				dynamic_cast<GammaPipeBackEnd*>(*iSegment)->startAll();
			}
		}
	}

	void stopAll()
	{
		stop();

		wxMutexLocker locker(m_segmentSetMutex);
		for(std::set<GammaPipeFrontEnd*>::iterator iSegment = m_segmentSet.begin(); 
			iSegment != m_segmentSet.end(); iSegment++)
		{
			if(dynamic_cast<GammaPipeBackEnd*>(*iSegment))
			{
				dynamic_cast<GammaPipeBackEnd*>(*iSegment)->stopAll();
			}
		}
	}

	wxInt32 setParamAll(GammaParam_e param, void* value)
	{
		wxInt32 r = 0;
		
		r += setParam(param, value);

		wxMutexLocker locker(m_segmentSetMutex);
		for(std::set<GammaPipeFrontEnd*>::iterator iSegment = m_segmentSet.begin(); 
			iSegment != m_segmentSet.end(); iSegment++)
		{
			if(dynamic_cast<GammaPipeBackEnd*>(*iSegment))
			{
				r += dynamic_cast<GammaPipeBackEnd*>(*iSegment)->setParamAll(param, value);
			}
		}

		return r;
	}

	void deleteAll()
	{
		{
			wxMutexLocker locker(m_segmentSetMutex);
			for(std::set<GammaPipeFrontEnd*>::iterator iSegment = m_segmentSet.begin(); 
				iSegment != m_segmentSet.end(); iSegment++)
			{
				if(dynamic_cast<GammaPipeBackEnd*>(*iSegment))
				{
					dynamic_cast<GammaPipeBackEnd*>(*iSegment)->deleteAll();
				}
			}
		}

		delete this;
	}

private:
	/**
	 * List of attached blocks.
	 */
	std::set<GammaPipeFrontEnd*> m_segmentSet;

	/**
	 * Mutex for list of attached blocks access.
	 */
	wxMutex m_segmentSetMutex;
};

class GammaPipeSegment : public GammaPipeMgmt, 
	public GammaPipeFrontEnd, public GammaPipeBackEnd
{
public:
	GammaPipeSegment(GammaManager* pManager) 
		: GammaPipeMgmt(pManager)
	{
	}

	virtual ~GammaPipeSegment()
	{
	}
};

class GammaPipeHead : public GammaPipeMgmt, public wxThreadHelper,
	public GammaPipeBackEnd
{
public:
	GammaPipeHead(GammaManager* pManager) 
		: GammaPipeMgmt(pManager)
	{
	}

	virtual ~GammaPipeHead()
	{
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
/*
protected: 
	virtual wxThread::ExitCode Entry() = 0;
*/

private:
	bool m_run;
};

#endif //_GAMMA_VIEW_BLOCK_BASE_H_
