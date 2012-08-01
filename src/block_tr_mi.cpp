/**
 * @file	block_tr_mi.cpp
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_mi.h"

wxThread::ExitCode GammaBlockTransMI::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataMatrix* blockDataIn = (GammaDataMatrix*)(DataGet());
			GammaDataImage* blockDataOut = new GammaDataImage;
			unsigned int max = 0;

			blockDataIn->Lock();
			for ( unsigned int i = 0; i < (256 * 256); i++ )
			{
				if ( max < blockDataIn->data[i] )
				{
					max = blockDataIn->data[i];
				}
			}

			for ( unsigned int x = 0; x < 256; x++ )
			{
				for ( unsigned int y = 0; y < 256; y++ )
				{
					blockDataOut->data->SetRGB( wxRect(x,y,1,1), 
						blockDataIn->data[256 * x + y] / max,
						blockDataIn->data[256 * x + y] / max,
						blockDataIn->data[256 * x + y] / max );
				}
			}

			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();

			//DataPush(blockDataOut);
			blockDataOut->data->SaveFile( 
				"now.bmp",
				wxBITMAP_TYPE_BMP );
		}
	}

	return 0;
}
