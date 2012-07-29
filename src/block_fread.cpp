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
		long int m_timeCounter = 0;
		while ( (!GetThread()->TestDestroy()) && (!file.Eof()) )
		{
			GammaDataItems* blockDataOut = new GammaDataItems;
			blockDataOut->data.resize(256);
			
			long int m_timeCounterStart = m_timeCounter;
			//blockDataOut->Lock();
			for (unsigned short int i = 0; ( (i < 256) && (!file.Eof()) ); i++)
			{
				file.Read(&(blockDataOut->data[i]), sizeof(GammaItem));
				if (blockDataOut->data[i].type == GAMMA_ITEM_TMARKER)
				{
					m_timeCounter = blockDataOut->data[i].data.time;
				}
			}
			//blockDataOut->Unlock();
			GetThread()->Sleep(m_timeCounter - m_timeCounterStart);
			DataPush(blockDataOut);
		}
	}
	file.Close();

	return 0;
}
