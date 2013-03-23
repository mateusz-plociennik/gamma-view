/**
 * @file	block_fread.h
 * @brief	saving data to a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_NEMACALC_H_
#define _GAMMA_VIEW_BLOCK_FREAD_H_

#include "block_base.h"
#include "block_data.h"

#include <wx/gdicmn.h>

class GammaNemaCalc : 
	public GammaBlockBase
{
public:
	GammaNemaCalc(GammaManager* pManager);
	~GammaNemaCalc();

	bool SetParam(GammaParam_e param, void* value);

protected:
	wxThread::ExitCode Entry();

private:
	void FloodFill(wxPoint start, uint32_t color);
	void MarginalRemove();
	void ConvolutionFilter(GammaDataMatrix* pDataOut);
	double GammaNemaCalc::GetIntgUniformity(GammaArea_e area);
	double GammaNemaCalc::GetDiffUniformity(GammaArea_e area, GammaDirection_e);

	GammaDataMatrix* m_pDataIn;

	GammaFieldOfView m_fieldOfView;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
