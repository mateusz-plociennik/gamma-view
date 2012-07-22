/**
 * @file block_data.h
 * @brief GammaBlockDataBase class declaration.
 * @author Mateusz Plociennik
 * @data 2012-07-20
 */

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

#include <wx/datetime.h>
#include <wx/thread.h>

/**
 * Base class to make GammaBlock able to send pointers without cast.
 */
class GammaBlockDataBase
{
public:
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
	virtual void Subscribe() = 0;

	/**
	 * Decerases count of subscribers. If subscribers value reaches zero data should be destroyed.
	 */
	virtual void Unsubscribe() = 0;

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
 * GammaBlockData class template.
 * @tparam BlockDataT Type of data stored in GammaBlockData structure.
 */
template <typename BlockDataT>
class GammaBlockData :
	public GammaBlockDataBase
{
public:
	/**
	 * Increases count of subscribers.
	 */
	void Subscribe()
	{
		m_subCount++;
	}

	/**
	 * Decerases count of subscribers. If subscribers value reaches zero data should be destroyed.
	 */
	void Unsubscribe()
	{
		m_subCount--;
		if (m_subCount == 0)
		{
			delete[] data;
		}
	}

	/**
	 * Data (type given in template).
	 */
	BlockDataT data;

};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_
