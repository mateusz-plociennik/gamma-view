/**
 * @file	block_usb.cpp
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_usb.h"
#include "block_trans.h"
#include <wx/fileconf.h>

bool GammaBlockUSB::DeviceFind()
{
	for ( m_device = (m_USBDevice->DeviceCount() - 1); 
		m_device > -1; m_device-- )
	{
		m_USBDevice->Open(m_device);
		
		if ( (m_USBDevice->VendorID == 0x04B4) && 
			(m_USBDevice->ProductID == 0x8813) )
		{
			break;
		}
	}
	
	if (m_device != GAMMA_NO_DEVICE)
	{
		wxLogStatus("Device found (VID_%04X&PID_%04X).", 
			m_USBDevice->VendorID, m_USBDevice->ProductID);
		return TRUE;
	}
	else
	{
		wxLogStatus("No device found!");
		return FALSE;
	}
}

bool GammaBlockUSB::DeviceInit()
{
	wxConfigBase* config = wxFileConfig::Get();
	wxConfigPathChanger changer(config, "/USBDevice/");
	
	bool ret = (
	DeviceReset() &&
	DeviceSet(GAMMA_SET_ZOOM, config->ReadLong("Zoom", 128)) &&
	DeviceSet(GAMMA_SET_SHIFT_X, config->ReadLong("ShiftX", 128)) &&
	DeviceSet(GAMMA_SET_SHIFT_Y, config->ReadLong("ShiftY", 128)) &&
	DeviceSet(GAMMA_SET_TMARKER, log10(config->ReadDouble("Tmarker", 10)) + 1) &&
	DeviceSet(GAMMA_SET_GATE, config->ReadLong("GateOn", 0)) );
	
	return ret;
}

bool GammaBlockUSB::DeviceReset()
{
	return m_USBDevice->Reset();
}

bool GammaBlockUSB::DeviceSet(unsigned char setting, unsigned char value)
{
	unsigned char buffer[] = {setting, value};
	long int length = 2;

	return m_USBDevice->BulkOutEndPt->XferData(buffer, length);
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	if (DeviceFind())
	{
		if (DeviceInit())
		{
			wxLogStatus("Init OK.");
		}
		while (!GetThread()->TestDestroy())
		{
			GammaDataUSB* blockDataOut = new GammaDataUSB;
			blockDataOut->data = new unsigned char[512];
			long int length = 512;
			
			blockDataOut->datetime.UNow();

			m_USBDevice->BulkInEndPt->XferData(blockDataOut->data, length);
			
			DataPush(blockDataOut);
		}
	}

	return 0;
}

GammaBlockUSB::GammaBlockUSB()
{
	m_USBDevice = new CCyUSBDevice(NULL);
}

GammaBlockUSB::~GammaBlockUSB()
{
	delete m_USBDevice;
}
