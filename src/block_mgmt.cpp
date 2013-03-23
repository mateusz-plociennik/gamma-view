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

void GammaManager::SetMode(GammaMode_e mode)
{
	switch(mode)
	{
	case GAMMA_MODE_USB_2_FILE:
		{
			GammaBlockBase* usb = new GammaBlockUSB(this);
			GammaBlockBase* trans = new GammaBlockTransUS(this);
			GammaBlockBase* file = new GammaBlockFileWrite(this);
			m_blockList.push_back(usb);
			m_blockList.push_back(trans);
			m_blockList.push_back(file);

			trans->BlockAttach(file);
			usb->BlockAttach(trans);
			
			file->Run();
			trans->Run();
			usb->Run();
			break;
		}
	case GAMMA_MODE_FAKE_2_FILE:
		{
			GammaBlockBase* usb = new GammaBlockUSBFake(this);
			GammaBlockBase* trans = new GammaBlockTransUS(this);
			GammaBlockBase* file = new GammaBlockFileWrite(this);
			m_blockList.push_back(usb);
			m_blockList.push_back(trans);
			m_blockList.push_back(file);

			trans->BlockAttach(file);
			usb->BlockAttach(trans);
			
			file->Run();
			trans->Run();
			usb->Run();
			break;
		}
	case GAMMA_MODE_FAKE_2_IMAGE:
		{
			GammaBlockBase* usb = new GammaBlockUSBFake(this);
			GammaBlockBase* tr_us = new GammaBlockTransUS(this);
			GammaBlockBase* tr_sm = new GammaBlockTransSM(this);
			GammaBlockBase* tr_mi = new GammaBlockTransMI(this);
			m_blockList.push_back(usb);
			m_blockList.push_back(tr_us);
			m_blockList.push_back(tr_sm);
			m_blockList.push_back(tr_mi);

			tr_sm->BlockAttach(tr_mi);
			tr_us->BlockAttach(tr_sm);
			usb->BlockAttach(tr_us);

			tr_mi->Run();
			tr_sm->Run();
			tr_us->Run();
			usb->Run();
			break;
		}
	case GAMMA_MODE_USB_2_IMAGE:
		{
			GammaBlockBase* usb = new GammaBlockUSB(this);
			GammaBlockBase* tr_us = new GammaBlockTransUS(this);
			GammaBlockBase* tr_sm = new GammaBlockTransSM(this);
			GammaBlockBase* tr_mi = new GammaBlockTransMI(this);
			m_blockList.push_back(usb);
			m_blockList.push_back(tr_us);
			m_blockList.push_back(tr_sm);
			m_blockList.push_back(tr_mi);

			tr_sm->BlockAttach(tr_mi);
			tr_us->BlockAttach(tr_sm);
			usb->BlockAttach(tr_us);

			tr_mi->Run();
			tr_sm->Run();
			tr_us->Run();
			usb->Run();
			break;
		}
	case GAMMA_MODE_USB_FULL:
		{
			GammaBlockBase* usb = new GammaBlockUSB(this);
			GammaBlockBase* tr_us = new GammaBlockTransUS(this);
			GammaBlockBase* write = new GammaBlockFileWrite(this);
			GammaBlockBase* tr_sm1 = new GammaBlockTransSM(this);
			GammaBlockBase* tr_mi1 = new GammaBlockTransMI(this);
			GammaBlockBase* tr_sm2 = new GammaBlockTransSM(this);
			GammaBlockBase* tr_mi2 = new GammaBlockTransMI(this);


			m_blockList.push_back(usb);
			m_blockList.push_back(tr_us);
			m_blockList.push_back(write);
			m_blockList.push_back(tr_sm1);
			m_blockList.push_back(tr_mi1);
			m_blockList.push_back(tr_sm2);
			m_blockList.push_back(tr_mi2);

			tr_sm1->BlockAttach(tr_mi1);
			tr_us->BlockAttach(tr_sm1);
			tr_sm2->BlockAttach(tr_mi2);
			tr_us->BlockAttach(tr_sm2);
			tr_us->BlockAttach(write);
			usb->BlockAttach(tr_us);

			tr_mi1->Run();
			tr_sm1->Run();
			tr_mi2->Run();
			tr_sm2->Run();
			write->Run();
			tr_us->Run();
			usb->Run();
			break;
		}
	case GAMMA_MODE_FILE_2_IMAGE:
		{
			GammaBlockBase* file = new GammaBlockFileRead(this);
			GammaBlockBase* unif = new GammaUniformity(this);
			GammaBlockBase* tr_sm = new GammaBlockTransSM(this);
			GammaBlockBase* nema = new GammaNemaCalc(this);
			GammaBlockBase* tr_mi = new GammaBlockTransMI(this);
			
			m_blockList.push_back(file);
			m_blockList.push_back(unif);
			m_blockList.push_back(tr_sm);
			m_blockList.push_back(nema);
			m_blockList.push_back(tr_mi);

			nema->BlockAttach(tr_mi);
			tr_sm->BlockAttach(nema); //tr_mi
			unif->BlockAttach(tr_sm);
			file->BlockAttach(unif);


			tr_mi->Run();
			nema->Run();
			tr_sm->Run();
			unif->Run();
			file->Run();
			break;
		}
	case GAMMA_MODE_NONE:
	default:
		{
			while(!m_blockList.empty())
			{
				m_blockList.front()->Stop();
				delete m_blockList.front();
				m_blockList.pop_front();
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

int GammaManager::DataTierSetParam(GammaParam_e param, void* value)
{
	int ret = 0;

	for( std::list<GammaBlockBase*>::iterator iBlock = m_blockList.begin();
		iBlock != m_blockList.end(); iBlock++ )
	{
		if( (*iBlock)->SetParam(param, value) )
		{
			ret++;
		}
	}

	return ret;
}
	
////////////////////////////////////////////////////////////////////////////////

bool GammaManager::PresentationTierSetParam(GammaParam_e param, void* value)
{
	return m_pFrame->SetParam(param, value);
}

/*
void GammaBlockManager::BlocksRun()
{
	
}
*/
