/////////////////////////////////////////////////////////////////////////////
// Name:        block_trans.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_trans.h"

wxThread::ExitCode GammaBlockTrans::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		GammaBlockData<unsigned char*>* blockDataIn = 
			new GammaBlockData<unsigned char*>;//BlockDataGet();

		GammaBlockData<std::list<GammaItem>>* blockDataOut = 
			new GammaBlockData<std::list<GammaItem>>;

		for (unsigned short int i = 0; i < 256; i++)
		{
			blockDataOut->data.resize(blockDataOut->data.size() + 1);

			if ( (blockDataIn->data[2 * i + 0] == 0xFF) && 
				(blockDataIn->data[2 * i + 1] == 0xFF) )
			{
				blockDataOut->data.back().type = GAMMA_ITEM_TMARKER;
				blockDataOut->data.back().data.time = 2; //TODO
				continue;
			}
			else if ( (blockDataIn->data[2 * i + 0] == 0x00) && 
				(blockDataIn->data[2 * i + 1] == 0x00) )
			{
				blockDataOut->data.back().type = GAMMA_ITEM_TRIGGER;
				blockDataOut->data.back().data.time = 3; //TODO
				continue;
			}
			else
			{
				blockDataOut->data.back().type = GAMMA_ITEM_POINT;
				blockDataOut->data.back().data.point.x = blockDataIn->data[2 * i + 0];
				blockDataOut->data.back().data.point.y = blockDataIn->data[2 * i + 1];
				blockDataOut->data.back().data.point.z = -1; //Not Available
			}
		}
		BlockDataPush(blockDataOut);
	}

	return 0;
}
