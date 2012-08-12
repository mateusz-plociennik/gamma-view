/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_sm.h"

wxThread::ExitCode GammaBlockTransSM::Entry()
{
	unsigned long int timeCounter = 0;
	unsigned long int timeSend = m_timeDiff;
	unsigned short int* t_matrix = new unsigned short int[256 * 256]();
	unsigned short int t_max = 0;

	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataItems* blockDataIn = static_cast<GammaDataItems*>(DataGet());
			
			blockDataIn->Lock();
			for ( wxVector<GammaItem>::iterator it = blockDataIn->data.begin();
				it != blockDataIn->data.end(); it++ )
			{
				if ( (*it).type == GAMMA_ITEM_POINT )
				{
					t_matrix[256 * (*it).data.point.x + (*it).data.point.y] += 1;

					if ( t_max < t_matrix[256 * (*it).data.point.x + (*it).data.point.y] )
					{
						t_max = t_matrix[256 * (*it).data.point.x + (*it).data.point.y];
					}
				}
				else if ( (*it).type == GAMMA_ITEM_TMARKER )
				{
					timeCounter = (*it).data.time;

					if ( timeSend < timeCounter )
					{
						timeSend += m_timeDiff;
						
						{
							GammaDataMatrix* blockDataOut = new GammaDataMatrix;
							blockDataOut->datetime = blockDataIn->datetime;
							memcpy(blockDataOut->data, t_matrix, 
								256 * 256 * sizeof(unsigned short int));
							blockDataOut->max = t_max;
							DataPush(blockDataOut);
						}

						if (!m_integrate)
						{
							delete[] t_matrix;
							t_matrix = new unsigned short int[256 * 256]();
							t_max = 0;
						}
					}
				}
			}
			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();
		}
	}

	return 0;
}
