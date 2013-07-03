/**
 * @file	block_usb.h
 * @brief	GammaUSB driver
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_USB_H_
#define _GAMMA_VIEW_BLOCK_USB_H_

#ifdef _WIN32
#include <windows.h>
#include "CyAPI.h"
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
#ifdef _WIN32
	CCyUSBDevice* m_USBDevice;
	short int m_device;
#endif
};

#endif //_GAMMA_VIEW_BLOCK_USB_H_
