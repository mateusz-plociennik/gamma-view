/////////////////////////////////////////////////////////////////////////////
// Name:        driver.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_usb.h"
#include "data_types.h"

GammaBlockUSB::GammaBlockUSB()
{
	m_priority = 75;
	m_USBDevice = new CCyUSBDevice(NULL);
}

GammaBlockUSB::~GammaBlockUSB()
{

}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	while (m_running)
	{
		DataSend();
	}
	return 0;
}
