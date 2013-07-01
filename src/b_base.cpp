/**
 * @file	b_base.cpp
 * @brief	GammaPipe
 * @author	Mateusz Plociennik
 * @data	2013-06-30
 */

#include "block_base.h"

GammaPipeFrontEnd& GammaPipeFrontEnd::operator+=(GammaPipeFrontEnd& second)
{
	wxASSERT(dynamic_cast<GammaPipeBackEnd*>(this));

	*dynamic_cast<GammaPipeBackEnd*>(this) += second;

	return *this;
}
