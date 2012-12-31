/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_sm.h"

wxThread::ExitCode GammaBlockTransSM::Entry()
{
	wxLogStatus("%s - started", __FUNCTION__);
	wxMutexLocker locker(m_threadRunMutex);

	unsigned long int timeCounter = 0;
	unsigned long int timeSend = m_timeDiff;
	unsigned short int* t_matrix = new unsigned short int[0x10000]();
	unsigned short int t_max = 0;

	while ( ShouldBeRunning() )
	{
		if ( DataReady() )
		{
			wxSharedPtr<GammaDataBase> dataIn(DataGet());
			GammaDataItems* pDataIn = static_cast<GammaDataItems*>(dataIn.get());
			wxMutexLocker locker(*pDataIn);
			
			for ( wxVector<GammaItem>::iterator it = pDataIn->data.begin();
				it != pDataIn->data.end(); it++ )
			{
				switch ( (*it).type )
				{
				case GAMMA_ITEM_POINT:
					{
						t_matrix[0x100 * (*it).data.point.x + (*it).data.point.y] += 1;

						if ( t_max < t_matrix[0x100 * (*it).data.point.x + (*it).data.point.y] )
						{
							t_max = t_matrix[0x100 * (*it).data.point.x + (*it).data.point.y];
						}
						break;
					}
				case GAMMA_ITEM_TMARKER:
					{
						timeCounter = (*it).data.time;

						if ( timeSend < timeCounter )
						{
							timeSend += m_timeDiff;
							
							{
								GammaDataMatrix* pDataOut(new GammaDataMatrix);
								pDataOut->dateTime = pDataIn->dateTime;
								memcpy(pDataOut->data, t_matrix, 
									sizeof(unsigned short int) * 0x10000);
								pDataOut->max = t_max;
								DataPush(pDataOut);
							}

							if (!m_integrate)
							{
								memset(t_matrix, 0x00, 
									sizeof(unsigned short int) * 0x10000);
								t_max = 0;
							}
						}
						break;
					}
				case GAMMA_ITEM_TRIGGER:
				default:
					{
						break;
					}
				}
			}
		}
	}

	delete[] t_matrix;

	wxLogStatus("%s - stoped", __FUNCTION__);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool GammaBlockTransSM::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_INTEGRATE_TIME:
		m_timeDiff = *static_cast<unsigned int*>(value); break;
	case GAMMA_PARAM_IMG_INTEGRATE_ENABLED:
		m_integrate = *static_cast<bool*>(value); break;
	default:
		return false;
	}
	
	return true;
}
