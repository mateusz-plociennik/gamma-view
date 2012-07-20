/////////////////////////////////////////////////////////////////////////////
// Name:        block_fsave.h
// Purpose:     saving data to a file
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_FSAVE_H_
#define _GAMMA_VIEW_BLOCK_FSAVE_H_

#include "block.h"

class GammaBlockFileSave : 
	public GammaBlock < GammaBlockData<std::list<GammaItem>> , void >
{

};

#endif //_GAMMA_VIEW_BLOCK_FSAVE_H_
