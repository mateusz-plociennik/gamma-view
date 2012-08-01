/**
 * @file	block_tr_us.cpp
 * @brief	translator for RAW data from USB
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_tr_us.h"
#include <wx/fileconf.h>

wxThread::ExitCode GammaBlockTransUS::Entry()
{
	{
		wxConfigBase* config = wxFileConfig::Get();
		wxConfigPathChanger changer(config, "/USBDevice/");
		m_timeDiv = config->ReadLong("Tmarker", 10);
		m_timeCounter = 0;
	}
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataUSB* blockDataIn = (GammaDataUSB*)DataGet();

			GammaDataItems* blockDataOut = new GammaDataItems;

			blockDataIn->Lock();
			blockDataOut->datetime = blockDataIn->datetime;
			for (unsigned short int i = 0; i < 256; i++)
			{
				if ( (blockDataIn->data[2 * i + 0] == 0xFF) && 
					(blockDataIn->data[2 * i + 1] == 0xFF) )
				{
					blockDataOut->data[i].type = GAMMA_ITEM_TMARKER;
					blockDataOut->data[i].data.time = m_timeCounter;
					m_timeCounter += m_timeDiv;
					continue;
				}
				else if ( (blockDataIn->data[2 * i + 0] == 0x00) && 
					(blockDataIn->data[2 * i + 1] == 0x00) )
				{
					blockDataOut->data[i].type = GAMMA_ITEM_TRIGGER;
					blockDataOut->data[i].data.time = m_timeCounter;
					continue;
				}
				else
				{
					blockDataOut->data[i].type = GAMMA_ITEM_POINT;
					blockDataOut->data[i].data.point.x = 
						blockDataIn->data[2 * i + 0];
					blockDataOut->data[i].data.point.y = 
						blockDataIn->data[2 * i + 1];
					blockDataOut->data[i].data.point.z = 
						(-1); //Not Available
				}
			}
			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();
			DataPush(blockDataOut);
		}
	}

	return 0;
}
