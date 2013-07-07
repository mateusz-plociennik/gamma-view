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
	, m_gateTrig(0)
	, m_gateCounter(0)
	, m_pDataOut(new GammaMatrix())
	, m_pDataOutShared(m_pDataOut)
{
}

GammaBlockTransSM::~GammaBlockTransSM()
{
}

void GammaBlockTransSM::processData(wxSharedPtr<GammaData> pData)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_ITEMS == pData->type);
	GammaItems* pDataIn = dynamic_cast<GammaItems*>(pData.get());
	
	for(std::vector<GammaItem>::iterator it = pDataIn->items.begin();
		it != pDataIn->items.end(); it++)
	{
		switch((*it).type)
		{
		case GAMMA_ITEM_TYPE_POINT:
			m_pDataOut->matrix[POINT((*it).data.point.x, (*it).data.point.y)]++;
			m_pDataOut->eventSum++;

			if(POINT_INSIDE_FOV(POINT((*it).data.point.x, (*it).data.point.y)))
			{
				if(m_pDataOut->eventMax < m_pDataOut->matrix[POINT((*it).data.point.x, (*it).data.point.y)])
				{
					m_pDataOut->eventMax = m_pDataOut->matrix[POINT((*it).data.point.x, (*it).data.point.y)];

					if(m_eventMaxTrig != 1 && m_eventMaxTrig <= m_pDataOut->eventMax)
					{
						m_pDataOut->trig = GAMMA_TRIG_MAX;
						pushData(m_pDataOut);
					}
				}

				m_pDataOut->eventSumIn++;
				if(m_eventSumTrig != 0 && m_eventSumTrig <= m_pDataOut->eventSumIn)
				{
					m_pDataOut->trig = GAMMA_TRIG_SUM;
					pushData(m_pDataOut);
				}
			}
			break;
		case GAMMA_ITEM_TYPE_TMARKER:
			m_markerTime = wxTimeSpan(0, 0, 0, (*it).data.time);
			//wxLogStatus("m_markerTime = %"wxLongLongFmtSpec"d", m_markerTime.GetValue().GetValue());
			if(m_pDataOut->time <= m_markerTime && m_markerTime <= m_pDataOut->time + wxTimeSpan::Second())
			{
				m_pDataOut->time = m_markerTime;
				if(m_intEndTime + m_throttle.getIntTime() <= m_pDataOut->time)
				{
					pushData(m_pDataOut);
					m_throttle.throttle();
				}

				if(m_timeTrig != 0 && m_timeTrig <= m_markerTime)
				{
					m_pDataOut->trig = GAMMA_TRIG_TIME;
					pushData(m_pDataOut);
				}
			}
			else
			{
				memset(m_pDataOut->matrix, 0, sizeof(wxUint32) * 256 * 256);
				m_pDataOut->eventMax = 1;
				m_pDataOut->eventSum = 0;
				m_pDataOut->eventSumIn = 0;
				//m_pDataOut->bTrig = false;
				m_pDataOut->time = m_markerTime;
				m_intBeginTime = m_markerTime;
				m_intEndTime = m_markerTime;
			}
			break;
		case GAMMA_ITEM_TYPE_TRIGGER:
			m_gateCounter++;
			if(m_gateTrig != 0 && m_gateTrig <= m_gateCounter)
			{
				m_pDataOut->trig = GAMMA_TRIG_GATE;
				pushData(m_pDataOut);
				m_gateCounter = 0;
			}
			break;
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

wxInt32 GammaBlockTransSM::setParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_IMG_INTEGRATE_TIME:
		m_throttle.setIntTime(*static_cast<wxTimeSpan*>(value)); break;
	case GAMMA_PARAM_IMG_SPEED:
		m_throttle.setSpeed(*static_cast<wxDouble*>(value)); break;
	case GAMMA_PARAM_IMG_INTEGRATE_ENABLED:
		m_intEnabled = *static_cast<bool*>(value); break;
	case GAMMA_PARAM_TRIG_AVG:
		m_eventAvgTrig = *static_cast<wxUint32*>(value); break;
	case GAMMA_PARAM_TRIG_MAX:
		m_eventMaxTrig = *static_cast<wxUint32*>(value); break;
	case GAMMA_PARAM_TRIG_SUM:
		m_eventSumTrig = *static_cast<wxUint64*>(value); break;
	case GAMMA_PARAM_TRIG_TIME:
		m_timeTrig = *static_cast<wxTimeSpan*>(value); break;
	case GAMMA_PARAM_TRIG_GATE:
		m_gateTrig = *static_cast<wxUint32*>(value); break;
	default:
		return false;
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void GammaBlockTransSM::pushData(GammaData* pDataOut)
{
	m_pDataOut->span = m_pDataOut->time - m_intBeginTime;
	GammaPipeSegment::pushData(m_pDataOutShared);

	if(!m_intEnabled || m_pDataOut->time < m_intEndTime || 
		m_intEndTime + 2 * m_throttle.getIntTime() <= m_pDataOut->time)
	{
		memset(m_pDataOut->matrix, 0, sizeof(wxUint32) * 256 * 256);
		m_pDataOut->eventMax = 1;
		m_pDataOut->eventSum = 0;
		m_pDataOut->eventSumIn = 0;
		m_intBeginTime = m_pDataOut->time;
	}
	m_intEndTime = m_pDataOut->time;
	m_pDataOut->trig = GAMMA_TRIG_NONE;
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
