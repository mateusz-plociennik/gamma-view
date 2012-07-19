/////////////////////////////////////////////////////////////////////////////
// Name:        block_usb.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_usb.h"

void GammaBlockUSB::BlockRun()
{
	m_running = TRUE;
	Create();
	SetPriority(m_priority);
	Run();
}

void GammaBlockUSB::BlockStop()
{
	m_running = FALSE;
	Wait();
}

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
	USBSet(GAMMA_SET_T_MARKER, GAMMA_T_MARKER_1MS) &&
	USBSet(GAMMA_SET_GATE_ON, 0) );

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
	m_length = 2;
	m_buffer[0] = setting;
	m_buffer[1] = value;
	return m_USBDevice->BulkOutEndPt->XferData(m_buffer, m_length);
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	while (m_running)
	{
		m_length = 512;
		memset(m_buffer, 0, sizeof(m_buffer));
		m_USBDevice->BulkInEndPt->XferData(m_buffer, m_length);
		
		unsigned char* t_buffer = new unsigned char[512];
		memcpy(t_buffer, m_buffer, sizeof(m_buffer));
		
		gammaData* data = new gammaData;
		data->data_p = t_buffer;
		data->length = sizeof(t_buffer);
		data->type = GAMMA_DATA_USB_RAW;
		
		m_dataOutList.push_back(data);
		DataSend();
	}

	return 0;
}

GammaBlockUSB::GammaBlockUSB()
{
	m_buffer = new UCHAR[512];
	m_priority = 75;
	m_USBDevice = new CCyUSBDevice(NULL);
}

GammaBlockUSB::~GammaBlockUSB()
{
	
}
