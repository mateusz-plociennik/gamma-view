/**
 * @file	b_nemacalc.cpp
 * @brief	NEMA Calculations
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "b_nemacalc.h"

#include "block_data.h"
#include "data_types.h"

#include "config.h"

#include <wx/gdicmn.h>

#include <math.h>

GammaNemaCalc::GammaNemaCalc(GammaManager* pManager) 
		:
		GammaPipeSegment(pManager)
{
	m_fieldOfView.center = wxPoint(128, 128);
	m_fieldOfView.radius = 120;

	wxLogStatus("%s", __FUNCTION__);
}

GammaNemaCalc::~GammaNemaCalc()
{
	wxLogStatus("%s", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////

bool GammaNemaCalc::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
/*	case GAMMA_PARAM_FILE_NAME_READ:
		m_fileName.Assign(*static_cast<wxString*>(value)); break;
*/	default:
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void GammaNemaCalc::processData(GammaDataBase* pData)
{
	m_pDataIn = static_cast<GammaDataMatrix*>(pData);
	GammaDataMatrix* pDataOut(new GammaDataMatrix);

	floodFill(wxPoint(0,0), 0);
	marginalRemove();

	convolutionFilter(pDataOut);

	memcpy(m_pDataIn->data, pDataOut->data, 256*256*sizeof(uint32_t));

	wxLogStatus("Intg = %f, Diff = %f", getIntgUniformity(GAMMA_AREA_CFOV), 
		getDiffUniformity(GAMMA_AREA_CFOV, GAMMA_DIRECTION_X));

	pushData(pDataOut);
	delete pDataOut;
}

////////////////////////////////////////////////////////////////////////////////

void GammaNemaCalc::floodFill(wxPoint start, uint32_t color)
{
	uint32_t threshold = m_pDataIn->max * 0;//2/4;

	std::list<wxPoint> queue;

	if(threshold < m_pDataIn->data[POINT(start.x,start.y)])
	{
		wxLogStatus("Wrong parameters!");
		return;
	}

	queue.push_back(start);

	while(!queue.empty())
	{
		wxPoint w(queue.back()), e(queue.back());
		queue.pop_back();

		while( (0 <= w.x-1) && (m_pDataIn->data[POINT(w.x-1, w.y)] < threshold) )
		{
			w.x--;
		}

		while( (e.x+1 <= 255) && (m_pDataIn->data[POINT(e.x+1, e.y)] < threshold) )
		{
			e.x++;
		}
		
		for(wxPoint n(w); n.x <= e.x; n.x++)
		{
			m_pDataIn->data[POINT(n.x, n.y)] = UINT32_MAX;

			if( (0 <= n.y-1) && (m_pDataIn->data[POINT(n.x, n.y-1)] < threshold) )
			{
				queue.push_back(wxPoint(n.x, n.y-1));
			}

			if( (n.y+1 <= 255) && (m_pDataIn->data[POINT(n.x, n.y+1)] < threshold) )
			{
				queue.push_back(wxPoint(n.x, n.y+1));
			}
		}
	}

	for(uint32_t y = 0; y <= 255; y++)
	{
		for(uint32_t x = 0; x <= 255; x++)
		{
			if(UINT32_MAX == m_pDataIn->data[POINT(x,y)])
			{
				m_pDataIn->data[POINT(x,y)] = color;
			}
		}
	}
}

void GammaNemaCalc::marginalRemove()
{
	for(uint32_t y = 1; y <= 254; y++)
	{
		for(uint32_t x = 1; x <= 254; x++)
		{
			if( 0 == m_pDataIn->data[POINT(x  ,y-1)] &&
				0 == m_pDataIn->data[POINT(x+1,y  )] &&
				0 == m_pDataIn->data[POINT(x  ,y+1)] &&
				0 == m_pDataIn->data[POINT(x-1,y  )] )
			{
				m_pDataIn->data[POINT(x,y)] = 0;
			}
		}
	}
}

void GammaNemaCalc::convolutionFilter(GammaDataMatrix* pDataOut)
{
	static uint32_t filter[] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1 };

	//uint32_t* dataFiltered = new uint32_t[256*256]();
	pDataOut->max = 0;

	for(uint32_t y = 1; y <= 254; y++)
	{
		for(uint32_t x = 1; x <= 254; x++)
		{
			pDataOut->data[POINT(x,y)] = (0 != m_pDataIn->data[POINT(x,y)]) * 
				( filter[0] * m_pDataIn->data[POINT(x-1,y-1)]
				+ filter[1] * m_pDataIn->data[POINT(x  ,y-1)]
				+ filter[2] * m_pDataIn->data[POINT(x+1,y-1)]
				+ filter[3] * m_pDataIn->data[POINT(x-1,y  )]
				+ filter[4] * m_pDataIn->data[POINT(x  ,y  )]
				+ filter[5] * m_pDataIn->data[POINT(x+1,y  )]
				+ filter[6] * m_pDataIn->data[POINT(x-1,y+1)]
				+ filter[7] * m_pDataIn->data[POINT(x  ,y+1)]
				+ filter[8] * m_pDataIn->data[POINT(x+1,y+1)] );

			uint32_t div = 
				( filter[0] * (0 != m_pDataIn->data[POINT(x-1,y-1)])
				+ filter[1] * (0 != m_pDataIn->data[POINT(x  ,y-1)])
				+ filter[2] * (0 != m_pDataIn->data[POINT(x+1,y-1)])
				+ filter[3] * (0 != m_pDataIn->data[POINT(x-1,y  )])
				+ filter[4] * (0 != m_pDataIn->data[POINT(x  ,y  )])
				+ filter[5] * (0 != m_pDataIn->data[POINT(x+1,y  )])
				+ filter[6] * (0 != m_pDataIn->data[POINT(x-1,y+1)])
				+ filter[7] * (0 != m_pDataIn->data[POINT(x  ,y+1)])
				+ filter[8] * (0 != m_pDataIn->data[POINT(x+1,y+1)]) );
			
			if(0 != div)
			{
				pDataOut->data[POINT(x,y)] /= div;
			}

			if(pDataOut->max < pDataOut->data[POINT(x,y)])
			{
				pDataOut->max = pDataOut->data[POINT(x,y)];
			}
		}
	}
}

