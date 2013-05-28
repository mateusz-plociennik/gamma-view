/**
 * @file	block_mgmt.h
 * @brief	blocks manager
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_MGMT_H_
#define _GAMMA_VIEW_BLOCK_MGMT_H_


#include "data_types.h"

#include <list>
#include "main.h"
#include <wx/thread.h>

//namespace LogicTier

//#include "block_base.h" Breaking cyclic dependency
class GammaPipeSegment;

//#include "frame_view.h" Breaking cyclic dependency
class GammaFrame;

#include "config.h"

class GammaManager
{

public:
	GammaManager(GammaFrame* pFrame)
		: m_pFrame(pFrame)
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
	void setMode(GammaMode_e mode);

	/**
	 * Sets parameters in low layer: GammaBlocks
	 */
	int DataTierSetParam(GammaParam_e param, void* value);

	/**
	 * Sets parameters in high layer: wxFrame
	 */
	bool PresentationTierSetParam(GammaParam_e param, void* value);

	GammaConfig* getConfig();
	
private:
	wxMutex m_mgrMutex;
	GammaFrame* m_pFrame;
	std::list<GammaPipeSegment*> m_blockList;

	GammaConfig m_config;
	
};

#endif //_GAMMA_VIEW_BLOCK_MGMT_H_