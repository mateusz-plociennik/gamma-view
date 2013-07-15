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
	, m_eventMaxTrig(1)
	, m_eventSumTrig(0)
	, m_areaTrig(GAMMA_AREA_CFOV)
	, m_timeTrig(0)
	, m_intBeginTime(0)
	, m_intEndTime(0)
	, m_gateTrig(0)
	, m_gateCounter(0)
	, m_glowEnabled(true)
	, m_glowTime(wxTimeSpan::Milliseconds(4000))
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

void GammaMatrixSum::processData(wxSharedPtr<GammaData> pData)
{
	wxMutexLocker locker(m_processDataMutex);
	//GammaMatrix* pDataOut = new GammaMatrix();

	wxASSERT(GAMMA_DATA_TYPE_MATRIX == pData->type);
	GammaMatrix* pDataOut = dynamic_cast<GammaMatrix*>(pData.get());

	wxTimeSpan intTimeSum = pDataOut->intTime;
	for(std::deque<wxSharedPtr<GammaData>>::iterator iMatrix = m_dataDeque.begin();
		iMatrix != m_dataDeque.end(); iMatrix++)
	{
		GammaMatrix* pDataIn = dynamic_cast<GammaMatrix*>((*iMatrix).get());

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
			wxInt32 mult = 0;
			if(m_glowEnabled)
			{
				mult = (m_glowTime.GetValue().GetValue() - intTimeSum.GetValue().GetValue())
					* GAMMA_EVENT_UNIT / m_glowTime.GetValue().GetValue();
			}
			else
			{
				mult = 0;
			}
			if(mult <= GAMMA_EVENT_UNIT / 8)
			{
				m_dataDeque.erase(iMatrix, m_dataDeque.end());
				break;
			}

			for(wxInt32 i = 0; i < 256 * 256; i += 4)
			{
				pDataOut->matrix[i+0] = std::max(pDataOut->matrix[i+0], mult * pDataIn->matrix[i+0] / GAMMA_EVENT_UNIT);
				pDataOut->matrix[i+1] = std::max(pDataOut->matrix[i+1], mult * pDataIn->matrix[i+1] / GAMMA_EVENT_UNIT);
				pDataOut->matrix[i+2] = std::max(pDataOut->matrix[i+2], mult * pDataIn->matrix[i+2] / GAMMA_EVENT_UNIT);
				pDataOut->matrix[i+3] = std::max(pDataOut->matrix[i+3], mult * pDataIn->matrix[i+3] / GAMMA_EVENT_UNIT);
			}
		}

		intTimeSum += pDataIn->intTime;
	}
	
	pDataOut->intTime = intTimeSum;
	if(!m_dataDeque.empty())
	{
		pDataOut->acqTime = dynamic_cast<GammaMatrix*>(m_dataDeque.back().get())->acqTime;
	}

	if(m_intEnabled)
	{
		m_dataDeque.clear();
	}
	m_dataDeque.push_front(pData);
	GammaPipeSegment::pushData(pData);
}

////////////////////////////////////////////////////////////////////////////////

wxInt32 GammaMatrixSum::setParam(GammaParam_e param, void* value)
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


void GammaMatrixSum::pushData()
{
	/*
	//UNREFERENCED_PARAMETER(pDataOut);
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
	switch(event.GetId())
	{
	case ID_MENU_INTEGRATE_ENABLED:
		m_intEnabled = event.IsChecked(); break;
	default:
		break;
	}
}
