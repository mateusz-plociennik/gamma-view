/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include "throttle.h"

#include <wx/thread.h>

GammaThrottle::GammaThrottle(wxTimeSpan intTime)
	: m_bWait(true)
	, m_condition(m_mutex)
	, m_intTime(intTime)
	, m_speed(-1)
{
	if(-1 != m_speed)
	{
		Start(m_intTime.GetMilliseconds().GetValue() / m_speed);
	}
}

GammaThrottle::~GammaThrottle()
{
	Stop();

	wxMutexLocker locker(m_mutex);
	m_speed = -1;
	m_bWait = false;
	m_condition.Signal();
}

void GammaThrottle::Notify()
{
	wxMutexLocker locker(m_mutex);
	if(-1 != m_speed)
	{
		m_bWait = false;
		m_condition.Signal();
	}
}

void GammaThrottle::throttle()
{
	wxMutexLocker locker(m_mutex);
	if(-1 != m_speed)
	{
		while(m_bWait)
		{	
			m_condition.Wait();
		}
		m_bWait = true;
	}
}

wxTimeSpan GammaThrottle::getIntTime() const
{
	return m_intTime;
}

void GammaThrottle::setIntTime(wxTimeSpan intTime)
{
	wxMutexLocker locker(m_mutex);
	m_intTime = intTime;
	Stop();
	if(-1 != m_speed)
	{
		Start(m_intTime.GetMilliseconds().GetValue() / m_speed);
	}
}

void GammaThrottle::setSpeed(wxDouble speed)
{
	wxMutexLocker locker(m_mutex);
	m_speed = speed;
	Stop();
	if(-1 != m_speed)
	{
		Start(m_intTime.GetMilliseconds().GetValue() / m_speed);
	}
}
