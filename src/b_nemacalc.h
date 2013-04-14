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
	public GammaPipeSegment
{
public:
	GammaNemaCalc(GammaManager* pManager);
	~GammaNemaCalc();

	void processData(GammaDataBase* pData);
	bool setParam(GammaParam_e param, void* value);

private:
	void floodFill(wxPoint start, uint32_t color);
	void marginalRemove();
	void convolutionFilter(GammaDataMatrix* pDataOut);
	double getIntgUniformity(GammaArea_e area);
	double getDiffUniformity(GammaArea_e area, GammaDirection_e);

	GammaDataMatrix* m_pDataIn;

	GammaFieldOfView m_fieldOfView;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
