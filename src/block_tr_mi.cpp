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
#include <cmath>

GammaTransMI::GammaTransMI(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_brightness(0.0)
	, m_contrast(1.0)
	, m_gamma(1.0)
	, m_bInvert(false)
	, m_colourmap(GAMMA_COLOURMAP_GRAY)
	, m_eventMaxTable(1, 1)
	, m_bSetUniformMatrix(false)
{
}

void GammaTransMI::calcMax(wxUint32 eventMax)
{
	static const wxDouble range = sqrt(2.0);

	wxDouble sigma = m_eventMaxTable.size();
	wxDouble sigmaX = 0, sigmaY = 0;
	wxDouble sigmaXX = 0, sigmaXY = 0, sigmaYY = 0;

	for(wxUint32 x = 0; x < m_eventMaxTable.size(); x++)
	{
		sigmaX += x;
		sigmaY += m_eventMaxTable[x];
		sigmaXX += x * x;
		sigmaXY += x * m_eventMaxTable[x];
		sigmaYY += m_eventMaxTable[x]* m_eventMaxTable[x];
	}

	wxDouble delta = sigma * sigmaXX - sigmaX * sigmaX;
	wxDouble a = (sigma * sigmaXY - sigmaX * sigmaY ) / delta;
	wxDouble b = (sigmaXX * sigmaY - sigmaX * sigmaXY) / delta;

	//wxDouble stdDev = sqrt(sigmaYY - a * sigmaXY - b * sigmaY);
	wxDouble y = a * m_eventMaxTable.size() + b;

	if(y / range < eventMax && eventMax < range * y)
	{
		if(64 <= m_eventMaxTable.size())
		{
			m_eventMaxTable.pop_front();
		}
		m_max = y;
	}
	else
	{
		while(1 < m_eventMaxTable.size())
		{
			m_eventMaxTable.pop_front();
		}
		m_max = eventMax;
	}

	m_eventMaxTable.push_back(eventMax);
}

void GammaTransMI::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_MATRIX == pData->type);
	GammaMatrix* pDataIn = static_cast<GammaMatrix*>(pData);
	GammaImage* pDataOut = new GammaImage;

	calcMax(pDataIn->eventMax);

	calcColour(0);
	pDataOut->image.SetRGB( wxRect(0, 0, 256, 256), 
		m_colour.Red(), m_colour.Green(), m_colour.Blue() );

	for(wxUint32 y = 0; y < 256; y++)
	{
		for(wxUint32 x = 0; x < 256; x++)
		{
			if(pDataIn->matrix[POINT(x,y)])
			{
				calcColour(pDataIn->matrix[POINT(x,y)]);
				pDataOut->image.SetRGB( x, y, 
					m_colour.Red(), m_colour.Green(), m_colour.Blue() );
			}
		}
	}

	if(GAMMA_TRIG_NONE != pDataIn->trig)
	{
		getManager()->DataTierSetParam(GAMMA_PARAM_UNIFORM_MATRIX_SET, pDataIn->matrix);
		getManager()->PresentationTierSetParam(GAMMA_PARAM_TRIG_TYPE, &pDataIn->trig);
	}

/*	wxLogStatus("eventMax = %u, m_max = %f, eventSum = %"wxLongLongFmtSpec"d, span = %"wxLongLongFmtSpec"d freq = %f k/s", 
		pDataIn->eventMax, m_max, pDataIn->eventSum, pDataIn->span.GetValue().GetValue(), 
		(double)pDataIn->eventSum / pDataIn->span.GetValue().GetValue());
*/

	getManager()->PresentationTierSetParam(GAMMA_PARAM_DATA_TYPE_MATRIX, (void*)pDataIn);

	getManager()->PresentationTierSetParam(GAMMA_PARAM_IMG_DATA, (void*)&pDataOut->image);
	getManager()->PresentationTierSetParam(GAMMA_PARAM_TIME_NOW, &pDataIn->time);
	delete pDataOut;
}

