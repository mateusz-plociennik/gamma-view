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

class GammaBlockTransSM;

class GammaThrottle : public wxTimer
{
	friend class GammaBlockTransSM;

protected:
	GammaThrottle(wxTimeSpan intTime);
	~GammaThrottle();
	void Notify();

	inline void throttle();
	wxTimeSpan getIntTime() const;
	void setIntTime(wxTimeSpan intTime);
	void setSpeed(wxDouble speed);

	bool m_bWait;
	wxMutex m_mutex;
	wxCondition m_condition;

private:
	wxTimeSpan m_intTime;
	wxDouble m_speed;
};

class GammaBlockTransSM : public GammaPipeSegment
{
public:
	GammaBlockTransSM(GammaManager* pManager);
	~GammaBlockTransSM();

	void processData(GammaData* pData);
	inline void GammaBlockTransSM::pushData(GammaData* pDataOut);
	wxInt32 setParam(GammaParam_e param, void* value);

private:
	wxUint32 m_intgTime;
	bool m_intEnabled;

	GammaMatrix m_dataOut;
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

	GammaThrottle m_throttle;
};


#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
