/**
 * @file	block_usb_fake.cpp
 * @brief	Fake device for testing purposes.
 * @author	Mateusz Plociennik
 * @data	2012-07-29
 */

#include "block_usb_fake.h"

wxThread::ExitCode GammaBlockUSBFake::Entry()
{
	while (!GetThread()->TestDestroy())
	{
		GammaDataUSB* blockDataOut = new GammaDataUSB;
		blockDataOut->data = new unsigned char[512];

		blockDataOut->datetime.UNow();

		for (short int i = 0; i < 256; i++)
		{
			blockDataOut->data[2 * i + 0] = blockDataOut->data[2 * i + 1] = i;
		}
		DataPush(blockDataOut);

		GetThread()->Sleep(10);
	}

	return 0;
}