void GammaTransMI::calcColour(wxDouble index)
{
/*	wxUint64 key = wxUint64(index) | wxUint64(m_max) << 32;

	std::map<wxUint64, wxColour>::iterator iColour = m_colourLut.find(key);
	if(m_colourLut.end() != iColour)
	{
		m_colour = iColour->second;
		return;
	}
	else if(m_colourLut.size() > 256)
	{
		m_colourLut.clear();
	}
*/
	double x = std::max( 0.0, std::min( 1.0, m_contrast * ( pow( !m_bInvert ? 
		index/m_max : 1.0 - index/m_max, 1.0/m_gamma ) + m_brightness ) ) );
	
	double r, g, b;
	switch(m_colourmap)
	{
	case GAMMA_COLOURMAP_AUTUMN:
		r = 1.0;
		g = x;
		b = 0.0;
		break;
	case GAMMA_COLOURMAP_BONE:
		r = (x < 3.0/4.0) * (7.0/8.0 * x) 
			+ (x >= 3.0/4.0) * (11.0/8.0 * x - 3.0/8.0); 
		g = (x < 3.0/8.0) * (7.0/8.0 * x)
			+ (x >= 3.0/8.0 && x < 3.0/4.0) * (29.0/24.0 * x - 1.0/8.0)
			+ (x >= 3.0/4.0) * (7.0/8.0 * x + 1.0/8.0);
		b = (x < 3.0/8.0) * (29.0/24.0 * x) 
			+ (x >= 3.0/8.0) * (7.0/8.0 * x + 1.0/8.0);
		break;
	case GAMMA_COLOURMAP_COOL:
		r = x;
		g = 1.0 - r;
		b = 1.0;
		break;
	case GAMMA_COLOURMAP_COPPER:
		r = (x < 4.0/5.0) * (5.0/4.0 * x) 
			+ (x >= 4.0/5.0);
		g = 4.0/5.0 * x;
		b = 1.0/2.0 * x;
		break;
	case GAMMA_COLOURMAP_HOT:
		r = (x < 2.0/5.0) * (5.0/2.0 * x) 
			+ (x >= 2.0/5.0);
		g = (x >= 2.0/5.0 && x < 4.0/5.0) * (5.0/2.0 * x - 1.0) 
			+ (x >= 4.0/5.0);
		b = (x >= 4.0/5.0) * (5.0 * x - 4.0);
		break;
	case GAMMA_COLOURMAP_HSV:
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
	case GAMMA_COLOURMAP_JET:
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
	case GAMMA_COLOURMAP_OCEAN:
		r = (x >= 2.0/3.0) * (3.0 * x - 2.0);
		g = (x >= 1.0/3.0) * (3.0/2.0 * x - 1.0/2.0);
		b = x;
		break;
	case GAMMA_COLOURMAP_PINK:
		r = sqrt( (x < 3.0/8.0) * (14.0/9.0 * x) 
			+ (x >= 3.0/8.0) * (2.0/3.0 * x + 1.0/3.0) );
		g = sqrt( (x < 3.0/8.0) * (2.0/3.0 * x)
			+ (x >= 3.0/8.0 && x < 3.0/4.0) * (14.0/9.0 * x - 1.0/3.0)
			+ (x >= 3.0/4.0) * (2.0/3.0 * x + 1.0/3.0) );
		b = sqrt( (x < 3.0/4.0) * (2.0/3.0 * x) 
			+ (x >= 3.0/4.0) * (2.0 * x - 1.0) );
		break;
	case GAMMA_COLOURMAP_RAINBOW:
		r = (x < 2.0/5.0) 
			+ (x >= 2.0/5.0 && x < 3.0/5.0) * (-5.0 * x + 3.0)
			+ (x >= 4.0/5.0) * (10.0/3.0 * x - 8.0/3.0);
		g = (x < 2.0/5.0) * (5.0/2.0 * x) 
			+ (x >= 2.0/5.0 && x < 3.0/5.0)
			+ (x >= 3.0/5.0 && x < 4.0/5.0) * (-5.0 * x + 4.0);
		b = (x >= 3.0/5.0 && x < 4.0/5.0) * (5.0 * x - 3.0) 
			+ (x >= 4.0/5.0);
		break;
	case GAMMA_COLOURMAP_SPRING:
		r = 1.0;
		g = x;
		b = 1.0 - g;
		break;
	case GAMMA_COLOURMAP_SUMMER:
		r = x;
		g = 0.5 + (r / 2);
		b = 0.4;
		break;
	case GAMMA_COLOURMAP_WINTER:
		r = 0.0;
		g = x;
		b = 1.0 - (g / 2.0);
		break;
	case GAMMA_COLOURMAP_GRAY:
	default:
		r = x;
		g = x;
		b = x;
		break;
	}

	m_colour.Set(r * 255, g * 255, b * 255);
//	m_colourLut[key] = m_colour;
}

wxInt32 GammaTransMI::setParam(GammaParam_e param, void* value)
{
	wxMutexLocker locker(m_processDataMutex);

	switch(param)
	{
	case GAMMA_PARAM_IMG_BRIGHTNESS:
		m_brightness = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_CONTRAST:
		m_contrast = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_GAMMA:
		m_gamma = *static_cast<double*>(value); break;
	case GAMMA_PARAM_COLOURMAP:
		m_colourmap = *static_cast<GammaColourmap_e*>(value); break;
	case GAMMA_PARAM_COLOURMAP_INVERT:
		m_bInvert = *static_cast<bool*>(value); break;
	case GAMMA_PARAM_BUTTON_SET:
		m_bSetUniformMatrix = true;
	default:
		return false;
	}

//	m_colourLut.clear();
	return true;
}
