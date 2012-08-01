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

void GammaBlockManager::SetMode(GammaBlockMode_e mode)
{
	switch(mode)
	{
		case GAMMA_MODE_USB_2_FILE:
		{
			GammaBlockBase* usb = new GammaBlockUSB;
			GammaBlockBase* trans = new GammaBlockTransUS;
			GammaBlockBase* file = new GammaBlockFileWrite;
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
			GammaBlockBase* usb = new GammaBlockUSBFake;
			GammaBlockBase* trans = new GammaBlockTransUS;
			GammaBlockBase* file = new GammaBlockFileWrite;
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
			GammaBlockBase* usb = new GammaBlockUSBFake;
			GammaBlockBase* tr_us = new GammaBlockTransUS;
			GammaBlockBase* tr_sm = new GammaBlockTransSM;
			GammaBlockBase* tr_mi = new GammaBlockTransMI;
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
		default:
		{
			while (!m_blockList.empty())
			{
				m_blockList.front()->Stop();
				delete m_blockList.front();
				m_blockList.pop_front();
			}

			break;
		}
	}
}

	/*
void GammaBlockManager::BlocksRun()
{
	
}
*/
