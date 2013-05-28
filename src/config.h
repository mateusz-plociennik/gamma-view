/**
 * @file	config.h
 * @brief	Config options
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#ifndef _GAMMA_VIEW_CONFIG_H_
#define _GAMMA_VIEW_CONFIG_H_

#include "data_types.h"

class GammaConfig
{
public:
	GammaConfig();
	~GammaConfig();

	GammaFieldOfView* getFieldOfView();

private:
	GammaFieldOfView m_fieldOfView;
	
};

#endif //_GAMMA_VIEW_CONFIG_H_
