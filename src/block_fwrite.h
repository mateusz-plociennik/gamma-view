/**
 * @file	block_fwrite.h
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_FWRITE_H_
#define _GAMMA_VIEW_BLOCK_FWRITE_H_

#include "block_base.h"
#include <wx/file.h>

class GammaBlockFileWrite : 
	public GammaBlockBase
{
public:
//	GammaBlockFileSave();
//	~GammaBlockFileSave();

protected:
	wxThread::ExitCode Entry();
};

#endif //_GAMMA_VIEW_BLOCK_FWRITE_H_
