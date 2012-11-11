/**
 * @file	block_fread.h
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_FREAD_H_
#define _GAMMA_VIEW_BLOCK_FREAD_H_

#include "block_base.h"
#include <wx/file.h>

class GammaBlockFileRead : 
	public GammaBlockBase
{
public:
	GammaBlockFileRead()
	{
		wxLogStatus("%s", __FUNCTION__);
	}
	~GammaBlockFileRead()
	{
		wxLogStatus("%s", __FUNCTION__);
	}

protected:
	wxThread::ExitCode Entry();
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
