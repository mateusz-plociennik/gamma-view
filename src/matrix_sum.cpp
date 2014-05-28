/**
 * @file	block_tr_sm.cpp
 * @brief	translate points to matrix
 * @author	Mateusz Plociennik
 * @data	2012-07-30
 */

#include <vector>

#include <wx/thread.h>

#include "matrix_sum.h"
#include "config.h"

GammaMatrixSum::GammaMatrixSum(GammaManager* pManager) 
	: GammaPipeSegment(pManager)
	, m_intEnabled(false)
	, m_eventMaxTrig(0)
	, m_eventSumTrig(0)
	, m_areaTrig(GAMMA_AREA_CFOV)
	, m_timeTrig(0)
	, m_intBeginTime(0)
	, m_intEndTime(0)
	, m_gateTrig(0)
	, m_gateCounter(0)
	, m_glowEnabled(true)
	, m_glowTime(wxTimeSpan::Milliseconds(4000))
	, m_sDataOut(new GammaMatrix())
	//, m_pDataOut(new GammaMatrix())
	//, m_pDataOutShared(m_pDataOut)
{
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GammaMatrixSum::onGlowMenu, this, 
		ID_MENU_GLOW_TIME_1_1000, ID_MENU_GLOW_ENABLED);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GammaMatrixSum::onIntegrateMenu, this, 
		ID_MENU_INTEGRATE_ENABLED);
}

GammaMatrixSum::~GammaMatrixSum()
{
	Unbind(wxEVT_COMMAND_MENU_SELECTED, &GammaMatrixSum::onGlowMenu, this, 
		ID_MENU_GLOW_TIME_1_1000, ID_MENU_GLOW_ENABLED);
	Unbind(wxEVT_COMMAND_MENU_SELECTED, &GammaMatrixSum::onIntegrateMenu, this, 
		ID_MENU_INTEGRATE_ENABLED);
}

void GammaMatrixSum::processData(wxSharedPtr<GammaData> sDataIn)
{
	wxMutexLocker locker(m_processDataMutex);

	wxASSERT(GAMMA_DATA_TYPE_MATRIX == sDataIn->type);
	GammaMatrix* pDataIn = dynamic_cast<GammaMatrix*>(sDataIn.get());

	//wxSharedPtr<GammaData> sDataOut(new GammaMatrix());
	GammaMatrix* pDataOut = dynamic_cast<GammaMatrix*>(m_sDataOut.get());

	//wxTimeSpan intTimeSum = pDataOut->intTime;

	wxInt32 mult = 0;
	if(m_intEnabled)
	{
		mult = GAMMA_EVENT_UNIT;
		pDataOut->intTime += pDataIn->intTime;
	}
	else 
	{
		pDataOut->intTime = pDataIn->intTime;
		pDataOut->acqTime = pDataIn->acqTime;
		pDataOut->eventSum = pDataIn->eventSum;
		pDataOut->eventSumFov = pDataIn->eventSumFov;

		if(m_glowEnabled)
		{
			mult = (m_glowTime.GetValue().GetValue() - pDataIn->intTime.GetValue().GetValue())
				* GAMMA_EVENT_UNIT / m_glowTime.GetValue().GetValue();
		}
	}

	if(m_intEnabled)
	{
		for(wxInt32 i = 0; i < 256 * 256; i += 4)
		{
			pDataOut->matrix[i+0] += pDataIn->matrix[i+0];
			pDataOut->matrix[i+1] += pDataIn->matrix[i+1];
			pDataOut->matrix[i+2] += pDataIn->matrix[i+2];
			pDataOut->matrix[i+3] += pDataIn->matrix[i+3];
		}
	}
	else
	{
		for(wxInt32 i = 0; i < 256 * 256; i += 4)
		{
			pDataOut->matrix[i+0] = std::max(pDataIn->matrix[i+0], mult * pDataOut->matrix[i+0] / GAMMA_EVENT_UNIT);
			pDataOut->matrix[i+1] = std::max(pDataIn->matrix[i+1], mult * pDataOut->matrix[i+1] / GAMMA_EVENT_UNIT);
			pDataOut->matrix[i+2] = std::max(pDataIn->matrix[i+2], mult * pDataOut->matrix[i+2] / GAMMA_EVENT_UNIT);
			pDataOut->matrix[i+3] = std::max(pDataIn->matrix[i+3], mult * pDataOut->matrix[i+3] / GAMMA_EVENT_UNIT);
		}
	}

	if(m_eventMaxTrig && m_eventMaxTrig < pDataOut->eventMax())
	{
		pDataOut->trig = GAMMA_TRIG_MAX;
	}
	if(m_eventSumTrig && m_eventSumTrig < pDataOut->eventSum)
	{
		pDataOut->trig = GAMMA_TRIG_SUM;
	}
	if(!m_timeTrig.IsNull() && m_timeTrig < pDataOut->acqTime)
	{
		pDataOut->trig = GAMMA_TRIG_TIME;
	}

	if(pDataOut->trig != GAMMA_TRIG_NONE)
	{
		wxThreadEvent event(wxEVT_THREAD, ID_EVENT_TRIG);
		event.SetPayload< wxSharedPtr<GammaData> >(m_sDataOut);
		wxTheApp->QueueEvent(event.Clone());
		pDataOut->trig = GAMMA_TRIG_NONE;
	}

	GammaPipeSegment::pushData(m_sDataOut);
}

