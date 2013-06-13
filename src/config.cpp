/**
 * @file	config.cpp
 * @brief	Config options
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "data_types.h"
#include "config.h"

GammaConfig::GammaConfig()
	: m_fieldOfView(wxRealPoint(127.5, 127.5), 127.5, GAMMA_AREA_CFOV)
{
}

GammaConfig::~GammaConfig()
{
}

GammaFieldOfView* GammaConfig::getFieldOfView()
{
	return &m_fieldOfView;
}


