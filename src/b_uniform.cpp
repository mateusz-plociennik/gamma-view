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
		:
		GammaBlockBase(pManager, GAMMA_QUEUE_BLOCK_UNIFORMITY)
{
	for(uint32_t i = 0; i < 256 * 256; i++)
	{
		m_matrix[i] = RAND_MAX * i / 32768 - RAND_MAX;
	}
}

wxThread::ExitCode GammaUniformity::Entry()
{
	wxLogStatus("%s - started", __FUNCTION__);
	wxMutexLocker locker(m_threadRunMutex);

	while(DataReady())
	{
		wxSharedPtr<GammaDataBase> dataIn(DataGet());
		GammaDataItems* pDataIn = static_cast<GammaDataItems*>(dataIn.get());
		wxMutexLocker locker(*pDataIn);

		GammaDataItems* dataOut = new GammaDataItems();
		
		for(std::vector<GammaItem>::iterator it = pDataIn->data.begin();
			it != pDataIn->data.end(); 
			it++)
		{
			switch ((*it).type)
			{
			case GAMMA_ITEM_POINT:
				for(int iCount = CorrCount((*it).data.point.x,(*it).data.point.y); 
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

		DataPush(dataOut);
	}

	wxLogStatus("%s - stoped", __FUNCTION__);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool GammaUniformity::SetParam(GammaParam_e param, void* value)
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

int GammaUniformity::CorrCount(uint8_t x, uint8_t y)
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

void GammaUniformity::SetMatrix(uint32_t* matrix, uint32_t norm)
{
	for(uint32_t i = 0; i < 256*256; i++)
	{
		m_matrix[i] = RAND_MAX - (RAND_MAX * norm / matrix[i]);
	}
}
