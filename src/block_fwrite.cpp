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

void GammaBlockFileWrite::processData(GammaDataBase* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaDataItems* pDataIn = dynamic_cast<GammaDataItems*>(pData);

	for( std::vector<GammaItem>::iterator iItem = pDataIn->data.begin();
		iItem != pDataIn->data.end(); iItem++ )
	{
		m_file.Write(&(*iItem), sizeof(GammaItem));
	}
}
