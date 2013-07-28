/**
 * @file	data_types.h
 * @brief	gives data types for gamma-view
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_DATA_TYPES_H_
#define _GAMMA_VIEW_DATA_TYPES_H_

#include <wx/gdicmn.h>

#include <set>

#define POINT(X, Y) ( 256 * (Y) + (X) )
#define POINT_INSIDE_FOV(P) getManager()->getConfig()->getFieldOfView()->isPointInside(P)

enum GammaEvent_e
{
	ID_GATE_TRIGGER = wxID_HIGHEST + 1,
	ID_EVENT_TRIG,
	ID_EVENT_SET_UNI_FILE,

	ID_MENU_UNIFORM,
	ID_MENU_UNIFORM_LOAD,

	ID_MENU_MODE,
	ID_MENU_MODE_LIVE,
	ID_MENU_MODE_LIVE_UNI,
	ID_MENU_MODE_PLAYBACK,
	ID_MENU_MODE_PLAYBACK_UNI,
	ID_MENU_MODE_UNIFORMITY,
	
	ID_MENU_INTEGRATE,
	ID_MENU_INTEGRATE_TIME_1_1000,
	ID_MENU_INTEGRATE_TIME_1_500,
	ID_MENU_INTEGRATE_TIME_1_250,
	ID_MENU_INTEGRATE_TIME_1_125,
	ID_MENU_INTEGRATE_TIME_1_60,
	ID_MENU_INTEGRATE_TIME_1_30,
	ID_MENU_INTEGRATE_TIME_1_15,
	ID_MENU_INTEGRATE_TIME_1_8,
	ID_MENU_INTEGRATE_TIME_1_4,
	ID_MENU_INTEGRATE_TIME_1_2,
	ID_MENU_INTEGRATE_TIME_1,
	ID_MENU_INTEGRATE_TIME_2,
	ID_MENU_INTEGRATE_TIME_4,
	ID_MENU_INTEGRATE_TIME_8,
	ID_MENU_INTEGRATE_TIME_16,
	ID_MENU_INTEGRATE_TIME_32,
	ID_MENU_INTEGRATE_ENABLED,

	ID_MENU_GLOW,
	ID_MENU_GLOW_TIME_1_1000,
	ID_MENU_GLOW_TIME_1_500,
	ID_MENU_GLOW_TIME_1_250,
	ID_MENU_GLOW_TIME_1_125,
	ID_MENU_GLOW_TIME_1_60,
	ID_MENU_GLOW_TIME_1_30,
	ID_MENU_GLOW_TIME_1_15,
	ID_MENU_GLOW_TIME_1_8,
	ID_MENU_GLOW_TIME_1_4,
	ID_MENU_GLOW_TIME_1_2,
	ID_MENU_GLOW_TIME_1,
	ID_MENU_GLOW_TIME_2,
	ID_MENU_GLOW_TIME_4,
	ID_MENU_GLOW_TIME_8,
	ID_MENU_GLOW_TIME_16,
	ID_MENU_GLOW_TIME_32,
	ID_MENU_GLOW_ENABLED,
};

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
	GAMMA_PARAM_TRIG_AVG,
	GAMMA_PARAM_TRIG_MAX,
	GAMMA_PARAM_TRIG_SUM,
	GAMMA_PARAM_TRIG_TIME,
	GAMMA_PARAM_TRIG_GATE,
	GAMMA_PARAM_TRIG_TYPE,
	GAMMA_PARAM_DATA_TYPE_USB, 
	GAMMA_PARAM_DATA_TYPE_ITEMS, 
	GAMMA_PARAM_DATA_TYPE_MATRIX,
	GAMMA_PARAM_DATA_TYPE_IMAGE,
	GAMMA_PARAM_SET_ZOOM, 
	GAMMA_PARAM_SET_SHIFT_X,
	GAMMA_PARAM_SET_SHIFT_Y,
	GAMMA_PARAM_SET_TMARKER,
	GAMMA_PARAM_SET_GATE,
};

enum GammaTrig_e
{
	GAMMA_TRIG_NONE = 0,
	GAMMA_TRIG_AVG, 
	GAMMA_TRIG_MAX, 
	GAMMA_TRIG_SUM, 
	GAMMA_TRIG_TIME, 
	GAMMA_TRIG_GATE,
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
	GAMMA_TMARKER_240MS,
};

enum GammaDataType_e
{
	GAMMA_DATA_TYPE_USB, 
	GAMMA_DATA_TYPE_ITEMS, 
	GAMMA_DATA_TYPE_MATRIX,
	GAMMA_DATA_TYPE_IMAGE,
};

enum GammaItemType_e
{
	GAMMA_ITEM_TYPE_POINT = 'P',
	GAMMA_ITEM_TYPE_TMARKER = 'T', 
	GAMMA_ITEM_TYPE_TRIGGER = 'G',
};

enum GammaMode_e
{
	GAMMA_MODE_USB_2_FILE = 0,
	GAMMA_MODE_USB_2_IMAGE,
	GAMMA_MODE_USB_2_IMAGE_UNI,
	GAMMA_MODE_FAKE_2_FILE,
	GAMMA_MODE_FAKE_2_IMAGE,
	GAMMA_MODE_FILE_2_IMAGE,
	GAMMA_MODE_FILE_2_IMAGE_UNI,
	GAMMA_MODE_USB_2_UNI_MATRIX,
	GAMMA_MODE_USB_FULL,
	GAMMA_MODE_PLAYBACK,
	GAMMA_MODE_VIRTUAL,
	GAMMA_MODE_NONE,
};

enum GammaArea_e
{
	GAMMA_AREA_FOV,
	GAMMA_AREA_UFOV,
	GAMMA_AREA_CFOV,
	GAMMA_AREA_UNDEFINED,
};

enum GammaDirection_e
{
	GAMMA_DIRECTION_X,
	GAMMA_DIRECTION_Y,
};

//#pragma pack(push) /* push current alignment to stack */
//#pragma pack(1) /* set alignment to 1 byte boundary */

