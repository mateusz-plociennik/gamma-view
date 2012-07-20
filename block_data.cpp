/////////////////////////////////////////////////////////////////////////////
// Name:        block_data.cpp
// Purpose:     data container for GammaBlock
// Author:      Mateusz Plociennik
// Created:     20/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_data.h"

template<typename Data>
GammaBlockData<Data>::GammaBlockData()
{
	m_refCount = 0;
}

template<typename Data>
GammaBlockData<Data>::~GammaBlockData()
{
	delete[] data;
}
