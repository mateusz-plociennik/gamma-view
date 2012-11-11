/**
 * @file	block_fread.cpp
 * @brief	Reads data from a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fread.h"

wxThread::ExitCode GammaBlockFileRead::Entry()
{
	wxLogStatus("%s - started", __FUNCTION__);
	wxFile file;

	file.Open("20120813_184157.gvb", wxFile::read);
	char tBuffer[3];
	file.Read(tBuffer, 3);
	if (!strncmp(tBuffer, "GVB", 3))
	{
		unsigned long int timeCounter = 0;
		unsigned char loaded = 0;

		while ( (!GetThread()->TestDestroy()) && (!file.Eof()) )
		{
			if (GetThread()->TestDestroy())
			{
				break;
			}
			GammaDataItems* blockDataOut = new GammaDataItems;
			
			long int timeCounterStart = timeCounter;
			for (unsigned short int i = 0; ( (i < 256) && (!file.Eof()) ); i++)
			{
				file.Read(&(blockDataOut->data.at(i)), sizeof(GammaItem));
				if (blockDataOut->data.at(i).type == GAMMA_ITEM_TMARKER)
				{
					timeCounter = blockDataOut->data.at(i).data.time;
				}
			}
			//GetThread()->Sleep(timeCounter - timeCounterStart);
			DataPush(blockDataOut);

			if ( loaded < (100 * file.Tell() / file.Length()) )
			{
				loaded = (100 * file.Tell() / file.Length());
				wxLogStatus("%s - loading (%u%%)", __FUNCTION__, loaded);
			}
		}
	}
	file.Close();

	wxLogStatus("%s - stoped", __FUNCTION__);

	return 0;
}
