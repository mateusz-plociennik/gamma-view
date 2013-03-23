/**
 * @file	block_usb_fake.cpp
 * @brief	Fake device for testing purposes.
 * @author	Mateusz Plociennik
 * @data	2012-07-29
 */

#include "block_usb_fake.h"

GammaBlockUSBFake::GammaBlockUSBFake(GammaManager* pManager) 
		:
		GammaBlockBase(pManager, 0)
{
}

wxThread::ExitCode GammaBlockUSBFake::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	while( ShouldBeRunning() )
	{
		GammaDataUSB* pDataOut(new GammaDataUSB);

		pDataOut->dateTime = wxDateTime::UNow();
		for(int i = 0; i < 0x100; i++)
		{
			pDataOut->data[2 * i + 0] = i;
			pDataOut->data[2 * i + 1] = rand() % 0x100;
		}
		pDataOut->data[0] = pDataOut->data[1] = 0xFF;
		
		DataPush(pDataOut);

		GetThread()->Sleep(1);
	}

	return 0;
}
