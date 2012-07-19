/////////////////////////////////////////////////////////////////////////////
// Name:        block_trans.cpp
// Purpose:     driver for GammaUSB
// Author:      Mateusz Plociennik
// Created:     15/07/2012
/////////////////////////////////////////////////////////////////////////////

#include "block_trans.h"

void GammaBlockTrans::BlockRun()
{
	m_running = TRUE;
	Create();
	SetPriority(m_priority);
	Run();
}

void GammaBlockTrans::BlockStop()
{
	m_running = FALSE;
	Wait();
}

wxThread::ExitCode GammaBlockTrans::Entry()
{
	

	return 0;
}
