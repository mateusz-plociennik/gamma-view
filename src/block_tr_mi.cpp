/**
 * @file	block_tr_mi.cpp
 * @brief	translate matrix to image
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "block_mgmt.h"
#include "block_tr_mi.h"
#include "wx/time.h"

GammaBlockTransMI::GammaBlockTransMI(GammaManager* pManager) 
		:
		GammaBlockBase(pManager), 
		m_colormap(GAMMA_COLORMAP_GRAY),
		m_bInvert(false),
		m_brightness(0.0),
		m_contrast(1.0),
		m_gamma(1.0)
{
	//
}

wxThread::ExitCode GammaBlockTransMI::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	while ( ShouldBeRunning() )
	{
		if (DataReady())
		{
			wxSharedPtr<GammaDataBase> dataIn(DataGet());
			GammaDataMatrix* pDataIn = static_cast<GammaDataMatrix*>(dataIn.get());

			GammaDataImage* pDataOut(new GammaDataImage);

			pDataIn->Lock();
			wxDateTime tStart = wxDateTime::UNow();	

			SetColour(m_colormap, 0, pDataIn->max);
			pDataOut->data->SetRGB( wxRect(0, 0, 256, 256), 
				m_colour.Red(), m_colour.Green(), m_colour.Blue() );

			for ( unsigned int x = 0; x < 256; x++ )
			{
				for ( unsigned int y = 0; y < 256; y++ )
				{
					if (pDataIn->data[256 * x + y] != 0)
					{
						/*
						unsigned char val = 
							(255 * pDataIn->data[256 * x + y] / pDataIn->max);
						*/
						
						SetColour(m_colormap, pDataIn->data[256 * x + y], pDataIn->max);
						pDataOut->data->SetRGB( x, y, 
							m_colour.Red(), m_colour.Green(), m_colour.Blue() );
						/*
						pDataOut->data->GetData()[3 * (256 * x + y) + 0] = 
						pDataOut->data->GetData()[3 * (256 * x + y) + 1] = 
						pDataOut->data->GetData()[3 * (256 * x + y) + 2] = 
							(255 * pDataIn->data[256 * x + y] / pDataIn->max);\
						*/
					}
				}
				SetColour(m_colormap, 255-x, 255);
				pDataOut->data->SetRGB( 255, x, 
					m_colour.Red(), 
					m_colour.Green(), 
					m_colour.Blue() );
			}
			wxDateTime tStop = wxDateTime::UNow();
			wxTimeSpan tDiff = tStop.Subtract(tStart);
			wxLogStatus("%s - Calculation time = %s", __FUNCTION__, tDiff.Format("%l").c_str());

			pDataIn->Unlock();

			//DataPush(pDataOut);
			//pDataOut->data->SaveFile( "now.bmp", wxBITMAP_TYPE_BMP );
			
			//m_frame.SetImage(*(pDataOut->data));
			GetManager()->PresentationTierSetParam(GAMMA_PARAM_IMG_DATA, (void*)pDataOut->data);
			
			delete pDataOut;
		}
	}

	return 0;
}

