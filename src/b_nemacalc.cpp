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
	wxLogStatus("%s", __FUNCTION__);
}

GammaNemaCalc::~GammaNemaCalc()
{
	wxLogStatus("%s", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////

void GammaNemaCalc::processData(GammaData* pData)
{	
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_MATRIX == pData->type);
	m_pDataIn = static_cast<GammaMatrix*>(pData);
	GammaMatrix* pDataOut(new GammaMatrix);

	floodFill(wxPoint(0,0), 0);
	marginalRemove();

	convolutionFilter(pDataOut);

	memcpy(m_pDataIn->matrix, pDataOut->matrix, sizeof(wxUint32) * 256 * 256);
	pDataOut->time = m_pDataIn->time;

	wxLogStatus("Intg = %f, Diff(X) = %f, Diff(Y) = %f", 
		getIntgUniformity(), 
		getDiffUniformity(GAMMA_DIRECTION_X), 
		getDiffUniformity(GAMMA_DIRECTION_Y));

	pushData(pDataOut);
	delete pDataOut;
}

////////////////////////////////////////////////////////////////////////////////

void GammaNemaCalc::floodFill(wxPoint start, wxUint32 colour)
{
	wxUint32 threshold = m_pDataIn->eventMax * 0;//2/4;

	std::list<wxPoint> queue;

	if(threshold < m_pDataIn->matrix[POINT(start.x,start.y)])
	{
		wxLogStatus("Wrong parameters!");
		return;
	}

	queue.push_back(start);

	while(!queue.empty())
	{
		wxPoint w(queue.back()), e(queue.back());
		queue.pop_back();

		while( (0 <= w.x-1) && (m_pDataIn->matrix[POINT(w.x-1, w.y)] < threshold) )
		{
			w.x--;
		}

		while( (e.x+1 <= 255) && (m_pDataIn->matrix[POINT(e.x+1, e.y)] < threshold) )
		{
			e.x++;
		}
		
		for(wxPoint n(w); n.x <= e.x; n.x++)
		{
			m_pDataIn->matrix[POINT(n.x, n.y)] = UINT32_MAX;

			if( (0 <= n.y-1) && (m_pDataIn->matrix[POINT(n.x, n.y-1)] < threshold) )
			{
				queue.push_back(wxPoint(n.x, n.y-1));
			}

			if( (n.y+1 <= 255) && (m_pDataIn->matrix[POINT(n.x, n.y+1)] < threshold) )
			{
				queue.push_back(wxPoint(n.x, n.y+1));
			}
		}
	}

	for(wxUint32 y = 0; y <= 255; y++)
	{
		for(wxUint32 x = 0; x <= 255; x++)
		{
			if(UINT32_MAX == m_pDataIn->matrix[POINT(x,y)])
			{
				m_pDataIn->matrix[POINT(x,y)] = colour;
			}
		}
	}
}

void GammaNemaCalc::marginalRemove()
{
	for(wxUint32 y = 1; y <= 254; y++)
	{
		for(wxUint32 x = 1; x <= 254; x++)
		{
			if( 0 == m_pDataIn->matrix[POINT(x  ,y-1)] &&
				0 == m_pDataIn->matrix[POINT(x+1,y  )] &&
				0 == m_pDataIn->matrix[POINT(x  ,y+1)] &&
				0 == m_pDataIn->matrix[POINT(x-1,y  )] )
			{
				m_pDataIn->matrix[POINT(x,y)] = 0;
			}
		}
	}
}

void GammaNemaCalc::convolutionFilter(GammaMatrix* pDataOut)
{
	static wxUint32 filter[] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1 };

	//wxUint32* dataFiltered = new wxUint32[256*256]();
	pDataOut->eventMax = 1;

	for(wxUint32 y = 1; y <= 254; y++)
	{
		for(wxUint32 x = 1; x <= 254; x++)
		{
			if(m_pDataIn->matrix[POINT(x,y)])
			{
				pDataOut->matrix[POINT(x,y)] =  
					( filter[0] * m_pDataIn->matrix[POINT(x-1,y-1)]
					+ filter[1] * m_pDataIn->matrix[POINT(x  ,y-1)]
					+ filter[2] * m_pDataIn->matrix[POINT(x+1,y-1)]
					+ filter[3] * m_pDataIn->matrix[POINT(x-1,y  )]
					+ filter[4] * m_pDataIn->matrix[POINT(x  ,y  )]
					+ filter[5] * m_pDataIn->matrix[POINT(x+1,y  )]
					+ filter[6] * m_pDataIn->matrix[POINT(x-1,y+1)]
					+ filter[7] * m_pDataIn->matrix[POINT(x  ,y+1)]
					+ filter[8] * m_pDataIn->matrix[POINT(x+1,y+1)] )
					/
					( filter[0] * (0 != m_pDataIn->matrix[POINT(x-1,y-1)])
					+ filter[1] * (0 != m_pDataIn->matrix[POINT(x  ,y-1)])
					+ filter[2] * (0 != m_pDataIn->matrix[POINT(x+1,y-1)])
					+ filter[3] * (0 != m_pDataIn->matrix[POINT(x-1,y  )])
					+ filter[4] * (0 != m_pDataIn->matrix[POINT(x  ,y  )])
					+ filter[5] * (0 != m_pDataIn->matrix[POINT(x+1,y  )])
					+ filter[6] * (0 != m_pDataIn->matrix[POINT(x-1,y+1)])
					+ filter[7] * (0 != m_pDataIn->matrix[POINT(x  ,y+1)])
					+ filter[8] * (0 != m_pDataIn->matrix[POINT(x+1,y+1)]) );

				if(pDataOut->eventMax < pDataOut->matrix[POINT(x,y)])
				{
					pDataOut->eventMax = pDataOut->matrix[POINT(x,y)];
				}
			}
			else
			{
				pDataOut->matrix[POINT(x,y)] = 0;
			}
		}
	}
}

