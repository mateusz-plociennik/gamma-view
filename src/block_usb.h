/**
 * @file	block_usb.h
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_USB_H_
#define _GAMMA_VIEW_BLOCK_USB_H_

#ifdef _WIN32
	#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
		#include <windows.h>
		#include <CyAPI.h>
	#else
		#include <lusb0_usb.h>
	#endif
#endif

#include "data_types.h"
#include "block_base.h"

class GammaBlockUSB : public GammaPipeHead
{
public:
	GammaBlockUSB(GammaManager* pManager);
	~GammaBlockUSB();

	bool deviceFind();
	bool deviceInit();
	bool deviceOpen();
	bool deviceReset();
	bool deviceSet(GammaSetting_e setting, wxUint8 value);
	wxInt32 setParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
#if defined(_MSC_VER) && defined(GAMMA_USB_CYAPI)
	CCyUSBDevice* m_usbDevice;
	short int m_device;
#else
	usb_dev_handle* m_usbDevice;
#endif
};

#endif //_GAMMA_VIEW_BLOCK_USB_H_
