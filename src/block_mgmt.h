/**
 * @file	block_mgmt.h
 * @brief	blocks manager
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_BLOCK_FREAD_H_
#define _GAMMA_VIEW_BLOCK_FREAD_H_

#include "data_types.h"
#include "block_base.h"
#include <list>

class GammaBlockManager
{

public:
	/**
	 * Return instance to singleton
	 */
	static GammaBlockManager& getInstance()
	{
		static GammaBlockManager instance;
        return instance;
	}

	/**
	 *
	 */
	void SetMode(GammaBlockMode_e mode);
	//void BlocksRun();

private:
	GammaBlockManager() 
	{
	}

	GammaBlockManager(const GammaBlockManager&);
	GammaBlockManager& operator=(const GammaBlockManager&);

	std::list<GammaBlockBase*> m_blockList;
};

#endif //_GAMMA_VIEW_BLOCK_FREAD_H_