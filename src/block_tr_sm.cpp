/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include <vector>
#include <wx/thread.h>
#include <wx/event.h>

#include "block_tr_sm.h"
#include "config.h"

GammaBlockTransSM::GammaBlockTransSM(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_intEnabled(false)
	, m_eventMaxTrig(1)
	, m_eventSumTrig(0)
	, m_areaTrig(GAMMA_AREA_CFOV)
	, m_timeTrig(0)
	, m_intTime(0, 0, 0, 250)
	, m_intBeginTime(0)
	, m_intEndTime(0)
	, m_gateTrig(0)
	, m_gateCounter(0)
	, m_sDataOut(new GammaMatrix())
{
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GammaBlockTransSM::onIntegrateMenu, this, 
		ID_MENU_INTEGRATE_TIME_1_1000, ID_MENU_INTEGRATE_ENABLED);
}

GammaBlockTransSM::~GammaBlockTransSM()
{
	Unbind(wxEVT_COMMAND_MENU_SELECTED, &GammaBlockTransSM::onIntegrateMenu, this, 
		ID_MENU_INTEGRATE_TIME_1_1000, ID_MENU_INTEGRATE_ENABLED);
}

void GammaBlockTransSM::processData(wxSharedPtr<GammaData> sDataIn)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_ITEMS == sDataIn->type);
	GammaItems* pDataIn = dynamic_cast<GammaItems*>(sDataIn.get());
	GammaMatrix* pDataOut = dynamic_cast<GammaMatrix*>(m_sDataOut.get());
	
	for(std::vector<GammaItem>::iterator it = pDataIn->items.begin();
		it != pDataIn->items.end(); it++)
	{
		switch((*it).type)
		{
		case GAMMA_ITEM_TYPE_POINT:
			pDataOut->matrix[POINT((*it).data.point.x, (*it).data.point.y)] += GAMMA_EVENT_UNIT;
			pDataOut->eventSum += GAMMA_EVENT_UNIT;

			if(POINT_INSIDE_FOV(POINT((*it).data.point.x, (*it).data.point.y)))
			{
				pDataOut->eventSumFov += GAMMA_EVENT_UNIT;
			}
			break;
		case GAMMA_ITEM_TYPE_TMARKER:
			m_markerTime = wxTimeSpan(0, 0, 0, (*it).data.time);
			//wxLogStatus("m_markerTime = %"wxLongLongFmtSpec"d", m_markerTime.GetValue().GetValue());
			if(m_intEndTime <= m_markerTime)
			{
				pDataOut->acqTime = m_markerTime;
				if(m_intEndTime + m_intTime <= pDataOut->acqTime)
				{
					m_intEndTime = pDataOut->acqTime;

					pDataOut->trig = GAMMA_TRIG_TIME;
					pDataOut->intTime = pDataOut->acqTime - m_intBeginTime;
					GammaPipeSegment::pushData(m_sDataOut);
					m_sDataOut.reset(new GammaMatrix());
					pDataOut = dynamic_cast<GammaMatrix*>(m_sDataOut.get());
					m_intBeginTime = m_markerTime;
				}

				
			}
			break;
		case GAMMA_ITEM_TYPE_TRIGGER:
			m_gateCounter++;
			//if(m_gateTrig != 0 && m_gateTrig <= m_gateCounter)
			if(m_gateCounter == 1000)
			{
				wxThreadEvent event(wxEVT_THREAD, ID_GATE_TRIGGER);
				event.SetPayload<wxTimeSpan>(m_markerTime);
				wxTheApp->QueueEvent(event.Clone());

				pDataOut->trig = GAMMA_TRIG_GATE;
				//pushData();
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
		//m_throttle.setIntTime(*static_cast<wxTimeSpan*>(value)); break;
	case GAMMA_PARAM_IMG_SPEED:
		//m_throttle.setSpeed(*static_cast<wxDouble*>(value)); break;
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

void GammaBlockTransSM::pushData()
{
	/*
	//wxUnusedVar(pDataOut);
	pDataOut->span = pDataOut->time - m_intBeginTime;
//	GammaPipeSegment::pushData(pDataOutShared);

//	if(!m_intEnabled || pDataOut->time < m_intEndTime || 
//		m_intEndTime + 2 * m_throttle.getIntTime() <= pDataOut->time)
	{
		memset(pDataOut->matrix, 0, sizeof(wxUint32) * 256 * 256);
		pDataOut->eventMax = 1;
		pDataOut->eventSum = 0;
		pDataOut->eventSumFov = 0;
		m_intBeginTime = pDataOut->time;
	}
	m_intEndTime = pDataOut->time;
	pDataOut->trig = GAMMA_TRIG_NONE;
	*/
}

void GammaBlockTransSM::onIntegrateMenu(wxCommandEvent& event)
{
	switch ( event.GetId() )
	{
	case ID_MENU_INTEGRATE_TIME_1_1000:
		m_intTime = wxTimeSpan::Milliseconds(1); break;
	case ID_MENU_INTEGRATE_TIME_1_500:
		m_intTime = wxTimeSpan::Milliseconds(2); break;
	case ID_MENU_INTEGRATE_TIME_1_250:
		m_intTime = wxTimeSpan::Milliseconds(4); break;
	case ID_MENU_INTEGRATE_TIME_1_125:
		m_intTime = wxTimeSpan::Milliseconds(8); break;
	case ID_MENU_INTEGRATE_TIME_1_60:
		m_intTime = wxTimeSpan::Milliseconds(17); break;
	case ID_MENU_INTEGRATE_TIME_1_30:
		m_intTime = wxTimeSpan::Milliseconds(33); break;
	case ID_MENU_INTEGRATE_TIME_1_15:
		m_intTime = wxTimeSpan::Milliseconds(67); break;
	case ID_MENU_INTEGRATE_TIME_1_8:
		m_intTime = wxTimeSpan::Milliseconds(125); break;
	case ID_MENU_INTEGRATE_TIME_1_4:
		m_intTime = wxTimeSpan::Milliseconds(250); break;
	case ID_MENU_INTEGRATE_TIME_1_2:
		m_intTime = wxTimeSpan::Milliseconds(500); break;
	case ID_MENU_INTEGRATE_TIME_1:
		m_intTime = wxTimeSpan::Milliseconds(1000); break;
	case ID_MENU_INTEGRATE_TIME_2:
		m_intTime = wxTimeSpan::Milliseconds(2000); break;
	case ID_MENU_INTEGRATE_TIME_4:
		m_intTime = wxTimeSpan::Milliseconds(4000); break;
	case ID_MENU_INTEGRATE_TIME_8:
		m_intTime = wxTimeSpan::Milliseconds(8000); break;
	case ID_MENU_INTEGRATE_TIME_16:
		m_intTime = wxTimeSpan::Milliseconds(16000); break;
	case ID_MENU_INTEGRATE_TIME_32:
		m_intTime = wxTimeSpan::Milliseconds(32000); break;
	default:
		break;
	}
}
