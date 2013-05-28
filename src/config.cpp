/**
 * @file	config.cpp
 * @brief	Config options
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "data_types.h"
#include "config.h"

GammaConfig::GammaConfig()
{
	m_fieldOfView.center = wxRealPoint(127.5, 127.5);
	m_fieldOfView.radius = 127.5;
	m_fieldOfView.area = GAMMA_AREA_CFOV; //GAMMA_AREA_FOV;
}

GammaConfig::~GammaConfig()
{

}

GammaFieldOfView* GammaConfig::getFieldOfView()
{
	return &m_fieldOfView;
}
