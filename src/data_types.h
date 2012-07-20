/////////////////////////////////////////////////////////////////////////////
// Name:        data_types.h
// Purpose:     gives data types for gamma-view
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAMMA_VIEW_DATA_TYPES_H_
#define _GAMMA_VIEW_DATA_TYPES_H_

//#include <wx/wx.h>
//#include <wx/datetime.h>


enum GammaSetting_e
{
	GAMMA_SET_RESERVED = 0x00,
	GAMMA_SET_DISPLAY = 0x01, //deprecated
	GAMMA_SET_ZOOM,
	GAMMA_SET_SHIFT_X,
	GAMMA_SET_SHIFT_Y,
	GAMMA_SET_TMARKER,
	GAMMA_SET_GATE,
	GAMMA_SET_END,
};

enum GammaSettingTmarker_e
{
	GAMMA_TMARKER_OFF = 0x00,
	GAMMA_TMARKER_1MS,
	GAMMA_TMARKER_10MS,
	GAMMA_TMARKER_100MS,
	GAMMA_TMARKER_1000MS,
};

enum GammaBlockDataItemType_e
{
	GAMMA_ITEM_POINT = 0,
	GAMMA_ITEM_TMARKER, 
	GAMMA_ITEM_TRIGGER,
};

enum GammaBlockDataType_e
{
	GAMMA_DATA_USB_RAW = 0,
	GAMMA_DATA_POINTS,
	GAMMA_DATA_WXIMAGE,
};

struct GammaPoint 		// 6 bytes
{
	unsigned char x;	// 1 byte
	unsigned char y; 	// 1 byte
	float z;			// 4 bytes
};

union GammaItem				// 7 bytes
{
	unsigned char type;			// 1 byte
	union
	{
		unsigned long int time;	// 4 bytes
		GammaPoint point;		// 6 bytes
	} data;
};

#endif //_GAMMA_VIEW_DATA_TYPES_H_