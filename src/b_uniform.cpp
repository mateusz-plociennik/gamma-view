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
	for(wxUint32 i = 0; i < 256 * 256; i++)
	{
		m_matrix[i] = RAND_MAX / 2;
	}
}

void GammaUniformity::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaItems* pDataIn = dynamic_cast<GammaItems*>(pData);
	GammaItems* dataOut = new GammaItems();
	
	for(std::vector<GammaItem>::iterator it = pDataIn->items.begin();
		it != pDataIn->items.end(); 
		it++)
	{
		switch ((*it).type)
		{
		case GAMMA_ITEM_POINT:
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

////////////////////////////////////////////////////////////////////////////////

bool GammaUniformity::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_UNIFORM_MATRIX_SET:
		setMatrix(static_cast<wxUint32*>(value)); break;
	default:
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

int GammaUniformity::corrCount(wxUint32 x, wxUint32 y)
{
	wxInt32 r = rand();

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

void GammaUniformity::setMatrix(wxUint32* matrix)
{
	wxUint32 max = 0;
	for(wxUint32 i = 0; i < 256*256; i++)
	{
		if(max < matrix[i])
		{
			max = matrix[i];
		}
	}

	for(wxUint32 i = 0; i < 256*256; i++)
	{
		if(0 != matrix[i])
		{
			m_matrix[i] = RAND_MAX - (RAND_MAX * (max/2) / matrix[i]);
		}
	}
}
