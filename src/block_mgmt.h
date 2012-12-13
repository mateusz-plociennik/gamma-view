/**
 * @file	block_mgmt.h
 * @brief	blocks manager
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_MGMT_H_
#define _GAMMA_VIEW_BLOCK_MGMT_H_

#include "data_types.h"
#include "block_base.h"
#include <list>
#include "main.h"
#include <wx/thread.h>

//namespace LogicTier

class GammaManager
{

public:
	GammaManager(MyFrame* pFrame) :
			m_pFrame(pFrame)
	{
		//
	}
	
	~GammaManager()
	{
		//
	}

	/**
	 * Set mode for blocks under manager
	 */
	void SetMode(GammaMode_e mode);
	
	/**
	 * Sets parameters in low layer: GammaBlocks
	 */
	bool SetDataTierParam(GammaParam_e name, void* value)
	{
		for ( std::list<GammaBlockBase*>::iterator iBlock = m_blockList.begin();
			iBlock != m_blockList.end(); iBlock++ )
		{
			(*iBlock)->SetParam(name, value);
		}
	}
	
	/**
	 * Sets parameters in high layer: wxFrame
	 */
	bool SetPresentationTierParam(GammaParam_e name, void* value)
	{
		m_pFrame->SetParam(name, value);
	}
	
private:
	wxMutex m_mgrMutex;
	MyFrame* m_pFrame;
	std::list<GammaBlockBase*> m_blockList;
	
};

#endif //_GAMMA_VIEW_BLOCK_MGMT_H_