/**
 * @file	block_usb_fake.cpp
 * @brief	Fake device for testing purposes.
 * @author	Mateusz Plociennik
 * @data	2012-07-29
 */

#include "block_usb_fake.h"

GammaBlockUSBFake::GammaBlockUSBFake(GammaManager* pManager) 
	: GammaPipeHead(pManager)
{
}

wxThread::ExitCode GammaBlockUSBFake::Entry()
{
	//wxMutexLocker locker(m_processDataMutex);

	GammaDataUSB* pDataOut(new GammaDataUSB);
	while( shouldBeRunning() )
	{
		for(int i = 0; i < 256; i++)
		{
			pDataOut->data[2 * i + 0] = i;
			pDataOut->data[2 * i + 1] = rand() % 255;
		}

		int pos = rand() % 256;
		pDataOut->data[2 * pos] = pDataOut->data[2 * pos + 1] = 0xFF;
		
		pushData(pDataOut);
	}
	delete pDataOut;

	return 0;
}
