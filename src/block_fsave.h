/////////////////////////////////////////////////////////////////////////////
// Name:        block_fsave.h
// Purpose:     saving data to a file
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_FSAVE_H_
#define _GAMMA_VIEW_BLOCK_FSAVE_H_

#include "block.h"
#include <wx/file.h>

class GammaBlockFileSave : 
	public GammaBlock < GammaBlockData<std::list<GammaItem>> , void >
{
public:
//	GammaBlockFileSave();
//	~GammaBlockFileSave();

protected:
	wxThread::ExitCode Entry();

private:
	//wxFile file;
};

#endif //_GAMMA_VIEW_BLOCK_FSAVE_H_
