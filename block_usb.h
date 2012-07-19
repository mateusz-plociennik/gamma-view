/////////////////////////////////////////////////////////////////////////////
// Name:        block_usb.h
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_USB_H_
#define _GAMMA_VIEW_BLOCK_USB_H_

#include "CyAPI.h"
#include "data_types.h"
#include "block.h"

class GammaBlockUSB : public GammaBlock
{
public:
	GammaBlockUSB();
	~GammaBlockUSB();

	void BlockRun();
	void BlockStop();

	void USBClose();
	bool USBInit();
	bool USBOpen();
	bool USBReset();
	bool USBSet(unsigned char setting, unsigned char value);

private:
	wxThread::ExitCode Entry();

	CCyUSBDevice* m_USBDevice;
	PUCHAR m_buffer;
	LONG m_length;
};

#endif //_GAMMA_VIEW_BLOCK_USB_H_
