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
		unsigned long int timeCounter = 0;
		while ( (!GetThread()->TestDestroy()) && (!file.Eof()) )
		{
			GammaDataItems* blockDataOut = new GammaDataItems;
			blockDataOut->data.resize(256);
			
			long int timeCounterStart = timeCounter;
			//blockDataOut->Lock();
			for (unsigned short int i = 0; ( (i < 256) && (!file.Eof()) ); i++)
			{
				file.Read(&(blockDataOut->data.at(i)), sizeof(GammaItem));
				if (blockDataOut->data.at(i).type == GAMMA_ITEM_TMARKER)
				{
					timeCounter = blockDataOut->data.at(i).data.time;
				}
			}
			//blockDataOut->Unlock();
			GetThread()->Sleep(timeCounter - timeCounterStart);
			DataPush(blockDataOut);
		}
	}
	file.Close();

	return 0;
}
