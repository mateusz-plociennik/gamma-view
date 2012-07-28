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
	wxConfigPathChanger changer(config, "/USBDevice");

	long tZoom;
	long tShiftX;
	long tShiftY;
	long tTmarker;
	long tGateOn;

	config->Read("Zoom", &tZoom, 128);
	config->Read("ShiftX", &tShiftX, 128);
	config->Read("ShiftY", &tShiftY, 128);
	config->Read("Tmarker", &tTmarker, 10);
	config->Read("GateOn", &tGateOn, 0);
	
	bool ret = (
	DeviceReset() &&
	DeviceSet(GAMMA_SET_ZOOM, tZoom) &&
	DeviceSet(GAMMA_SET_SHIFT_X, tShiftX) &&
	DeviceSet(GAMMA_SET_SHIFT_Y, tShiftY) &&
	DeviceSet(GAMMA_SET_TMARKER, log10((float)tTmarker) + 1) &&
	DeviceSet(GAMMA_SET_GATE, tGateOn) );
	
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
	if (m_device != GAMMA_NO_DEVICE)
	{
		DeviceInit();
		while (!GetThread()->TestDestroy())
		{
			GammaDataUSB* blockDataOut = new GammaDataUSB;
			blockDataOut->data = new unsigned char[512];
			long int length = 512;
			
			blockDataOut->datetime.UNow();
			m_USBDevice->BulkInEndPt->XferData(blockDataOut->data, length);
			
			BlockDataPush(blockDataOut);
		}
	}

	return 0;
}

GammaBlockUSB::GammaBlockUSB()
{
	m_USBDevice = new CCyUSBDevice(NULL);

	DeviceFind();
}

GammaBlockUSB::~GammaBlockUSB()
{
	delete m_USBDevice;
}


