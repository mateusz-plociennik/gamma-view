/////////////////////////////////////////////////////////////////////////////
// Name:        driver.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_BLOCK_DRIVER_H_
#define _GAMMA_VIEW_BLOCK_DRIVER_H_

#include "CyAPI.h"
#include "data_types.h"
#include "block.h"

enum gammaResult_e
{
	GAMMA_RES_EOD = 0,
	GAMMA_RES_ERROR,
};

enum gammaSetting_e
{
	GAMMA_SET_DISPLAY = 0x01, // deprecated
	GAMMA_SET_ZOOM,
	GAMMA_SET_SHIFT_X,
	GAMMA_SET_SHIFT_Y,
	GAMMA_SET_T_MARKER,
	GAMMA_SET_GATE_ON,
	GAMMA_SET_LAST,
};

enum gammaSettingTmarker_e
{
	GAMMA_T_MARKER_OFF = 0x00,
	GAMMA_T_MARKER_1MS,
	GAMMA_T_MARKER_10MS,
	GAMMA_T_MARKER_100MS,
	GAMMA_T_MARKER_1000MS,
};

class GammaBlockUSB : GammaBlock
{
public:
	GammaBlockUSB();
	~GammaBlockUSB();

private:
	wxThread::ExitCode Entry();
	CCyUSBDevice *m_USBDevice;
};

#endif //_GAMMA_VIEW_BLOCK_DRIVER_H_
