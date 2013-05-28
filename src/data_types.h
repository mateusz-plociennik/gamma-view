/**
 * @file	data_types.h
 * @brief	gives data types for gamma-view
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_DATA_TYPES_H_
#define _GAMMA_VIEW_DATA_TYPES_H_

#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS
#include <inttypes.h>
#include <wx/gdicmn.h>

#include <crtdbg.h>

#define POINT(X,Y) (256*(Y)+(X))
#define POINT_INSIDE_FOV(X,Y) (pow(getManager()->getConfig()->getFieldOfView()->getRadius(), 2) > \
	pow(getManager()->getConfig()->getFieldOfView()->center.x - X, 2) + pow(getManager()->getConfig()->getFieldOfView()->center.y - Y, 2))

enum GammaParam_e
{
	GAMMA_PARAM_COLOURMAP,
	GAMMA_PARAM_COLOURMAP_INVERT,
	GAMMA_PARAM_USB_ZOOM,
	GAMMA_PARAM_USB_SHIFT_X,
	GAMMA_PARAM_USB_SHIFT_Y,
	GAMMA_PARAM_USB_TMARKER,
	GAMMA_PARAM_USB_GATE,
	GAMMA_PARAM_FILE_NAME_READ,
	GAMMA_PARAM_FILE_NAME_WRITE,
	GAMMA_PARAM_FILE_SET_TIME,
	GAMMA_PARAM_IMG_BRIGHTNESS,
	GAMMA_PARAM_IMG_CONTRAST,
	GAMMA_PARAM_IMG_GAMMA,
	GAMMA_PARAM_IMG_DATA,
	GAMMA_PARAM_IMG_INTEGRATE_ENABLED, 
	GAMMA_PARAM_IMG_INTEGRATE_TIME,
	GAMMA_PARAM_IMG_SPEED,
	GAMMA_PARAM_TIME_NOW,
	GAMMA_PARAM_TIME_END,
	GAMMA_PARAM_UNIFORM_MATRIX_SET,
	GAMMA_PARAM_MATRIX_DATA,
	GAMMA_PARAM_FREQUENCY,
	GAMMA_PARAM_BUTTON_SET,
};

enum GammaColourmap_e
{
	GAMMA_COLOURMAP_AUTUMN = 0,
	GAMMA_COLOURMAP_BONE,
	GAMMA_COLOURMAP_COOL,
	GAMMA_COLOURMAP_COPPER,
	GAMMA_COLOURMAP_GRAY,
	GAMMA_COLOURMAP_HOT,
	GAMMA_COLOURMAP_HSV,
	GAMMA_COLOURMAP_JET,
	GAMMA_COLOURMAP_OCEAN,
	GAMMA_COLOURMAP_PINK,
	GAMMA_COLOURMAP_RAINBOW,
	GAMMA_COLOURMAP_SPRING,
	GAMMA_COLOURMAP_SUMMER,
	GAMMA_COLOURMAP_WINTER,
};

enum GammaSetting_e
{
	GAMMA_SET_RESERVED = 0x00,
	GAMMA_SET_DISPLAY = 0x01, //deprecated
	GAMMA_SET_ZOOM,
	GAMMA_SET_SHIFT_X,
	GAMMA_SET_SHIFT_Y,
	GAMMA_SET_TMARKER,
	GAMMA_SET_GATE,
	GAMMA_SET_MAX, //end
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
	GAMMA_ITEM_POINT = 'P',
	GAMMA_ITEM_TMARKER = 'T', 
	GAMMA_ITEM_TRIGGER = 'G',
};

enum GammaMode_e
{
	GAMMA_MODE_USB_2_FILE = 0,
	GAMMA_MODE_USB_2_IMAGE,
	GAMMA_MODE_FAKE_2_FILE,
	GAMMA_MODE_FAKE_2_IMAGE,
	GAMMA_MODE_FILE_2_IMAGE,
	GAMMA_MODE_USB_FULL,
	GAMMA_MODE_PLAYBACK,
	GAMMA_MODE_VIRTUAL,
	GAMMA_MODE_NONE,
};

enum GammaArea_e
{
	GAMMA_AREA_CFOV,
	GAMMA_AREA_UFOV,
	GAMMA_AREA_FOV,
	GAMMA_AREA_UNDEFINED,
};

enum GammaDirection_e
{
	GAMMA_DIRECTION_X,
	GAMMA_DIRECTION_Y,
};

struct GammaPoint 		// 8 bytes
{
	wxUint8 x;			// 1 byte
	wxUint8 y; 			// 1 byte
	wxUint8 reserved[2];// 2 byte
	wxFloat32 z;		// 4 bytes
};

struct GammaItem		// 12 bytes
{
	wxUint8 type;		// 1 byte
	wxUint8 reserved[3];// 3 byte
	union
	{
		wxUint32 time;	// 4 bytes
		GammaPoint point;// 8 bytes
	} data;
};

struct GammaItem2		// 8 bytes // Reduced size GammaItem (use in future?)
{
	wxUint8 type;		// 1 byte
	wxUint8 reserved;	// 1 byte
	wxUint8 x;			// 1 byte
	wxUint8 y; 			// 1 byte

	union
	{
		wxUint32 time;	// 4 bytes
		wxFloat32 z;	// 4 bytes
	} extra;			// 4 bytes
};


struct GammaFieldOfView // 12 bytes
{
	wxRealPoint center; // 2 * 4 bytes
	wxDouble radius; // 4 bytes
	GammaArea_e area;
	
	wxDouble getRadius(GammaArea_e iArea = GAMMA_AREA_UNDEFINED)
	{
		if(GAMMA_AREA_UNDEFINED == iArea)
		{
			iArea = area;
		}
		switch(iArea)
		{
		default:
		case GAMMA_AREA_FOV:
			return radius;
		case GAMMA_AREA_UFOV:
			return radius * 95 / 100;
		case GAMMA_AREA_CFOV:
			return radius * 95 * 75 / 10000;
		}
	}
};

#endif //_GAMMA_VIEW_DATA_TYPES_H_
