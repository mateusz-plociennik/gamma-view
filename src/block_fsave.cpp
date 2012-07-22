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
		GammaBlockData<std::list<GammaItem>*>* blockDataIn = 
			static_cast<GammaBlockData<std::list<GammaItem>*>*> (BlockDataGet());

		blockDataIn->Lock();
		for ( std::list<GammaItem>::iterator i = blockDataIn->data->begin();
			i != blockDataIn->data->end(); i++ )
		{
			file.Write(&(*i), sizeof(GammaItem));
		}
		blockDataIn->Unlock();
		blockDataIn->Unsubscribe();
	}

	return 0;
}
