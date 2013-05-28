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

class GammaBlockFileRead : public GammaPipeHead
{
public:
	GammaBlockFileRead(GammaManager* pManager);
	~GammaBlockFileRead();

	bool setParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
	bool checkHeader();
	wxTimeSpan getTime();
	wxTimeSpan getEndTime();
	bool setTime(wxTimeSpan reqTime);

	wxFileName m_fileName;
	wxFile m_file;
	wxUint32 m_intgTime;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
