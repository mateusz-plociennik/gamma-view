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
}

GammaBlockFileWrite::~GammaBlockFileWrite()
{
	if(m_file.IsOpened())
	{
		m_file.Close();
	}
}

void GammaBlockFileWrite::processData(wxSharedPtr<GammaData> sDataIn)
{
	wxMutexLocker locker(m_processDataMutex);

	if(!m_file.IsOpened())
	{
		switch(sDataIn->type)
		{
		case GAMMA_DATA_TYPE_ITEMS:
			m_file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M%S.gvb"), 
				wxFile::write);
			m_file.Write("GVB", 3);
			break;
		case GAMMA_DATA_TYPE_MATRIX:
			m_file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M%S.gvm"), 
				wxFile::write);
			m_file.Write("GVM", 3);
			break;
		default:
			wxASSERT_MSG(0, _("Not implemented!"));
			return;
		}
	}

	switch(sDataIn->type)
	{
	case GAMMA_DATA_TYPE_ITEMS:
		{
			GammaItems* pDataIn = dynamic_cast<GammaItems*>(sDataIn.get());

			for( std::vector<GammaItem>::iterator iItem = pDataIn->items.begin();
				iItem != pDataIn->items.end(); iItem++ )
			{
				m_file.Write(&(*iItem), sizeof(GammaItem));
			}

			break;
		}
	case GAMMA_DATA_TYPE_MATRIX:
		{
			GammaMatrix* pDataIn = dynamic_cast<GammaMatrix*>(sDataIn.get());
			
			m_file.Write(pDataIn, sizeof(GammaMatrix));

			break;
		}
	default:
		break;
	}
}
