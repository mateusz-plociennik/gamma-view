/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_si.h"

wxThread::ExitCode GammaBlockTransSM::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataItems* blockDataIn = (GammaDataItems*)DataGet();
			unsigned int m[256*256];

			blockDataIn->Lock();
			for ( wxVector<GammaItem>::iterator it = blockDataIn->data.begin();
				it != blockDataIn->data.end(); it++ )
			{
				if ((*it).type == GAMMA_ITEM_POINT)
				{
					m[256 * (*it).data.point.x + (*it).data.point.y] += 1;
				}
			}
			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();
		}
	}
	file.Close();

	return 0;
}
