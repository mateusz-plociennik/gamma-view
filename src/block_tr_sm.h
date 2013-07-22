/**
 * @file	block_tr_sm.h
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#ifndef _GAMMA_VIEW_BLOCK_TRANS_SM_H_
#define _GAMMA_VIEW_BLOCK_TRANS_SM_H_

#include "block_base.h"

#include <wx/timer.h>
#include <wx/thread.h>


class GammaBlockTransSM : public GammaPipeSegment
{
public:
	GammaBlockTransSM(GammaManager* pManager);
	~GammaBlockTransSM();

	void processData(wxSharedPtr<GammaData> pData);
	inline void pushData();
	wxInt32 setParam(GammaParam_e param, void* value);
	void onIntegrateMenu(wxCommandEvent& event);

private:
	wxTimeSpan m_intTime;
	bool m_intEnabled;

	//GammaMatrix* m_pDataOut;
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
};


#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