////////////////////////////////////////////////////////////////////////////////

wxInt32 GammaMatrixSum::setParam(GammaParam_e param, void* value)
{
	wxMutexLocker locker(m_processDataMutex);

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


void GammaMatrixSum::pushData()
{
	/*
	//wxUnusedVar(pDataOut);
	m_pDataOut->span = m_pDataOut->time - m_intBeginTime;
	GammaPipeSegment::pushData(m_pDataOutShared);

//	if(!m_intEnabled || m_pDataOut->time < m_intEndTime || 
//		m_intEndTime + 2 * m_throttle.getIntTime() <= m_pDataOut->time)
	{
		memset(m_pDataOut->matrix, 0, sizeof(wxUint32) * 256 * 256);
		m_pDataOut->eventMax = 1;
		m_pDataOut->eventSum = 0;
		m_pDataOut->eventSumFov = 0;
		m_intBeginTime = m_pDataOut->time;
	}
	m_intEndTime = m_pDataOut->time;
	m_pDataOut->trig = GAMMA_TRIG_NONE;
	*/
}

void GammaMatrixSum::onGlowMenu(wxCommandEvent& event)
{
	wxMutexLocker locker(m_processDataMutex);

	switch(event.GetId())
	{
	case ID_MENU_GLOW_TIME_1_1000:
		m_glowTime = wxTimeSpan::Milliseconds(1); break;
	case ID_MENU_GLOW_TIME_1_500:
		m_glowTime = wxTimeSpan::Milliseconds(2); break;
	case ID_MENU_GLOW_TIME_1_250:
		m_glowTime = wxTimeSpan::Milliseconds(4); break;
	case ID_MENU_GLOW_TIME_1_125:
		m_glowTime = wxTimeSpan::Milliseconds(8); break;
	case ID_MENU_GLOW_TIME_1_60:
		m_glowTime = wxTimeSpan::Milliseconds(16); break;
	case ID_MENU_GLOW_TIME_1_30:
		m_glowTime = wxTimeSpan::Milliseconds(33); break;
	case ID_MENU_GLOW_TIME_1_15:
		m_glowTime = wxTimeSpan::Milliseconds(66); break;
	case ID_MENU_GLOW_TIME_1_8:
		m_glowTime = wxTimeSpan::Milliseconds(125); break;
	case ID_MENU_GLOW_TIME_1_4:
		m_glowTime = wxTimeSpan::Milliseconds(250); break;
	case ID_MENU_GLOW_TIME_1_2:
		m_glowTime = wxTimeSpan::Milliseconds(500); break;
	case ID_MENU_GLOW_TIME_1:
		m_glowTime = wxTimeSpan::Milliseconds(1000); break;
	case ID_MENU_GLOW_TIME_2:
		m_glowTime = wxTimeSpan::Milliseconds(2000); break;
	case ID_MENU_GLOW_TIME_4:
		m_glowTime = wxTimeSpan::Milliseconds(4000); break;
	case ID_MENU_GLOW_TIME_8:
		m_glowTime = wxTimeSpan::Milliseconds(8000); break;
	case ID_MENU_GLOW_TIME_16:
		m_glowTime = wxTimeSpan::Milliseconds(16000); break;
	case ID_MENU_GLOW_TIME_32:
		m_glowTime = wxTimeSpan::Milliseconds(32000); break;
	case ID_MENU_GLOW_ENABLED:
		m_glowEnabled = event.IsChecked(); break;
	default:
		break;
	}
}

void GammaMatrixSum::onIntegrateMenu(wxCommandEvent& event)
{
	wxMutexLocker locker(m_processDataMutex);

	switch(event.GetId())
	{
	case ID_MENU_INTEGRATE_ENABLED:
		m_intEnabled = event.IsChecked(); 
		m_sDataOut.reset(new GammaMatrix());
		break;
	default:
		break;
	}
}
