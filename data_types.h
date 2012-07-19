/////////////////////////////////////////////////////////////////////////////
// Name:        data_types.h
// Purpose:     gives data types for gamma-view
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_DATA_TYPES_H_
#define _GAMMA_VIEW_DATA_TYPES_H_

#include <wx/thread.h>

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

enum gammaDataType_e
{
	GAMMA_DATA_USB_RAW = 0,
	GAMMA_DATA_GAMMA_VIEW
};

struct gammaPoint 					// 64 bits
{
	bool isPoint			: 1; 	// 1 bit
	unsigned char reserved1	: 7; 	// 7 bits
	unsigned char reserved2	: 8; 	// 8 bits
	
	unsigned char x;				// 8 bits
	unsigned char y; 				// 8 bits
	float z;						// 32 bits
};

union gammaDataItem			// 8 bytes
{
	unsigned long long dt;	// 8 bytes
	gammaPoint gp;			// 8 bytes
};

struct gammaData
{
	unsigned char type;
	unsigned int length;
	wxMutex mutex;
	void* data_p;
};

#endif //_GAMMA_VIEW_DATA_TYPES_H_