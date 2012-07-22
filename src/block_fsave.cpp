/////////////////////////////////////////////////////////////////////////////
// Name:        block_fsave.h
// Purpose:     saving data to a file
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_fsave.h"

wxThread::ExitCode GammaBlockFileSave::Entry()
{
	wxFile file;

	file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M.gvb"), wxFile::write);
	while (!GetThread()->TestDestroy())
	{
		
		
	}

	return 0;
}
