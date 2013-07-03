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
	m_timeDiv = 10;//config->ReadLong("Tmarker", 10);
	m_timeCounter = 0;
}

void GammaBlockTransUS::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_USB == pData->type);
	GammaDataUSB* pDataIn = dynamic_cast<GammaDataUSB*>(pData);
	GammaItems* pDataOut = new GammaItems;

	//pDataOut->dateTime = pDataIn->dateTime;
	for(wxUint32 i = 0; i < 256; i++)
	{
		if( (pDataIn->data[2 * i + 0] == 0xFF) && 
			(pDataIn->data[2 * i + 1] == 0xFF) )
		{
			pDataOut->items[i].type = GAMMA_ITEM_TYPE_TMARKER;
			pDataOut->items[i].data.time = m_timeCounter;
			m_timeCounter += m_timeDiv;
		}
		else if( (pDataIn->data[2 * i + 0] == 0x00) && 
			(pDataIn->data[2 * i + 1] == 0x00) )
		{
			pDataOut->items[i].type = GAMMA_ITEM_TYPE_TRIGGER;
			pDataOut->items[i].data.time = m_timeCounter;
		}
		else
		{
			pDataOut->items[i].type = GAMMA_ITEM_TYPE_POINT;
			pDataOut->items[i].data.point.x = 
				pDataIn->data[2 * i + 0];
			pDataOut->items[i].data.point.y = 
				pDataIn->data[2 * i + 1];
			pDataOut->items[i].data.point.z = 
				(-1); //Not Available
		}
	}

	pushData(pDataOut);
	delete pDataOut;
}

wxInt32 GammaBlockTransUS::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_SET_TMARKER: 
		switch(*static_cast<GammaSettingTmarker_e*>(value))
		{
		case GAMMA_TMARKER_OFF:
			m_timeDiv = 0; break;
		case GAMMA_TMARKER_1MS:
			m_timeDiv = 1; break;
		case GAMMA_TMARKER_10MS:
			m_timeDiv = 10; break;
		case GAMMA_TMARKER_100MS:
			m_timeDiv = 100; break;
		case GAMMA_TMARKER_240MS:
			m_timeDiv = 240; break;
		default:
			break;
		}

		break;
	default:
		return 0;
	}

	return 1;
}
