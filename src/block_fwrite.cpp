/**
 * @file	block_fwrite.cpp
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include <vector>

#include "block_fwrite.h"
#include "config.h"

GammaBlockFileWrite::GammaBlockFileWrite(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
{
	m_file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M%S.gvb"), wxFile::write);
	m_file.Write("GVB", 3);
}

GammaBlockFileWrite::~GammaBlockFileWrite()
{
	m_file.Close();
}

void GammaBlockFileWrite::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_ITEMS == pData->type);
	GammaItems* pDataIn = dynamic_cast<GammaItems*>(pData);

	for( std::vector<GammaItem>::iterator iItem = pDataIn->items.begin();
		iItem != pDataIn->items.end(); iItem++ )
	{
		m_file.Write(&(*iItem), sizeof(GammaItem));
	}
}
