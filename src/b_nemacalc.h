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

	void processData(wxSharedPtr<GammaData> pData);
	//wxInt32 setParam((GammaParam_e param, void* value);

private:
	void floodFill(wxPoint start, wxUint32 colour);
	void marginalRemove();
	void convolutionFilter(GammaMatrix* pDataOut);
	double getIntgUniformity();
	double getDiffUniformity(GammaDirection_e);

	GammaMatrix* m_pDataIn;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_
