/**
 * @file	block_usb_fake.h
 * @brief	Fake device for testing purposes.
 * @author	Mateusz Plociennik
 * @data	2012-07-29
 */

#ifndef _GAMMA_VIEW_BLOCK_USB_FAKE_H_
#define _GAMMA_VIEW_BLOCK_USB_FAKE_H_

#include "data_types.h"
#include "block_base.h"

class GammaBlockUSBFake : 
	public GammaBlockBase
{
public:
	GammaBlockUSBFake(GammaManager* pManager);

protected:
	wxThread::ExitCode Entry();

private:

};

#endif //_GAMMA_VIEW_BLOCK_USB_FAKE_H_
