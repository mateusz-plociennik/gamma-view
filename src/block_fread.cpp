/**
 * @file	block_fread.cpp
 * @brief	Reads data from a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fread.h"

wxThread::ExitCode GammaBlockFileRead::Entry()
{
	wxFile file;

	file.Open("data/read.gvb", wxFile::read);
	while ( (!GetThread()->TestDestroy()) && (!file.Eof()) )
	{
		GammaBlockData<std::list<GammaItem>*>* blockDataOut = 
			new GammaBlockData<std::list<GammaItem>*>;

		//blockDataOut->Lock();
		for (unsigned short int i = 0; i < 256; i++)
		{
			if (file.Eof())
			{
				break;
			}
			blockDataOut->data->resize(blockDataOut->data->size() + 1);
			file.Read(&(*(blockDataOut->data->rend())), sizeof(GammaItem));
		}
		//blockDataOut->Unlock();
		BlockDataPush(blockDataOut);
	}
	file.Close();

	return 0;
}