double GammaNemaCalc::getIntgUniformity()
{
	wxUint32 min = UINT32_MAX;
	wxUint32 max = 0;

	for(wxUint32 y = 0; y <= 255; y++)
	{
		for(wxUint32 x = 0; x <= 255; x++)
		{
			if(0 == m_pDataIn->matrix[POINT(x, y)] || !POINT_INSIDE_FOV(POINT(x, y)))
			{
				continue;
			}

			if(m_pDataIn->matrix[POINT(x, y)] < min)
			{
				min = m_pDataIn->matrix[POINT(x, y)];
			}

			if(max < m_pDataIn->matrix[POINT(x, y)])
			{
				max = m_pDataIn->matrix[POINT(x, y)];
			}
		}
	}

	return 100.0 * (max - min) / (max + min);
}

double GammaNemaCalc::getDiffUniformity(GammaDirection_e direction)
{
	double result = 0;
	
	for(wxUint32 y = 2; y <= 253; y++)
	{
		for(wxUint32 x = 2; x <= 253; x++)
		{
			wxUint32 min = UINT32_MAX;
			wxUint32 max = 0;

			switch(direction)
			{
			case GAMMA_DIRECTION_X:
				if( 0 == m_pDataIn->matrix[POINT(x-2, y)] || !POINT_INSIDE_FOV(POINT(x-2, y)) ||
					0 == m_pDataIn->matrix[POINT(x+2, y)] || !POINT_INSIDE_FOV(POINT(x+2, y)) )
				{
					continue;
				}

				for(wxInt32 i = -2; i <= 2; i++)
				{
					if(m_pDataIn->matrix[POINT(x+i, y)] < min)
					{
						min = m_pDataIn->matrix[POINT(x+i, y)];
					}

					if(max < m_pDataIn->matrix[POINT(x+i, y)])
					{
						max = m_pDataIn->matrix[POINT(x+i, y)];
					}
				}
				break;
			case GAMMA_DIRECTION_Y:
				if( 0 == m_pDataIn->matrix[POINT(x, y-2)] || !POINT_INSIDE_FOV(POINT(x, y-2)) ||
					0 == m_pDataIn->matrix[POINT(x, y+2)] || !POINT_INSIDE_FOV(POINT(x, y+2)) )
				{
					continue;
				}

				for(wxInt32 i = -2; i <= 2; i++)
				{
					if(m_pDataIn->matrix[POINT(x, y+i)] < min)
					{
						min = m_pDataIn->matrix[POINT(x, y+i)];
					}

					if(max < m_pDataIn->matrix[POINT(x, y+i)])
					{
						max = m_pDataIn->matrix[POINT(x, y+i)];
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
