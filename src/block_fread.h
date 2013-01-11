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
#include <wx/filename.h>

class GammaBlockFileRead : 
	public GammaBlockBase
{
public:
	GammaBlockFileRead(GammaManager* pManager) :
			GammaBlockBase(pManager)
	{
		wxLogStatus("%s", __FUNCTION__);
	}
	~GammaBlockFileRead()
	{
		wxLogStatus("%s", __FUNCTION__);
	}

	bool SetParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
	bool CheckHeader();
	wxTimeSpan GetTimeEnd();

	wxFileName m_fileName;
	wxFile m_file;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
