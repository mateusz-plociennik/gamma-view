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
		:
		GammaBlockBase(pManager, GAMMA_QUEUE_BLOCK_FWRITE)
{
}

GammaBlockFileWrite::~GammaBlockFileWrite()
{
	//
}

wxThread::ExitCode GammaBlockFileWrite::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	wxFile file;

	file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M%S.gvb"), wxFile::write);
	file.Write("GVB", 3);
	while( ShouldBeRunning() )
	{
		if(DataReady())
		{
			wxSharedPtr<GammaDataBase> dataIn(DataGet());
			GammaDataItems* pDataIn = static_cast<GammaDataItems*>(dataIn.get());
			wxMutexLocker locker(*pDataIn);

			for( std::vector<GammaItem>::iterator iItem = pDataIn->data.begin();
				iItem != pDataIn->data.end(); iItem++ )
			{
				file.Write(&(*iItem), sizeof(GammaItem));
			}
		}
	}
	file.Close();

	return 0;
}
