/**
 * @file	block_fwrite.cpp
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fwrite.h"

wxThread::ExitCode GammaBlockFileWrite::Entry()
{
	wxFile file;

	file.Open(wxDateTime::Now().Format("%Y%m%d_%H%M%S.gvb"), wxFile::write);
	file.Write("GVB", 3);
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataItems* blockDataIn = (GammaDataItems*)DataGet();

			blockDataIn->Lock();
			for ( wxVector<GammaItem>::iterator i = blockDataIn->data.begin();
				i != blockDataIn->data.end(); i++ )
			{
				file.Write(&(*i), sizeof(GammaItem));
			}
			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();
		}
	}
	file.Close();

	return 0;
}
