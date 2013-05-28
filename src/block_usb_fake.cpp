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
	wxMutexLocker locker(m_processDataMutex);

	while( shouldBeRunning() )
	{
		GammaDataUSB* pDataOut(new GammaDataUSB);

		//pDataOut->dateTime = wxDateTime::UNow();
		for(int i = 0; i < 256; i++)
		{
			pDataOut->data[2 * i + 0] = i;
			pDataOut->data[2 * i + 1] = rand() % 0x100;
		}
		pDataOut->data[0] = pDataOut->data[1] = 0xFF;
		pDataOut->data[8] = pDataOut->data[9] = 0xFF;
		pDataOut->data[20] = pDataOut->data[21] = 0xFF;
		pDataOut->data[34] = pDataOut->data[35] = 0xFF;
		pDataOut->data[80] = pDataOut->data[81] = 0xFF;
		pDataOut->data[122] = pDataOut->data[123] = 0xFF;
		
		pushData(pDataOut);
		delete pDataOut;
		//GetThread()->Sleep(1);
	}

	return 0;
}
