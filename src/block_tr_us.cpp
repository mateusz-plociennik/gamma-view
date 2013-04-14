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
	: GammaPipeSegment(pManager)
{
	wxConfigBase* config = wxFileConfig::Get();
	wxConfigPathChanger changer(config, "/USBDevice/");
	m_timeDiv = config->ReadLong("Tmarker", 10);
	m_timeCounter = 0;
}

void GammaBlockTransUS::processData(GammaDataBase* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaDataUSB* pDataIn = dynamic_cast<GammaDataUSB*>(pData);
	GammaDataItems* pDataOut = new GammaDataItems;

	//pDataOut->dateTime = pDataIn->dateTime;
	for(wxUint32 i = 0; i < 256; i++)
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

	pushData(pDataOut);
	delete pDataOut;
}
