/**
 * @file	block_fwrite.cpp
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fwrite.h"

wxThread::ExitCode GammaBlockFileWrite::Entry()
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
	file.Close();

	return 0;
}
