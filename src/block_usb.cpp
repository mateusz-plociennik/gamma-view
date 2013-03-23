/**
 * @file	block_usb.cpp
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_usb.h"
#include "block_tr_us.h"
#include <wx/fileconf.h>

bool GammaBlockUSB::DeviceFind()
{
#ifdef _WIN32
	for( m_device = 0; m_device < m_USBDevice->DeviceCount(); m_device++ )
	{
		m_USBDevice->Open(m_device);
		
		if( (m_USBDevice->VendorID == 0x04B4) && 
			(m_USBDevice->ProductID == 0x8813) )
		{
			break;
		}
	}
	
	if(m_device < m_USBDevice->DeviceCount())
	{
		wxLogStatus("Device found (VID_%04X&PID_%04X).", 
			m_USBDevice->VendorID, m_USBDevice->ProductID);
		return true;
	}
	else
	{
		wxLogStatus("No device found.");
		return false;
	}
#else
	return true;
#endif
}

bool GammaBlockUSB::DeviceInit()
{
	wxConfigBase* config = wxFileConfig::Get();
	wxConfigPathChanger changer(config, "/USBDevice/");

#ifdef _WIN32	
	if( DeviceReset() &&
		DeviceSet(GAMMA_SET_ZOOM, config->ReadLong("Zoom", 128)) &&
		DeviceSet(GAMMA_SET_SHIFT_X, config->ReadLong("ShiftX", 128)) &&
		DeviceSet(GAMMA_SET_SHIFT_Y, config->ReadLong("ShiftY", 128)) &&
		DeviceSet(GAMMA_SET_TMARKER, log10(config->ReadDouble("Tmarker", 10)) + 1) &&
		DeviceSet(GAMMA_SET_GATE, config->ReadLong("GateOn", 0)) )
	{
		wxLogStatus("Device init OK.");
		return true;
	}
	else
	{
		wxLogStatus("Device init FAILED.");
		return false;
	}
#else
	return true;
#endif
}

bool GammaBlockUSB::DeviceReset()
{
#ifdef _WIN32
	return m_USBDevice->Reset();
#else
	return true;
#endif
}

bool GammaBlockUSB::DeviceSet(unsigned char setting, unsigned char value)
{
#ifdef _WIN32
	unsigned char buffer[] = {setting, value};
	long int length = 2;

	return m_USBDevice->BulkOutEndPt->XferData(buffer, length);
#else
	return true;
#endif
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

#ifdef _WIN32
	if( DeviceFind() && DeviceInit() )
	{
		while( ShouldBeRunning() )
		{
			GammaDataUSB* pDataOut(new GammaDataUSB);
			long int length = 0x200;

			pDataOut->dateTime = wxDateTime::UNow();
			m_USBDevice->BulkInEndPt->XferData(pDataOut->data, length);
			wxASSERT(0x200 == length);
			
			DataPush(pDataOut);
		}
	}
#endif
	return 0;
}

GammaBlockUSB::GammaBlockUSB(GammaManager* pManager) :
	GammaBlockBase(pManager, 0)
{
#ifdef _WIN32
	m_USBDevice = new CCyUSBDevice(NULL);
#endif
}

GammaBlockUSB::~GammaBlockUSB()
{
#ifdef _WIN32
	delete m_USBDevice;
#endif
}
