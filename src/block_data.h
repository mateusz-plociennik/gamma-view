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
class GammaDataBase : 
	public wxMutex
{
public:
	/**
	 * Default constructor.
	 */
	GammaDataBase() :
			wxMutex(wxMUTEX_DEFAULT)
	{
	}

	virtual ~GammaDataBase()
	{
	}

	/**
	 * Date and time of packet arrival.
	 */
	wxDateTime dateTime;
};

/**
 * GammaDataUSB class.
 */
class GammaDataUSB :
	public GammaDataBase
{
public:
	GammaDataUSB()
	{
		data = new unsigned char[0x200]();
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
class GammaDataItems :
	public GammaDataBase
{
public:
	GammaDataItems()
	{
		data.resize(0x100);
	}

	~GammaDataItems()
	{
		data.clear();
	}

	wxVector<GammaItem> data;
};

/**
 * GammaDataImage class.
 */
class GammaDataMatrix :
	public GammaDataBase
{
public:
	GammaDataMatrix()
	{
		data = new unsigned short int[0x10000]();
		max = 0;
	}

	~GammaDataMatrix()
	{
		delete[] data;
	}

	unsigned short int* data;
	unsigned short int max;
};

/**
 * GammaDataImage class.
 */
class GammaDataImage :
	public GammaDataBase
{
public:
	GammaDataImage()
	{
		data = new wxImage(0x100, 0x100, true);
	}

	~GammaDataImage()
	{
		delete data;
	}

	wxImage* data;
};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_
