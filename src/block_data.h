/////////////////////////////////////////////////////////////////////////////
// Name:        block_data.h
// Purpose:     data container for GammaBlock
// Author:      Mateusz Plociennik
// Created:     20/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_DATA_H_
#define _GAMMA_VIEW_BLOCK_DATA_H_

//#include <wx/wx.h>

template<typename Data>
class GammaBlockData
{
public:
	GammaBlockData();
	~GammaBlockData();

//	wxDateTime datetime;
//	wxMutex dataMutex;
	int datetime;
	Data data;

private:
	unsigned char m_refCount;

};

#endif //_GAMMA_VIEW_BLOCK_DATA_H_