/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include <vector>
#include <wx/thread.h>

#include "block_tr_sm.h"
#include "config.h"

GammaBlockTransSM::GammaBlockTransSM(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_throttle(wxTimeSpan::Milliseconds(250))
	, m_intEnabled(false)
	, m_eventMaxTrig(1)
	, m_eventSumTrig(0)
	, m_areaTrig(GAMMA_AREA_CFOV)
	, m_timeTrig(0)
	, m_intBeginTime(0)
	, m_intEndTime(0)
{
}

GammaBlockTransSM::~GammaBlockTransSM()
{
}

void GammaBlockTransSM::processData(GammaData* pData)
{
	wxMutexLocker locker(m_processDataMutex);

	GammaItems* pDataIn = dynamic_cast<GammaItems*>(pData);
	
	for(std::vector<GammaItem>::iterator it = pDataIn->items.begin();
		it != pDataIn->items.end(); it++)
	{
		switch((*it).type)
		{
		case GAMMA_ITEM_POINT:
			m_dataOut.matrix[POINT((*it).data.point.x, (*it).data.point.y)] += 1;
			if(POINT_INSIDE_FOV((*it).data.point.x, (*it).data.point.y))
			{
				if(m_dataOut.eventMax < m_dataOut.matrix[POINT((*it).data.point.x, (*it).data.point.y)])
				{
					m_dataOut.eventMax = m_dataOut.matrix[POINT((*it).data.point.x, (*it).data.point.y)];

					if(m_eventMaxTrig != 1 && m_eventMaxTrig < m_dataOut.eventMax)
					{
						pushData(&m_dataOut);
					}
				}
			}

			m_dataOut.eventSum += 1;
			if(m_eventSumTrig != 0 && m_eventSumTrig < m_dataOut.eventSum)
			{
				pushData(&m_dataOut);
			}
			break;
		case GAMMA_ITEM_TMARKER:
			m_markerTime = wxTimeSpan(0, 0, 0, (*it).data.time);
			if(m_dataOut.time <= m_markerTime && m_markerTime <= m_dataOut.time + wxTimeSpan::Second())
			{
				m_dataOut.time = m_markerTime;
				if(m_intEndTime + m_throttle.getIntTime() <= m_dataOut.time)
				{
					pushData(&m_dataOut);
					m_throttle.throttle();
				}
			}
			else
			{
				memset(m_dataOut.matrix, 0, sizeof(wxUint32) * 256 * 256);
				m_dataOut.eventMax = 1;
				m_dataOut.eventSum = 0;
				m_dataOut.time = m_markerTime;
				m_intBeginTime = m_markerTime;
				m_intEndTime = m_markerTime;
			}
			break;
		case GAMMA_ITEM_TRIGGER:
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

bool GammaBlockTransSM::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_INTEGRATE_TIME:
		m_throttle.setIntTime(*static_cast<wxTimeSpan*>(value)); break;
	case GAMMA_PARAM_IMG_SPEED:
		m_throttle.setSpeed(*static_cast<wxDouble*>(value)); break;
	case GAMMA_PARAM_IMG_INTEGRATE_ENABLED:
		m_intEnabled = *static_cast<bool*>(value); break;
	default:
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void GammaBlockTransSM::pushData(GammaData* pDataOut)
{
	m_dataOut.span = m_dataOut.time - m_intBeginTime;
	GammaPipeSegment::pushData(pDataOut);
	if(!m_intEnabled || m_dataOut.time < m_intEndTime || 
		m_intEndTime + 2 * m_throttle.getIntTime() <= m_dataOut.time)
	{
		memset(m_dataOut.matrix, 0, sizeof(wxUint32) * 256 * 256);
		m_dataOut.eventMax = 1;
		m_dataOut.eventSum = 0;
		m_intBeginTime = m_dataOut.time;
	}
	m_intEndTime = m_dataOut.time;
}

////////////////////////////////////////////////////////////////////////////////

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
	m_condition.Broadcast();
}

void GammaThrottle::Notify()
{
	wxMutexLocker locker(m_mutex);
	if(-1 != m_speed)
	{
		m_bWait = false;
		m_condition.Broadcast();
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
