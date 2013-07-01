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
#include <list>
#include "data_types.h"

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
	{
	}

	virtual ~GammaData()
	{
	}

	const GammaDataType_e type;
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

/**
 * GammaImage class.
 */
class GammaMatrix :
	public GammaData
{
public:
	GammaMatrix()
		: GammaData(GAMMA_DATA_TYPE_MATRIX)
		, eventMax(1)
		, eventSum(0)
		, eventSumIn(0)
		, time(0)
		, span(0)
		, trig(GAMMA_TRIG_NONE)
	{
	}

	~GammaMatrix()
	{
	}

	wxUint32 matrix[256 * 256];
	wxUint32 eventMax;
	wxUint64 eventSum;
	wxUint64 eventSumIn;

	wxTimeSpan time;
	wxTimeSpan span;

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
