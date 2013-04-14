/**
 * @file	b_uniform.cpp
 * @brief	uniformity filter
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "b_uniform.h"

#include <vector>


#include "config.h"

GammaUniformity::GammaUniformity(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
{
	for(uint32_t i = 0; i < 256 * 256; i++)
	{
		m_matrix[i] = RAND_MAX * i / 32768 - RAND_MAX;
	}
}

void GammaUniformity::processData(GammaDataBase* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaDataItems* pDataIn = dynamic_cast<GammaDataItems*>(pData);
	GammaDataItems* dataOut = new GammaDataItems();
	
	for(std::vector<GammaItem>::iterator it = pDataIn->data.begin();
		it != pDataIn->data.end(); 
		it++)
	{
		switch ((*it).type)
		{
		case GAMMA_ITEM_POINT:
			for(int iCount = corrCount((*it).data.point.x,(*it).data.point.y); 
				iCount != 0; iCount--)
			{
				dataOut->data.push_back((*it));
			}
			break;
		default:
			dataOut->data.push_back((*it));
			break;
		}
	}

	pushData(dataOut);
	delete dataOut;
}

////////////////////////////////////////////////////////////////////////////////

bool GammaUniformity::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
/*	case GAMMA_PARAM_IMG_INTEGRATE_TIME:
		m_intgTime = *static_cast<unsigned int*>(value); break;
	case GAMMA_PARAM_IMG_INTEGRATE_ENABLED:
		m_intgEnabled = *static_cast<bool*>(value); break;
*/	default:
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

int GammaUniformity::corrCount(uint8_t x, uint8_t y)
{
	int32_t r = rand();

	if(r < m_matrix[POINT(x,y)])
	{
		return 2; // Add
	}
	else if(m_matrix[POINT(x,y)] < -r)
	{
		return 0; // Remove
	}
	else
	{
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////////

void GammaUniformity::setMatrix(uint32_t* matrix, uint32_t norm)
{
	for(uint32_t i = 0; i < 256*256; i++)
	{
		m_matrix[i] = RAND_MAX - (RAND_MAX * norm / matrix[i]);
	}
}
