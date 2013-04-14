/**
 * @file	block_tr_mi.cpp
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_mgmt.h"
#include "block_tr_mi.h"
#include "config.h"
#include <wx/time.h>

GammaTransMI::GammaTransMI(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_brightness(0.0)
	, m_contrast(1.0)
	, m_gamma(1.0)
	, m_invert(false)
	, m_colormap(GAMMA_COLORMAP_GRAY)
{
}

void GammaTransMI::processData(GammaDataBase* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaDataMatrix* pDataIn = static_cast<GammaDataMatrix*>(pData);
	GammaDataImage* pDataOut = new GammaDataImage;

	setColor(0, pDataIn->max);
	pDataOut->data->SetRGB( wxRect(0, 0, 256, 256), 
		m_color.Red(), m_color.Green(), m_color.Blue() );

	for(wxUint32 x = 0; x < 256; x++)
	{
		for(wxUint32 y = 0; y < 256; y++)
		{
			if(pDataIn->data[POINT(x,y)])
			{			
				setColor(pDataIn->data[POINT(x,y)], pDataIn->max);
				pDataOut->data->SetRGB( x, y, 
					m_color.Red(), m_color.Green(), m_color.Blue() );
			}
		}
	}

	getManager()->PresentationTierSetParam(GAMMA_PARAM_IMG_DATA, (void*)pDataOut->data);
	delete pDataOut;
}

void GammaTransMI::setColor(wxUint32 index, wxUint32 max)
{
	double x = std::max(0.0, std::min(1.0, 
		m_contrast * ( pow((double)index/max, 1.0/m_gamma) + m_brightness ) ));

	if(m_invert)
	{
		x = 1.0 - x;
	}
	
	double r, g, b;
	switch(m_colormap)
	{
	case GAMMA_COLORMAP_AUTUMN:
		r = 1.0;
		g = x;
		b = 0.0;
		break;
	case GAMMA_COLORMAP_BONE:
		r = (x < 3.0/4.0) * (7.0/8.0 * x) 
			+ (x >= 3.0/4.0) * (11.0/8.0 * x - 3.0/8.0); 
		g = (x < 3.0/8.0) * (7.0/8.0 * x)
			+ (x >= 3.0/8.0 && x < 3.0/4.0) * (29.0/24.0 * x - 1.0/8.0)
			+ (x >= 3.0/4.0) * (7.0/8.0 * x + 1.0/8.0);
		b = (x < 3.0/8.0) * (29.0/24.0 * x) 
			+ (x >= 3.0/8.0) * (7.0/8.0 * x + 1.0/8.0);
		break;
	case GAMMA_COLORMAP_COOL:
		r = x;
		g = 1.0 - r;
		b = 1.0;
		break;
	case GAMMA_COLORMAP_COPPER:
		r = (x < 4.0/5.0) * (5.0/4.0 * x) 
			+ (x >= 4.0/5.0);
		g = 4.0/5.0 * x;
		b = 1.0/2.0 * x;
		break;
	case GAMMA_COLORMAP_HOT:
		r = (x < 2.0/5.0) * (5.0/2.0 * x) 
			+ (x >= 2.0/5.0);
		g = (x >= 2.0/5.0 && x < 4.0/5.0) * (5.0/2.0 * x - 1.0) 
			+ (x >= 4.0/5.0);
		b = (x >= 4.0/5.0) * (5.0 * x - 4.0);
		break;
	case GAMMA_COLORMAP_HSV:
		r = (x < 1.0/6.0)
			+ (x >= 1.0/6.0 && x < 2.0/6.0) * (-6.0 * x + 2.0)
			+ (x >= 4.0/6.0 && x < 5.0/6.0) * (6.0 * x - 4.0)
			+ (x >= 5.0/6.0);
		g = (x < 1.0/6.0) * (6.0 * x)
			+ (x >= 1.0/6.0 && x < 3.0/6.0) 
			+ (x >= 3.0/6.0 && x < 4.0/6.0) * (-6.0 * x + 4.0);
		b = (x >= 2.0/6.0 && x < 3.0/6.0) * (6.0 * x - 2.0) 
			+ (x >= 3.0/6.0 && x < 5.0/6.0)
			+ (x >= 5.0/6.0) * (-6.0 * x + 6.0);
		break;
	case GAMMA_COLORMAP_JET:
		r = (x >= 3.0/8.0 && x < 5.0/8.0) * (4.0 * x - 3.0/2.0)
			+ (x >= 5.0/8.0 && x < 7.0/8.0) 
			+ (x >= 7.0/8.0) * (-4.0 * x + 9.0/2.0);
		g = (x >= 1.0/8.0 && x < 3.0/8.0) * (4.0 * x - 1.0/2.0)
			+ (x >= 3.0/8.0 && x < 5.0/8.0) 
			+ (x >= 5.0/8.0 && x < 7.0/8.0) * (-4.0 * x + 7.0/2.0);
		b = (x < 1.0/8.0) * (4.0 * x + 1.0/2.0) 
			+ (x >= 1.0/8.0 && x < 3.0/8.0)
			+ (x >= 3.0/8.0 && x < 5.0/8.0) * (-4.0 * x + 5.0/2.0);
		break;
	case GAMMA_COLORMAP_OCEAN:
		r = (x >= 2.0/3.0) * (3.0 * x - 2.0);
		g = (x >= 1.0/3.0) * (3.0/2.0 * x - 1.0/2.0);
		b = x;
		break;
	case GAMMA_COLORMAP_PINK:
		r = sqrt( (x < 3.0/8.0) * (14.0/9.0 * x) 
			+ (x >= 3.0/8.0) * (2.0/3.0 * x + 1.0/3.0) );
		g = sqrt( (x < 3.0/8.0) * (2.0/3.0 * x)
			+ (x >= 3.0/8.0 && x < 3.0/4.0) * (14.0/9.0 * x - 1.0/3.0)
			+ (x >= 3.0/4.0) * (2.0/3.0 * x + 1.0/3.0) );
		b = sqrt( (x < 3.0/4.0) * (2.0/3.0 * x) 
			+ (x >= 3.0/4.0) * (2.0 * x - 1.0) );
		break;
	case GAMMA_COLORMAP_RAINBOW:
		r = (x < 2.0/5.0) 
			+ (x >= 2.0/5.0 && x < 3.0/5.0) * (-5.0 * x + 3.0)
			+ (x >= 4.0/5.0) * (10.0/3.0 * x - 8.0/3.0);
		g = (x < 2.0/5.0) * (5.0/2.0 * x) 
			+ (x >= 2.0/5.0 && x < 3.0/5.0)
			+ (x >= 3.0/5.0 && x < 4.0/5.0) * (-5.0 * x + 4.0);
		b = (x >= 3.0/5.0 && x < 4.0/5.0) * (5.0 * x - 3.0) 
			+ (x >= 4.0/5.0);
		break;
	case GAMMA_COLORMAP_SPRING:
		r = 1.0;
		g = x;
		b = 1.0 - g;
		break;
	case GAMMA_COLORMAP_SUMMER:
		r = x;
		g = 0.5 + (r / 2);
		b = 0.4;
		break;
	case GAMMA_COLORMAP_WINTER:
		r = 0.0;
		g = x;
		b = 1.0 - (g / 2.0);
		break;
	case GAMMA_COLORMAP_GRAY:
	default:
		r = x;
		g = x;
		b = x;
		break;
	}

	m_color.Set(r * 255, g * 255, b * 255);
}

bool GammaTransMI::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_BRIGHTNESS:
		m_brightness = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_CONTRAST:
		m_contrast = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_GAMMA:
		m_gamma = *static_cast<double*>(value); break;
	case GAMMA_PARAM_COLORMAP:
		m_colormap = *static_cast<GammaColormap_e*>(value); break;
	case GAMMA_PARAM_COLORMAP_INVERT:
		m_invert = *static_cast<bool*>(value); break;
	default:
		return false;
	}

	return true;
}