double GammaNemaCalc::getIntgUniformity(GammaArea_e area)
{
	uint32_t radius = 0;

	switch(area)
	{
	default:
	case GAMMA_AREA_FOV:
		radius = m_fieldOfView.radius; break;
	case GAMMA_AREA_UFOV:
		radius = m_fieldOfView.radius * 95 / 100; break;
	case GAMMA_AREA_CFOV:
		radius = m_fieldOfView.radius * 95 * 75 / 10000; break;
	}

	uint32_t min = UINT32_MAX;
	uint32_t max = 0;

	for(uint32_t y = 0; y <= 255; y++)
	{
		for(uint32_t x = 0; x <= 255; x++)
		{
			if(0 == m_pDataIn->data[POINT(x,y)] || radius < 
				sqrt(pow((double)m_fieldOfView.center.x - x, 2.0) + pow((double)m_fieldOfView.center.y - y, 2.0)))
			{
				continue;
			}

			if(m_pDataIn->data[POINT(x,y)] < min)
			{
				min = m_pDataIn->data[POINT(x,y)];
			}

			if(max < m_pDataIn->data[POINT(x,y)])
			{
				max = m_pDataIn->data[POINT(x,y)];
			}
		}
	}

	return 100.0 * (max - min) / (max + min);
}

double GammaNemaCalc::getDiffUniformity(GammaArea_e area, GammaDirection_e direction)
{
	uint32_t radius = 0;

	switch(area)
	{
	default:
	case GAMMA_AREA_FOV:
		radius = m_fieldOfView.radius; break;
	case GAMMA_AREA_UFOV:
		radius = m_fieldOfView.radius * 95 / 100; break;
	case GAMMA_AREA_CFOV:
		radius = m_fieldOfView.radius * 95 * 75 / 10000; break;
	}

	double result = 0;
	
	for(uint32_t y = 2; y <= 253; y++)
	{
		for(uint32_t x = 2; x <= 253; x++)
		{
			uint32_t min = UINT32_MAX;
			uint32_t max = 0;

			switch(direction)
			{
			case GAMMA_DIRECTION_X:
				if( 0 == m_pDataIn->data[POINT(x-2,y)] || radius < 
					sqrt(pow((double)m_fieldOfView.center.x - x-2, 2.0) + pow((double)m_fieldOfView.center.y - y, 2.0)) ||
					0 == m_pDataIn->data[POINT(x+2,y)] || radius < 
					sqrt(pow((double)m_fieldOfView.center.x - x+2, 2.0) + pow((double)m_fieldOfView.center.y - y, 2.0)) )
				{
					continue;
				}

				for(int32_t i = -2; i <= 2; i++)
				{
					if(m_pDataIn->data[POINT(x+i,y)] < min)
					{
						min = m_pDataIn->data[POINT(x+i,y)];
					}

					if(max < m_pDataIn->data[POINT(x+i,y)])
					{
						max = m_pDataIn->data[POINT(x+i,y)];
					}
				}
				break;
			case GAMMA_DIRECTION_Y:
				if( 0 == m_pDataIn->data[POINT(x,y-2)] || radius < 
					sqrt(pow((double)m_fieldOfView.center.x - x, 2.0) + pow((double)m_fieldOfView.center.y - y-2, 2.0)) ||
					0 == m_pDataIn->data[POINT(x,y+2)] || radius < 
					sqrt(pow((double)m_fieldOfView.center.x - x, 2.0) + pow((double)m_fieldOfView.center.y - y+2, 2.0)) )
				{
					continue;
				}

				for(int32_t i = -2; i <= 2; i++)
				{
					if(m_pDataIn->data[POINT(x,y+i)] < min)
					{
						min = m_pDataIn->data[POINT(x,y+i)];
					}

					if(max < m_pDataIn->data[POINT(x,y+i)])
					{
						max = m_pDataIn->data[POINT(x,y+i)];
					}
				}
				break;
			}
			
			if(result < 100.0 * (max - min) / (max + min))
			{
				result = 100.0 * (max - min) / (max + min);
			}
		}
	}

	return result;
}
