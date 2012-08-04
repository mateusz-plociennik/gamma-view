/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_sm.h"

wxThread::ExitCode GammaBlockTransSM::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataItems* blockDataIn = static_cast<GammaDataItems*>(DataGet());
			GammaDataMatrix* blockDataOut = new GammaDataMatrix;

			blockDataIn->Lock();
			blockDataOut->datetime = blockDataIn->datetime;
			for ( wxVector<GammaItem>::iterator it = blockDataIn->data.begin();
				it != blockDataIn->data.end(); it++ )
			{
				if ((*it).type == GAMMA_ITEM_POINT)
				{
					blockDataOut->data[256 * (*it).data.point.x + (*it).data.point.y] += 1;

					if ( blockDataOut->max < blockDataOut->data[256 * (*it).data.point.x + (*it).data.point.y] )
					{
						blockDataOut->max = blockDataOut->data[256 * (*it).data.point.x + (*it).data.point.y];
					}
				}
			}
			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();
			DataPush(blockDataOut);
		}
	}

	return 0;
}
