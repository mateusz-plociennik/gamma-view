/**
 * @file	block_usb.h
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_USB_H_
#define _GAMMA_VIEW_BLOCK_USB_H_

#ifdef _WIN32
#include "CyAPI.h"
#endif

#include "data_types.h"
#include "block_base.h"

class GammaBlockUSB : 
	public GammaBlockBase
{
public:
	GammaBlockUSB(GammaManager* pManager);
	~GammaBlockUSB();

	bool DeviceFind();
	bool DeviceInit();
	bool DeviceOpen();
	bool DeviceReset();
	bool DeviceSet(unsigned char setting, unsigned char value);

protected:
	wxThread::ExitCode Entry();

private:
#ifdef _WIN32
	CCyUSBDevice* m_USBDevice;
	short int m_device;
#endif
};

#endif //_GAMMA_VIEW_BLOCK_USB_H_
