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
	, m_bInitalized(false)
{
	for(wxUint32 i = 0; i < 256 * 256; i++)
	{
		m_matrix[i] = 2 * RAND_MAX * i / (256 * 256 - 1) - RAND_MAX;
	}
}

void GammaUniformity::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_ITEMS == pData->type);
	GammaItems* pDataIn = dynamic_cast<GammaItems*>(pData);

	if(m_bInitalized)
	{
		GammaItems* dataOut = new GammaItems();
		
		for(std::vector<GammaItem>::iterator it = pDataIn->items.begin();
			it != pDataIn->items.end(); 
			it++)
		{
			switch ((*it).type)
			{
			case GAMMA_ITEM_TYPE_POINT:
				for(int iCount = corrCount((*it).data.point.x,(*it).data.point.y); 
					iCount != 0; iCount--)
				{
					dataOut->items.push_back((*it));
				}
				break;
			default:
				dataOut->items.push_back((*it));
				break;
			}
		}

		pushData(dataOut);
		delete dataOut;
	}
	else
	{
		pushData(pDataIn);
	}
}

////////////////////////////////////////////////////////////////////////////////

wxInt32 GammaUniformity::setParam(GammaParam_e param, void* value)
{
	wxMutexLocker locker(m_processDataMutex);

	switch(param)
	{
	case GAMMA_PARAM_UNIFORM_MATRIX_SET:
		setMatrix(static_cast<wxUint32*>(value)); 
		m_bInitalized = true;
		break;
	default:
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

int GammaUniformity::corrCount(wxUint32 x, wxUint32 y)
{
	m_r = rand();

	if(m_r < m_matrix[POINT(x,y)])
	{
		return 2; // Add
	}
	else if(m_matrix[POINT(x,y)] < -m_r)
	{
		return 0; // Remove
	}
	else
	{
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////////

void GammaUniformity::setMatrix(wxUint32* matrix)
{
	wxUint32 eventSum = 0;
	wxUint32 points = 0;

	for(wxUint32 y = 0; y < 256; y++)
	{
		for(wxUint32 x = 0; x < 256; x++)
		{
			if(POINT_INSIDE_FOV(POINT(x, y)))
			{
				eventSum += matrix[POINT(x, y)];
				points++;
			}
		}
	}

	for(wxUint32 i = 0; i < 256*256; i++)
	{
		if(matrix[i] > (eventSum/points) * RAND_MAX / wxINT32_MAX)
		{
			m_matrix[i] = RAND_MAX * (eventSum/points) / matrix[i] - RAND_MAX;
		}
		else
		{
			m_matrix[i] = wxINT32_MAX;
		}
	}
}
