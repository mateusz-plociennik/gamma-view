/**
 * @file	block_usb.cpp
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_usb.h"
#include "block_tr_us.h"
#include <wx/fileconf.h>

bool GammaBlockUSB::deviceFind()
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

bool GammaBlockUSB::deviceInit()
{
	wxConfigBase* config = wxFileConfig::Get();
	wxConfigPathChanger changer(config, "/USBDevice/");

#ifdef _WIN32	
	if( deviceReset() &&
		deviceSet(GAMMA_SET_ZOOM, config->ReadLong("Zoom", 127)) &&
		deviceSet(GAMMA_SET_SHIFT_X, config->ReadLong("ShiftX", 127)) &&
		deviceSet(GAMMA_SET_SHIFT_Y, config->ReadLong("ShiftY", 127)) &&
		deviceSet(GAMMA_SET_TMARKER, log10(config->ReadDouble("Tmarker", 10)) + 1) &&
		deviceSet(GAMMA_SET_GATE, config->ReadLong("GateOn", 0)) )
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

bool GammaBlockUSB::deviceReset()
{
#ifdef _WIN32
	if(m_USBDevice->IsOpen())
	{
		return m_USBDevice->Reset();
	}
	else
	{
		return false;
	}
#else
	return true;
#endif
}

bool GammaBlockUSB::deviceSet(GammaSetting_e setting, wxUint8 value)
{
#ifdef _WIN32
	if(m_USBDevice->IsOpen())
	{
		UCHAR buffer[] = {static_cast<UCHAR>(setting), value};
		LONG length = sizeof(buffer);

		return m_USBDevice->BulkOutEndPt->XferData(buffer, length);
	}
	else
	{
		return false;
	}
#else
	return true;
#endif
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	//wxMutexLocker locker(m_processDataMutex);

#ifdef _WIN32
	if(deviceFind() && deviceInit() && m_USBDevice->IsOpen())
	{
		while( shouldBeRunning() )
		{
			GammaDataUSB* pDataOut(new GammaDataUSB);
			long int length = 0x200;

			m_USBDevice->BulkInEndPt->XferData(pDataOut->data, length);
			wxASSERT(0x200 == length);
			
			pushData(pDataOut);
			delete pDataOut;
		}
	}
#endif
	return 0;
}

GammaBlockUSB::GammaBlockUSB(GammaManager* pManager) : GammaPipeHead(pManager)
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

wxInt32 GammaBlockUSB::setParam(GammaParam_e param, void* value)
{
	wxUint8 value8 = *static_cast<wxUint8*>(value);
	GammaSetting_e setting = GAMMA_SET_MAX;

	switch(param)
	{
	case GAMMA_PARAM_SET_ZOOM: 
		setting = GAMMA_SET_ZOOM; break;
	case GAMMA_PARAM_SET_SHIFT_X:
		setting = GAMMA_SET_SHIFT_X; break;
	case GAMMA_PARAM_SET_SHIFT_Y:
		setting = GAMMA_SET_SHIFT_Y; break;
	case GAMMA_PARAM_SET_TMARKER:
		{
			setting = GAMMA_SET_TMARKER;
			GammaSettingTmarker_e tMarker = *static_cast<GammaSettingTmarker_e*>(value);
			deviceSet(setting, tMarker);
			return 1;
		}
	case GAMMA_PARAM_SET_GATE:
		setting = GAMMA_SET_GATE; break;
	default:
		return 0;
	}

	deviceSet(setting, value8);
	return 1;
}
