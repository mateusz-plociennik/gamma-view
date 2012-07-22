/////////////////////////////////////////////////////////////////////////////
// Name:        block_data.h
// Purpose:     blockData container for GammaBlockBase
// Author:      Mateusz Plociennik
// Created:     20/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

//#include <wx/wx.h>

template<typename BlockData>
class GammaBlockData
{
public:
	GammaBlockData();
	~GammaBlockData();

//	wxDateTime datetime;
//	wxMutex dataMutex;
	int datetime;
	BlockData data;

private:
	unsigned char m_refCount;

};

template<typename BlockData>
GammaBlockData<BlockData>::GammaBlockData()
{
	m_refCount = 0;
}

template<typename BlockData>
GammaBlockData<BlockData>::~GammaBlockData()
{
	delete[] data;
}

#endif //_GAMMA_VIEW_BLOCK_DATA_H_