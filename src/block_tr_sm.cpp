/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include <vector>

#include "block_tr_sm.h"
#include "config.h"

GammaBlockTransSM::GammaBlockTransSM(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_intgTime(32000)
	, m_intgEnabled(false)
{
	timeCounter = 0;
	timeSend = m_intgTime;
	t_matrix = new uint32_t[256*256]();
	t_max = 0;
	t_sum = 0;
}

GammaBlockTransSM::~GammaBlockTransSM()
{
	delete[] t_matrix;
}

void GammaBlockTransSM::processData(GammaDataBase* pData)
{
	GammaDataItems* pDataIn = dynamic_cast<GammaDataItems*>(pData);
	
	for(std::vector<GammaItem>::iterator it = pDataIn->data.begin();
		it != pDataIn->data.end(); it++)
	{
		switch( (*it).type )
		{
		case GAMMA_ITEM_POINT:
			t_matrix[POINT((*it).data.point.x, (*it).data.point.y)] += 1;
			t_sum += 1;

			if( t_max < t_matrix[POINT((*it).data.point.x, (*it).data.point.y)] )
			{
				t_max = t_matrix[POINT((*it).data.point.x, (*it).data.point.y)];
			}
			break;
		case GAMMA_ITEM_TMARKER:
			timeCounter = (*it).data.time;

			if( timeSend < timeCounter )
			{
				timeSend += m_intgTime;
				
				{
					GammaDataMatrix* pDataOut(new GammaDataMatrix);
					pDataOut->dateTime = pDataIn->dateTime;
					memcpy(pDataOut->data, t_matrix, 256 * 256 * sizeof(uint32_t));
					pDataOut->max = t_max;
					pDataOut->sum = t_sum;
					pushData(pDataOut);
					delete pDataOut;
				}

				if(!m_intgEnabled)
				{
					memset(t_matrix, 0, 256 * 256 * sizeof(uint32_t));
					t_max = 0;
					t_sum = 0;
				}
			}
			break;
		case GAMMA_ITEM_TRIGGER:
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

bool GammaBlockTransSM::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_INTEGRATE_TIME:
		m_intgTime = *static_cast<unsigned int*>(value); break;
	case GAMMA_PARAM_IMG_INTEGRATE_ENABLED:
		m_intgEnabled = *static_cast<bool*>(value); break;
	default:
		return false;
	}
	
	return true;
}
