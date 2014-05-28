/**
 * @file	block_data.h
 * @brief	GammaBlockDataBase class declaration.
 * @author	Mateusz Plociennik
 * @data	2012-07-20
 */

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

#include <vector>

#include <wx/datetime.h>
#include <wx/thread.h>
#include <wx/image.h>
#include <list>
#include "data_types.h"

#pragma warning(disable : 4351) // elements of array will be default initialized

/**
 * Base class to make GammaBlock able to send pointers without cast.
 */
class GammaData
{
public:
	/**
	 * Default constructor.
	 */
	GammaData(GammaDataType_e i_type)
		: type(i_type)
		, m_readersCount(0)
	{
	}

	virtual ~GammaData()
	{
	}

	void enterRead()
	{
		wxCriticalSectionLocker orderLocker(m_orderCriticalSection);
		wxCriticalSectionLocker readersLocker(m_readersCriticalSection);
		if(0 == m_readersCount++)
		{
			m_accessCriticalSection.Enter();
		}
	}

	void leaveRead()
	{
		wxCriticalSectionLocker readersLocker(m_readersCriticalSection);
		if(0 == --m_readersCount)
		{
			m_accessCriticalSection.Leave();
		}
	}

	void enterWrite()
	{
		wxCriticalSectionLocker orderLocker(m_orderCriticalSection);
		m_accessCriticalSection.Enter();
	}

	void leaveWrite()
	{
		m_accessCriticalSection.Leave();
	}

	const GammaDataType_e type;

private:
	wxCriticalSection m_orderCriticalSection;
	wxCriticalSection m_accessCriticalSection;
	wxCriticalSection m_readersCriticalSection;
	wxInt32 m_readersCount;
};

/**
 * GammaDataUSB class.
 */
class GammaDataUSB :
	public GammaData
{
public:
	GammaDataUSB()
		: GammaData(GAMMA_DATA_TYPE_USB)
	{
	}

	~GammaDataUSB()
	{
	}

	wxUint8 data[256 * 256];
};

/**
 * GammaItems class.
 */
class GammaItems :
	public GammaData
{
public:
	GammaItems()
		: GammaData(GAMMA_DATA_TYPE_ITEMS)
		, items(256)
	{
	}

	~GammaItems()
	{
	}

	std::vector<GammaItem> items;
};

#define GAMMA_EVENT_UNIT (256)

/**
 * GammaImage class.
 */
class GammaMatrix :
	public GammaData
{
public:
	GammaMatrix()
		: GammaData(GAMMA_DATA_TYPE_MATRIX)
		, matrix()
		, eventSum(0)
		, eventSumFov(0)
		, acqTime(0)
		, intTime(0)
		, trig(GAMMA_TRIG_NONE)
	{
	}

	~GammaMatrix()
	{
	}

	wxUint32 eventMax()
	{
		wxUint32 r = 0;
		for(wxInt32 i = 0; i < 256 * 256; i++)
		{
			if(r < matrix[i])
			{
				r = matrix[i];
			}
		}

		return r;
	}

	wxUint32 matrix[256 * 256];
	wxUint64 eventSum;
	wxUint64 eventSumFov;

	wxTimeSpan acqTime;
	wxTimeSpan intTime;

	GammaTrig_e trig;
};

/**
 * GammaImage class.
 */
class GammaImage :
	public GammaData
{
public:
	GammaImage()
		: GammaData(GAMMA_DATA_TYPE_IMAGE)
		, image(256, 256, true)
	{
	}

	~GammaImage()
	{
	}

	wxImage image;
};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_