struct GammaPoint 		// 8 bytes
{
	wxUint8 x;			// 1 byte
	wxUint8 y; 			// 1 byte
	//wxUint8 reserved[2];// 2 byte
	wxFloat32 z;		// 4 bytes
};

struct GammaItem		// 12 bytes
{
	wxUint8 type;		// 1 byte
	//wxUint8 reserved[3];// 3 byte
	union
	{
		wxUint32 time;	// 4 bytes //change to  8bytes wxDateTime?
		GammaPoint point;// 8 bytes
	} data;
};

//#pragma pack(pop) /* restore original alignment from stack */

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

class GammaFieldOfView // 12 bytes
{
public:
	GammaFieldOfView(wxRealPoint center, wxDouble radius, GammaArea_e area)
		: m_center(center)
		, m_radius(radius)
		, m_area(area)
	{
		calcPoint();
	}

	GammaArea_e getArea()
	{
		return m_area;
	}

	void setArea(GammaArea_e area)
	{
		m_area = area;
	}

	wxRealPoint getCenter()
	{
		return m_center;
	}

	void setCenter(wxRealPoint center)
	{
		m_center = center;
		calcPoint();
	}

	wxDouble getRadius(GammaArea_e area = GAMMA_AREA_UNDEFINED)
	{
		if(GAMMA_AREA_UNDEFINED == area)
		{
			area = m_area;
		}
		switch(area)
		{
		default:
		case GAMMA_AREA_FOV:
			return m_radius;
		case GAMMA_AREA_UFOV:
			return m_radius * 95 / 100;
		case GAMMA_AREA_CFOV:
			return m_radius * 95 * 75 / 10000;
		}
	}

	void setRadius(wxDouble radius)
	{
		m_radius = radius;
		calcPoint();
	}

	bool isPointInside(wxUint32 point, GammaArea_e area = GAMMA_AREA_UNDEFINED)
	{
		return m_pointSet[GAMMA_AREA_UNDEFINED != area ? area : m_area].end() 
			!= m_pointSet[GAMMA_AREA_UNDEFINED != area ? area : m_area].find(point);
	}

	wxUint32 getPointCount(GammaArea_e area = GAMMA_AREA_UNDEFINED)
	{
		return m_pointSet[GAMMA_AREA_UNDEFINED != area ? area : m_area].size();
	}

private:
	GammaArea_e m_area;
	wxRealPoint m_center;
	wxDouble m_radius;

	std::set<wxUint32> m_pointSet[GAMMA_AREA_UNDEFINED];

	void calcPoint()
	{
		for(wxUint32 y = 0; y < 256; y++)
		{
			for(wxUint32 x = 0; x < 256; x++)
			{
				if(pow(getRadius(GAMMA_AREA_FOV), 2) > pow(m_center.x - x, 2) + pow(m_center.y - y, 2))
				{
					m_pointSet[GAMMA_AREA_FOV].insert(POINT(x, y));
				}

				if(pow(getRadius(GAMMA_AREA_UFOV), 2) > pow(m_center.x - x, 2) + pow(m_center.y - y, 2))
				{
					m_pointSet[GAMMA_AREA_UFOV].insert(POINT(x, y));
				}

				if(pow(getRadius(GAMMA_AREA_CFOV), 2) > pow(m_center.x - x, 2) + pow(m_center.y - y, 2))
				{
					m_pointSet[GAMMA_AREA_CFOV].insert(POINT(x, y));
				}
			}
		}
	}
};

#endif //_GAMMA_VIEW_DATA_TYPES_H_
