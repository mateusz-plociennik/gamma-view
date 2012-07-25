/**
 * @file	block_mgmt.cpp
 * @brief	blocks manager
 * @author	Mateusz Plociennik
 * @data	2012-07-25
 */

#include "block_mgmt.h"

#include "block_fread.h"
#include "block_fwrite.h"
#include "block_trans.h"
#include "block_usb.h"

void GammaBlockManager::SetMode(GammaBlockMode_e mode)
{
	switch(mode)
	{
		case GAMMA_MODE_USB_2_FILE:
		{
			GammaBlockBase* usb = new GammaBlockUSB;
			GammaBlockBase* trans = new GammaBlockTrans;
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
		default:
		{
			m_blockList.front()->Stop();

			break;
		}
	}
}

	/*
void GammaBlockManager::BlocksRun()
{
	
}
*/
