/**
 * @file	block_tr_mi.cpp
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_tr_mi.h"
#include "wx/time.h"

GammaBlockTransMI::GammaBlockTransMI(wxWindow* parent)
	: m_frame(parent, wxID_ANY, wxT("gamma-view"))
{
	//m_image = new wxImage(256,256);
	m_frame.SetClientSize(256, 256);
	m_frame.Show();
}

wxThread::ExitCode GammaBlockTransMI::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		if (DataReady())
		{
			GammaDataMatrix* blockDataIn = 
				static_cast<GammaDataMatrix*>(DataGet());
			GammaDataImage* blockDataOut = new GammaDataImage;

			blockDataIn->Lock();
			wxDateTime tStart = wxDateTime::UNow();
			
			for ( unsigned int x = 0; x < 256; x++ )
			{
				for ( unsigned int y = 0; y < 256; y++ )
				{
					if (blockDataIn->data[256 * x + y] != 0)
					{
						/*
						unsigned char val = 
							(255 * blockDataIn->data[256 * x + y] / blockDataIn->max);
						blockDataOut->data->SetRGB( x, y, val, val, val );
						*/
						blockDataOut->data->GetData()[3 * (256 * x + y) + 0] = 
						blockDataOut->data->GetData()[3 * (256 * x + y) + 1] = 
						blockDataOut->data->GetData()[3 * (256 * x + y) + 2] = 
							(255 * blockDataIn->data[256 * x + y] / blockDataIn->max);
					}
				}
			}
			wxDateTime tStop = wxDateTime::UNow();

			wxTimeSpan tDiff = tStop.Subtract(tStart);
			wxLogStatus("Calculation time = %s", tDiff.Format("%l").c_str());

			blockDataIn->Unlock();
			blockDataIn->Unsubscribe();

			//DataPush(blockDataOut);
			blockDataOut->data->SaveFile( "now.bmp", wxBITMAP_TYPE_BMP );
			
			m_frame.SetImage(*(blockDataOut->data));
			
			delete blockDataOut;
		}
	}

	return 0;
}
