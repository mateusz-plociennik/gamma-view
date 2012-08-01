/**
 * @file	block_data.h
 * @brief	GammaBlockDataBase class declaration.
 * @author	Mateusz Plociennik
 * @data	2012-07-20
 */

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

#include <wx/datetime.h>
#include <wx/thread.h>
#include <list>
#include "data_types.h"

/**
 * Base class to make GammaBlock able to send pointers without cast.
 */
class GammaBlockDataBase
{
public:
	/**
	 * Default constructor.
	 */
	GammaBlockDataBase()
	{
		m_subCount = 0;
	}

	/**
	 * True virtual function to destroy data. 
	 * Must be virtual, because different methods to destroy data.
	 */
	//virtual void DataDestroy() = 0;

	/**
	 * Locks dataMutex.
	 */
	wxMutexError Lock()
	{
		return dataMutex.Lock();
	}

	/**
	 * Unlocks dataMutex.
	 */
	wxMutexError Unlock()
	{
		return dataMutex.Unlock();
	}

	/**
	 * Increases count of subscribers.
	 */
	void Subscribe()
	{
		m_subCount++;
	}

	/**
	 * Decerases count of subscribers. 
	 * If subscribers value reaches zero data should be destroyed.
	 */
	void Unsubscribe()
	{
		m_subCount--;
		if (m_subCount == 0)
		{
			delete this;
		}
	}

	/**
	 * Date and time of packet arrival.
	 */
	wxDateTime datetime;

	/**
	 * Count of subscribers. When its value reaches zero, object should be destroyed.
	 */
	unsigned char m_subCount;

private:
	/**
	 * Mutex for data access.
	 */
	wxMutex dataMutex;

};

/**
 * GammaDataUSB class.
 */
class GammaDataUSB:
	public GammaBlockDataBase
{
public:
	GammaDataUSB()
	{
		data = new unsigned char(512);
	}

	~GammaDataUSB()
	{
		delete[] data;
	}

	unsigned char* data;
};

/**
 * GammaDataItems class.
 */
class GammaDataItems:
	public GammaBlockDataBase
{
public:
	~GammaDataItems()
	{
		data.clear();
	}

	wxVector<GammaItem> data;
};

/**
 * GammaDataImage class.
 */
class GammaDataMatrix:
	public GammaBlockDataBase
{
public:
	GammaDataMatrix()
	{
		data = new unsigned short int(256 * 256);
	}

	~GammaDataMatrix()
	{
		delete[] data;
	}

	unsigned short int* data;
};

/**
 * GammaDataImage class.
 */
class GammaDataImage:
	public GammaBlockDataBase
{
public:
	~GammaData()
	{
		//delete data;
	}

	wxImage data;
};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_
