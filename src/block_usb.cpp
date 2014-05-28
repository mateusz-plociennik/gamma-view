/**
 * @file	block_usb.cpp
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_usb.h"
#include "block_tr_us.h"
#include <wx/fileconf.h>

#define GAMMA_VID	0x04B4
#define GAMMA_PID	0x8813

#define EP1OUT	0x01
#define EP6IN	0x86

#define GAMMA_READ_SIZE		0x200
#define GAMMA_READ_TIMEOUT	1000
#define GAMMA_WRITE_TIMEOUT	100

bool GammaBlockUSB::deviceFind()
{
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	for( m_device = 0; m_device < m_usbDevice->DeviceCount(); m_device++ ) {
		m_usbDevice->Open(m_device);
		
		if(m_usbDevice->VendorID == GAMMA_VID && m_usbDevice->ProductID == GAMMA_PID) {
			break;
		}
	}
	
	if(m_device < m_usbDevice->DeviceCount()) {
		wxLogStatus("Device found (VID_%04X&PID_%04X).", 
			m_usbDevice->VendorID, m_usbDevice->ProductID);
		return true;
	}
	else {
		wxLogStatus("No device found.");
		return false;
	}
#else
	// Before any communication can occur with a device, it needs to be found. 
	// This is accomplished by finding all of the busses and then finding all of
	// the devices on all of the busses:
	usb_init();
	usb_find_busses();
	usb_find_devices();

	// After this, the application should manually loop through all of the 
	// busess and all of the devices and matching the device by whatever 
	// criteria is needed:
	for (struct usb_bus* iBus = usb_get_busses(); iBus; iBus = iBus->next) {
		for (struct usb_device* iDev = iBus->devices; iDev; iDev = iDev->next) {
			/* Check if this device is a printer */
			if (iDev->descriptor.idVendor == GAMMA_VID && iDev->descriptor.idProduct == GAMMA_PID) {
				/* Open the device, claim the interface and do your processing */
				m_usbDevice = usb_open(iDev);
				return true;
			}
		}
	}
	return false;
#endif
}

bool GammaBlockUSB::deviceInit()
{
	wxConfigBase* config = wxFileConfig::Get();
	wxConfigPathChanger changer(config, "/USBDevice/");

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
}

bool GammaBlockUSB::deviceReset()
{
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	if(!m_usbDevice->IsOpen() || !m_usbDevice->Reset())
	{
		return false;
	}
#else
	if( /*0 > usb_reset(m_usbDevice) ||*/
		0 > usb_set_configuration(m_usbDevice, 1) || 
		0 > usb_claim_interface(m_usbDevice, 0))
	{
		return false;
	}
#endif
	return true;
}

bool GammaBlockUSB::deviceSet(GammaSetting_e setting, wxUint8 value)
{
	wxUint8 buffer[] = {static_cast<wxUint8>(setting), value};
	int bytesWritten = sizeof(buffer);

#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	LONG length = bytesWritten;
	return m_usbDevice->BulkOutEndPt->XferData(buffer, length);
	bytesWritten = length;
#else
	//usb_claim_interface(m_usbDevice, 0);

	bytesWritten = usb_bulk_write(m_usbDevice, EP1OUT, 
		(char*)buffer, bytesWritten, GAMMA_WRITE_TIMEOUT);

	//usb_release_interface(m_usbDevice, 0);

	return sizeof(buffer) == bytesWritten;
#endif
}

wxThread::ExitCode GammaBlockUSB::Entry()
{
	//wxMutexLocker locker(m_processDataMutex);

	if(deviceFind() && deviceInit())
	{
		while(!GetThread()->TestDestroy())
		{
			GammaDataUSB* pDataOut(new GammaDataUSB);

			int bytesRead = 0x200;
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
			LONG length = bytesRead;
			m_usbDevice->BulkInEndPt->XferData(pDataOut->data, length);
			bytesRead = length;
#else
			//usb_claim_interface(m_usbDevice, 0);

			bytesRead = usb_bulk_read(m_usbDevice, EP6IN, 
				(char*)pDataOut->data, bytesRead, GAMMA_READ_TIMEOUT);

			//usb_release_interface(m_usbDevice, 0);
#endif
			wxASSERT(GAMMA_READ_SIZE == bytesRead);
			
			pushData(wxSharedPtr<GammaData>(pDataOut));
		}
	}

	return 0;
}

GammaBlockUSB::GammaBlockUSB(GammaManager* pManager) 
	: GammaPipeHead(pManager)
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	, m_usbDevice(new CCyUSBDevice(NULL))
#else
	, m_usbDevice(NULL)
#endif
{
}

GammaBlockUSB::~GammaBlockUSB()
{
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	delete m_usbDevice;
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
