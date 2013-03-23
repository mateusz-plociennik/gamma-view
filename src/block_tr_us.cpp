/**
 * @file	block_tr_us.cpp
 * @brief	translator for RAW data from USB
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_tr_us.h"
#include "config.h"
#include <wx/fileconf.h>

GammaBlockTransUS::GammaBlockTransUS(GammaManager* pManager) 
		: 
		GammaBlockBase(pManager, GAMMA_QUEUE_BLOCK_TRANS_US)
{
	//
}

wxThread::ExitCode GammaBlockTransUS::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	{
		wxConfigBase* config = wxFileConfig::Get();
		wxConfigPathChanger changer(config, "/USBDevice/");
		m_timeDiv = config->ReadLong("Tmarker", 10);
		m_timeCounter = 0;
	}
	while( ShouldBeRunning() )
	{
		if(DataReady())
		{
			wxSharedPtr<GammaDataBase> dataIn(DataGet());
			GammaDataUSB* pDataIn = static_cast<GammaDataUSB*>(dataIn.get());

			GammaDataItems* pDataOut = new GammaDataItems;

			pDataIn->Lock();
			pDataOut->dateTime = pDataIn->dateTime;
			for(uint32_t i = 0; i < 256; i++)
			{
				if( (pDataIn->data[2 * i + 0] == 0xFF) && 
					(pDataIn->data[2 * i + 1] == 0xFF) )
				{
					pDataOut->data.at(i).type = GAMMA_ITEM_TMARKER;
					pDataOut->data.at(i).data.time = m_timeCounter;
					m_timeCounter += m_timeDiv;
				}
				else if( (pDataIn->data[2 * i + 0] == 0x00) && 
					(pDataIn->data[2 * i + 1] == 0x00) )
				{
					pDataOut->data.at(i).type = GAMMA_ITEM_TRIGGER;
					pDataOut->data.at(i).data.time = m_timeCounter;
				}
				else
				{
					pDataOut->data.at(i).type = GAMMA_ITEM_POINT;
					pDataOut->data.at(i).data.point.x = 
						pDataIn->data[2 * i + 0];
					pDataOut->data.at(i).data.point.y = 
						pDataIn->data[2 * i + 1];
					pDataOut->data.at(i).data.point.z = 
						(-1); //Not Available
				}
			}
			pDataIn->Unlock();
			DataPush(pDataOut);
		}
	}

	return 0;
}
