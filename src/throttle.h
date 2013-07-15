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

#define GAMMA_THROTTLE_SPEED_MAX (-1)

class GammaThrottle : public wxTimer
{

public:
	GammaThrottle(wxTimeSpan intTime);
	~GammaThrottle();

	inline void throttle();
	
	void setIntTime(wxTimeSpan intTime);
	wxTimeSpan getIntTime() const;
	void setSpeed(wxDouble speed);
	wxDouble getSpeed() const;

private:
	void Notify();

	bool m_bWait;
	wxMutex m_mutex;
	wxCondition m_condition;

	wxTimeSpan m_intTime;
	wxDouble m_speed;
};

#endif //_GAMMA_VIEW_BLOCK_TRANS_SM_H_
