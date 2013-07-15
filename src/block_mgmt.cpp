/**
 * @file	block_mgmt.cpp
 * @brief	blocks manager
 * @author	Mateusz Plociennik
 * @data	2012-07-25
 */

#include "block_mgmt.h"

#include "block_fread.h"
#include "block_fwrite.h"
#include "block_tr_us.h"
#include "block_tr_sm.h"
#include "block_tr_mi.h"
#include "block_usb.h"
#include "block_usb_fake.h"
#include "b_nemacalc.h"
#include "b_uniform.h"
#include "matrix_sum.h"

GammaManager::GammaManager(GammaFrame* pFrame)
	: m_pFrame(pFrame)
	, m_pPipeHead(NULL)
{
	m_pFrame->PushEventHandler(this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GammaManager::onEvent, this, 201, 204);
}

GammaManager::~GammaManager()
{
	m_pFrame->RemoveEventHandler(this);
}

void GammaManager::onEvent(wxCommandEvent& event)
{
	wxLogStatus(__FUNCTION__);
}


void GammaManager::setMode(GammaMode_e mode)
{
	switch(mode)
	{
	case GAMMA_MODE_USB_2_FILE:
		{
			m_pPipeHead = new GammaBlockUSB(this);
			GammaPipeFrontEnd* trans = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* file = new GammaBlockFileWrite(this);

			*m_pPipeHead += *trans += *file;
			break;
		}
	case GAMMA_MODE_FAKE_2_FILE:
		{
			m_pPipeHead = new GammaBlockUSBFake(this);
			GammaPipeFrontEnd* trans = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* file = new GammaBlockFileWrite(this);

			*m_pPipeHead += *trans += *file;
			break;
		}
	case GAMMA_MODE_FAKE_2_IMAGE:
		{
			m_pPipeHead = new GammaBlockUSBFake(this);
			GammaPipeFrontEnd* tr_us = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* unif = new GammaUniformity(this);
			GammaPipeFrontEnd* tr_sm = new GammaBlockTransSM(this);
			GammaPipeFrontEnd* tr_mi = new GammaTransMI(this);

			*m_pPipeHead += *tr_us += *unif += *tr_sm += /*nema += */*tr_mi;
			break;
		}
	case GAMMA_MODE_USB_2_IMAGE:
		{
			m_pPipeHead = new GammaBlockUSB(this);
			GammaPipeFrontEnd* tr_us = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* tr_sm = new GammaBlockTransSM(this);
			GammaPipeFrontEnd* tr_mi = new GammaTransMI(this);

			*m_pPipeHead += *tr_us += *tr_sm += *tr_mi;
			break;
		}
	case GAMMA_MODE_USB_2_IMAGE_UNI:
		{
			m_pPipeHead = new GammaBlockUSB(this);
			GammaPipeFrontEnd* tr_us = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* unif = new GammaUniformity(this);
			GammaPipeFrontEnd* tr_sm = new GammaBlockTransSM(this);
			GammaPipeFrontEnd* tr_mi = new GammaTransMI(this);

			*m_pPipeHead += *tr_us += *unif += *tr_sm += *tr_mi;
			break;
		}
	case GAMMA_MODE_USB_FULL:
		{
			m_pPipeHead = new GammaBlockUSB(this);
			GammaPipeFrontEnd* tr_us = new GammaBlockTransUS(this);
			GammaPipeFrontEnd* fwrite = new GammaBlockFileWrite(this);
			GammaPipeFrontEnd* tr_sm = new GammaBlockTransSM(this);
			GammaPipeFrontEnd* tr_mi = new GammaTransMI(this);

			*m_pPipeHead += *tr_us += *fwrite;
			*tr_us += *tr_sm += *tr_mi;

			break;
		}
	case GAMMA_MODE_FILE_2_IMAGE:
		{
			m_pPipeHead = new GammaBlockFileRead(this);
			GammaPipeFrontEnd* unif = new GammaUniformity(this);
			GammaPipeFrontEnd* tr_sm = new GammaBlockTransSM(this);
			//GammaPipeFrontEnd* nema = new GammaNemaCalc(this);
			GammaPipeFrontEnd* tr_mi = new GammaTransMI(this);
			GammaPipeFrontEnd* buff1 = new GammaPipeBuffer(this);
			GammaPipeFrontEnd* buff2 = new GammaPipeBuffer(this);
			GammaPipeFrontEnd* buff3 = new GammaPipeBuffer(this, 8);
			GammaPipeFrontEnd* buff4 = new GammaPipeBuffer(this, 8);
			GammaPipeFrontEnd* m_sum = new GammaMatrixSum(this);

			*m_pPipeHead += *buff1 += *unif += *buff2 += *tr_sm  += *buff3 += *m_sum += *buff4 +=/*nema += */*tr_mi;

			break;
		}
	case GAMMA_MODE_NONE:
	default:
		{
			m_pPipeHead->stopAll();
			m_pPipeHead->deleteAll();
			m_pPipeHead = NULL;
			
			break;
		}
	}

	if(GAMMA_MODE_NONE != mode)
	{
		m_pPipeHead->startAll();
	}
}

////////////////////////////////////////////////////////////////////////////////

int GammaManager::DataTierSetParam(GammaParam_e param, void* value)
{
	return m_pPipeHead->setParamAll(param, value);
}
	
////////////////////////////////////////////////////////////////////////////////

bool GammaManager::PresentationTierSetParam(GammaParam_e param, void* value)
{
	return m_pFrame->SetParam(param, value);
}

GammaConfig* GammaManager::getConfig()
{
	return &m_config;
}

GammaFrame* GammaManager::getFrame()
{
	return m_pFrame;
}
