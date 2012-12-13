/**
 * @file	block_fread.cpp
 * @brief	Reads data from a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fread.h"

#include <wx/filename.h>

bool GammaBlockFileRead::SetParam(GammaParam_e name, void* value)
{
	switch ( name )
	{
	case GAMMA_PARAM_FILE_READ_NAME:
		{
			m_fileName(*static_cast<wxFileName>(value));
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}

wxThread::ExitCode GammaBlockFileRead::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	//wxFileName fileName("20120813_184157.gvb"); //jednorodnosc
	wxFileName fileName("20120813_200126.gvb"); //siatka
	//wxFileName fileName("5066901116_002707.gif");
	wxFile file;
	char tBuffer[3];

	wxLogStatus("%s - started", __FUNCTION__);

	wxLogStatus("File modification time: " 
		+ fileName.GetModificationTime().Format());
	file.Open(fileName.GetFullPath(), wxFile::read);
	
	if ( (wxInvalidOffset != file.Read(tBuffer, 3)) 
		&& (0 == strncmp(tBuffer, "GVB", 3)) )
	{
		unsigned long int timeCounter = 0;
		unsigned char loaded = 0;

		while ( ShouldBeRunning() )
		{
			GammaDataItems* dataOut(new GammaDataItems);
			
			for (unsigned short int i = 0; (i < 256) && (!file.Eof()); i++)
			{
				file.Read(&(dataOut->data.at(i)), sizeof(GammaItem));
				if (dataOut->data.at(i).type == GAMMA_ITEM_TMARKER)
				{
					timeCounter = dataOut->data.at(i).data.time;
				}
			}

			DataPush(dataOut);

			if ( loaded < (100 * file.Tell() / file.Length()) )
			{
				loaded = (100 * file.Tell() / file.Length());
				wxLogStatus("%s - loading (%u%%)", __FUNCTION__, loaded);
			}
		}
	}
	file.Close();

	wxLogStatus("%s - stoped", __FUNCTION__);

	return 0;
}
