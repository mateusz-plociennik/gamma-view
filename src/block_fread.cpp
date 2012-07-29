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

	file.Open("read.gvb", wxFile::read);
	char tBuffer[3];
	file.Read(tBuffer, 3);
	if (!strcmp(tBuffer, "GVB"))
	{
		while ( (!GetThread()->TestDestroy()) && (!file.Eof()) )
		{
			GammaDataItems* blockDataOut = new GammaDataItems;

			//blockDataOut->Lock();
			for (unsigned short int i = 0; ( (i < 256) && (!file.Eof()) ); i++)
			{
				blockDataOut->data->resize(blockDataOut->data->size() + 1);
				file.Read(&(*(blockDataOut->data->rend())), sizeof(GammaItem));
			}
			//blockDataOut->Unlock();
			DataPush(blockDataOut);
		}
	}
	file.Close();

	return 0;
}