void GammaBlockTransMI::SetColour(GammaColormap_e colormap, unsigned int index, unsigned int max)
{
	double r = 0.0, g = 0.0, b = 0.0;
	double x = (double)index / max;

	x = m_contrast * pow(x, m_gamma) + m_brightness;

	if (m_bInvert)
	{
		x = 1.0 - x;
	}
	if (x < 0.0)
	{
		x = 0.0;
	}
	if (1.0 < x)
	{
		x = 1.0;
	}

	switch(colormap)
	{
	case GAMMA_COLORMAP_AUTUMN:
		{
			r = 1.0;
			g = x;
			b = 0.0;
			break;
		}
	case GAMMA_COLORMAP_BONE:
		{
			r = (x < 3.0/4.0) * (7.0/8.0 * x) 
				+ (x >= 3.0/4.0) * (11.0/8.0 * x - 3.0/8.0); 
			g = (x < 3.0/8.0) * (7.0/8.0 * x)
				+ (x >= 3.0/8.0 && x < 3.0/4.0) * (29.0/24.0 * x - 1.0/8.0)
				+ (x >= 3.0/4.0) * (7.0/8.0 * x + 1.0/8.0);
			b = (x < 3.0/8.0) * (29.0/24.0 * x) 
				+ (x >= 3.0/8.0) * (7.0/8.0 * x + 1.0/8.0);
			break;
		}
	case GAMMA_COLORMAP_COOL:
		{
			r = x;
			g = 1.0 - r;
			b = 1.0;
			break;
		}
	case GAMMA_COLORMAP_COPPER:
		{
		  r = (x < 4.0/5.0) * (5.0/4.0 * x) 
				+ (x >= 4.0/5.0);
			g = 4.0/5.0 * x;
			b = 1.0/2.0 * x;
			break;
		}
	case GAMMA_COLORMAP_HOT:
		{
			r = (x < 2.0/5.0) * (5.0/2.0 * x) 
				+ (x >= 2.0/5.0);
			g = (x >= 2.0/5.0 && x < 4.0/5.0) * (5.0/2.0 * x - 1.0) 
				+ (x >= 4.0/5.0);
			b = (x >= 4.0/5.0) * (5.0 * x - 4.0);
			break;
		}
	case GAMMA_COLORMAP_HSV:
		{
			r = (x < 1.0/6.0)
				+ (x >= 1.0/6.0 && x < 2.0/6.0) * (-6.0 * x + 2.0)
				+ (x >= 4.0/6.0 && x < 5.0/6.0) * (6.0 * x - 4.0)
				+ (x >= 5.0/6.0);
			g = (x < 1.0/6.0) * (6.0 * x)
				+ (x >= 1.0/6.0 && x < 3.0/6.0) 
				+ (x >= 3.0/6.0 && x < 4.0/6.0) * (-6.0 * x + 4.0);
			b = (x >= 2.0/6.0 && x < 3.0/6.0) * (6.0 * x - 2.0) 
				+ (x >= 3.0/6.0 && x < 5.0/6.0)
				+ (x >= 5.0/6.0) * (-6.0 * x + 6.0);
			break;
		}
	case GAMMA_COLORMAP_JET:
		{
			r = (x >= 3.0/8.0 && x < 5.0/8.0) * (4.0 * x - 3.0/2.0)
				+ (x >= 5.0/8.0 && x < 7.0/8.0) 
				+ (x >= 7.0/8.0) * (-4.0 * x + 9.0/2.0);
			g = (x >= 1.0/8.0 && x < 3.0/8.0) * (4.0 * x - 1.0/2.0)
				+ (x >= 3.0/8.0 && x < 5.0/8.0) 
				+ (x >= 5.0/8.0 && x < 7.0/8.0) * (-4.0 * x + 7.0/2.0);
			b = (x < 1.0/8.0) * (4.0 * x + 1.0/2.0) 
				+ (x >= 1.0/8.0 && x < 3.0/8.0)
				+ (x >= 3.0/8.0 && x < 5.0/8.0) * (-4.0 * x + 5.0/2.0);
			break;
		}
	case GAMMA_COLORMAP_OCEAN:
		{
			r = (x >= 2.0/3.0) * (3.0 * x - 2.0);
			g = (x >= 1.0/3.0) * (3.0/2.0 * x - 1.0/2.0);
			b = x;
			break;
		}
	case GAMMA_COLORMAP_PINK:
		{
			r = sqrt( (x < 3.0/8.0) * (14.0/9.0 * x) 
				+ (x >= 3.0/8.0) * (2.0/3.0 * x + 1.0/3.0) );
			g = sqrt( (x < 3.0/8.0) * (2.0/3.0 * x)
				+ (x >= 3.0/8.0 && x < 3.0/4.0) * (14.0/9.0 * x - 1.0/3.0)
				+ (x >= 3.0/4.0) * (2.0/3.0 * x + 1.0/3.0) );
			b = sqrt( (x < 3.0/4.0) * (2.0/3.0 * x) 
				+ (x >= 3.0/4.0) * (2.0 * x - 1.0) );
			break;
		}
	case GAMMA_COLORMAP_RAINBOW:
		{
			r = (x < 2.0/5.0) 
				+ (x >= 2.0/5.0 && x < 3.0/5.0) * (-5.0 * x + 3.0)
				+ (x >= 4.0/5.0) * (10.0/3.0 * x - 8.0/3.0);
			g = (x < 2.0/5.0) * (5.0/2.0 * x) 
				+ (x >= 2.0/5.0 && x < 3.0/5.0)
				+ (x >= 3.0/5.0 && x < 4.0/5.0) * (-5.0 * x + 4.0);
			b = (x >= 3.0/5.0 && x < 4.0/5.0) * (5.0 * x - 3.0) 
				+ (x >= 4.0/5.0);
			break;
		}
	case GAMMA_COLORMAP_SPRING:
		{
			r = 1.0;
			g = x;
			b = 1.0 - g;
			break;
		}
	case GAMMA_COLORMAP_SUMMER:
		{
			r = x;
			g = 0.5 + (r / 2);
			b = 0.4;
			break;
		}
	case GAMMA_COLORMAP_WINTER:
		{
			r = 0.0;
			g = x;
			b = 1.0 - (g / 2.0);
			break;
		}
	case GAMMA_COLORMAP_GRAY:
	default:
		{
			r = x;
			g = x;
			b = x;
			break;
		}
	}

	m_colour.Set(r * 255, g * 255, b * 255);
}

bool GammaBlockTransMI::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_BRIGHTNESS:
		m_brightness = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_CONTRAST:
		m_contrast = *static_cast<double*>(value); break;
	case GAMMA_PARAM_IMG_GAMMA:
		m_gamma = *static_cast<double*>(value); break;
	case GAMMA_PARAM_COLORMAP:
		m_colormap = *static_cast<GammaColormap_e*>(value); break;
	case GAMMA_PARAM_COLORMAP_INVERT:
		m_bInvert = *static_cast<bool*>(value); break;
	default:
		return false;
	}

	return true;
}
