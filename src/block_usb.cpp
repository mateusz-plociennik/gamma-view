/////////////////////////////////////////////////////////////////////////////
// Name:        block_usb.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_usb.h"
#include "block_trans.h"

void GammaBlockUSB::USBClose()
{
	m_USBDevice->Close();
}

bool GammaBlockUSB::USBInit()
{
	bool ret = (
	USBOpen() &&
	USBReset() &&
	USBSet(GAMMA_SET_ZOOM, 0x80) &&
	USBSet(GAMMA_SET_SHIFT_X, 0x80) &&
	USBSet(GAMMA_SET_SHIFT_Y, 0x80) &&
	USBSet(GAMMA_SET_TMARKER, GAMMA_TMARKER_10MS) &&
	USBSet(GAMMA_SET_GATE, 0) );
	
	static_cast<GammaBlockTrans*> (m_blockList.front())->SetTimeDiv(10);
	return ret;
}

bool GammaBlockUSB::USBOpen()
{
	return m_USBDevice->Open(NULL);
}

bool GammaBlockUSB::USBReset()
{
	return m_USBDevice->Reset();
}

bool GammaBlockUSB::USBSet(unsigned char setting, unsigned char value)
{
	long int length = 2;
	unsigned char buffer[] = {setting, value};

	return m_USBDevice->BulkOutEndPt->XferData(buffer, length);
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		long int length = 512;
		GammaBlockData<unsigned char*>* blockDataOut = new GammaBlockData<unsigned char*>;
		blockDataOut->data = new unsigned char[512];
		
		blockDataOut->datetime = 0;

		m_USBDevice->BulkInEndPt->XferData(blockDataOut->data, length);
		
		BlockDataPush(blockDataOut);
	}

	return 0;
}

GammaBlockUSB::GammaBlockUSB()
{
	m_USBDevice = new CCyUSBDevice(NULL);
}

GammaBlockUSB::~GammaBlockUSB()
{
	
}


