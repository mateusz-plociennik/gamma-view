/**
 * @file	block_usb.h
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_USB_H_
#define _GAMMA_VIEW_BLOCK_USB_H_

#include "CyAPI.h"
#include "data_types.h"
#include "block_base.h"

#define GAMMA_NO_DEVICE -1

class GammaBlockUSB : 
	public GammaBlockBase
{
public:
	GammaBlockUSB();
	~GammaBlockUSB();

	bool DeviceFind();
	bool DeviceInit();
	bool DeviceOpen();
	bool DeviceReset();
	bool DeviceSet(unsigned char setting, unsigned char value);

protected:
	wxThread::ExitCode Entry();

private:
	CCyUSBDevice* m_USBDevice;
	short int m_device;
};

#endif //_GAMMA_VIEW_BLOCK_USB_H_
