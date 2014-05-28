/**
 * @file	block_tr_sm.h
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_MATRIX_SUM_H_
#define _GAMMA_VIEW_BLOCK_MATRIX_SUM_H_

#include "block_base.h"

#include <wx/timer.h>
#include <wx/thread.h>
#include <deque>

class GammaMatrixSum : public GammaPipeSegment
{
public:
	GammaMatrixSum(GammaManager* pManager);
	~GammaMatrixSum();

	void processData(wxSharedPtr<GammaData> pData);
	inline void pushData();
	wxInt32 setParam(GammaParam_e param, void* value);

	void onGlowMenu(wxCommandEvent& event);
	void onIntegrateMenu(wxCommandEvent& event);

private:
	//wxUint32 m_intTime;
	bool m_intEnabled;
	bool m_glowEnabled;

	wxSharedPtr<GammaData> m_sDataOut;
	wxTimeSpan m_markerTime;

	wxTimeSpan m_intBeginTime;
	wxTimeSpan m_intEndTime;
	//wxUint32* t_matrix;
	//wxUint32 t_max;
	//wxUint64 t_sum;
	wxUint32 m_eventAvgTrig;
	wxUint32 m_eventMaxTrig;
	wxUint64 m_eventSumTrig;
	GammaArea_e m_areaTrig;
	wxTimeSpan m_timeTrig;
	wxUint32 m_gateCounter;
	wxUint32 m_gateTrig;
	wxTimeSpan m_glowTime;

	std::deque< wxSharedPtr<GammaData> > m_dataDeque;
};


#endif //_GAMMA_VIEW_BLOCK_MATRIX_SUM_H_
