/**
 * @file	block_data.h
 * @brief	GammaBlockDataBase class declaration.
 * @author	Mateusz Plociennik
 * @data	2012-07-20
 */

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

#include <vector>
#include <stdint.h>

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
	GammaData()
	{
	}

	virtual ~GammaData()
	{
	}
};

/**
 * GammaDataUSB class.
 */
class GammaDataUSB :
	public GammaData
{
public:
	GammaDataUSB()
	{
		data = new wxUint8[256*256]();
	}

	~GammaDataUSB()
	{
		delete[] data;
	}

	wxUint8* data;
};

/**
 * GammaItems class.
 */
class GammaItems :
	public GammaData
{
public:
	GammaItems()
	{
		items.resize(256);
	}

	~GammaItems()
	{
		items.clear();
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
		: eventMax(1)
		, eventSum(0)
	{
		matrix = new wxUint32[256 * 256]();
	}

	GammaMatrix(const GammaMatrix& that)
	{
		matrix = new wxUint32[256 * 256];
		memcpy(matrix, that.matrix, sizeof(wxUint32) * 256 * 256);
	}

	~GammaMatrix()
	{
		delete[] matrix;
	}

	wxUint32* matrix;
	wxUint32 eventMax;
	wxUint64 eventSum;

	wxTimeSpan time;
	wxTimeSpan span;
};

/**
 * GammaImage class.
 */
class GammaImage :
	public GammaData
{
public:
	GammaImage()
	{
		image = new wxImage(256, 256, true);
	}

	~GammaImage()
	{
		delete image;
	}

	wxImage* image;
};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_
