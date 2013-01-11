/**
 * @file	block_fread.cpp
 * @brief	Reads data from a file
 * @author	Mateusz Plociennik
 * @data	2012-07-15
 */

#include "block_fread.h"

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/datetime.h>

bool GammaBlockFileRead::SetParam(GammaParam_e param, void* value)
{
	switch(param)
	{
	case GAMMA_PARAM_FILE_NAME_READ:
		{
			m_fileName.Assign(*static_cast<wxString*>(value));
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

wxThread::ExitCode GammaBlockFileRead::Entry()
{
	wxMutexLocker locker(m_threadRunMutex);

	m_fileName.Assign("20120813_184157.gvb"); //jednorodnosc
	//m_fileName.Assign("20120813_200126.gvb"); //siatka
	//wxFileName fileName("5066901116_002707.gif");
	
	wxLogStatus("%s - started", __FUNCTION__);

	wxLogStatus("File modification time: " 
		+ m_fileName.GetModificationTime().Format());
	m_file.Open(m_fileName.GetFullPath(), wxFile::read);
	
	if (!CheckHeader())
	{
		return (wxThread::ExitCode)1;
	}

	unsigned char loaded = 0;

	wxTimeSpan timeEnd = GetTimeEnd();
	GetManager()->PresentationTierSetParam(GAMMA_PARAM_TIME_END, &timeEnd);
	
	wxTimeSpan timeNow(0);
	GetManager()->PresentationTierSetParam(GAMMA_PARAM_TIME_NOW, &timeNow);

	wxTimeSpan timeTmp(0);
	while ( ShouldBeRunning() && !m_file.Eof() )
	{
		GammaDataItems* dataOut(new GammaDataItems);
		
		for (unsigned short int i = 0; (i < 256) && (!m_file.Eof()); i++)
		{
			m_file.Read(&(dataOut->data.at(i)), sizeof(GammaItem));
			if (dataOut->data.at(i).type == GAMMA_ITEM_TMARKER)
			{
				timeTmp = wxTimeSpan(0, 0, 0, dataOut->data.at(i).data.time);
				if ( timeTmp - timeNow > wxTimeSpan::Milliseconds(500) )
				{
					timeNow = timeTmp;
					GetManager()->PresentationTierSetParam(GAMMA_PARAM_TIME_NOW, &timeNow);
				}
			}
		}

		DataPush(dataOut);

		if ( loaded < (100 * m_file.Tell() / m_file.Length()) )
		{
			loaded = (100 * m_file.Tell() / m_file.Length());
			wxLogStatus("%s - loading (%u%%)", __FUNCTION__, loaded);
		}
	}
	m_file.Close();

	wxLogStatus("%s - stoped", __FUNCTION__);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool GammaBlockFileRead::CheckHeader()
{
	char tBuffer[3];

	return wxInvalidOffset != m_file.Read(tBuffer, 3) 
		&& 0 == strncmp(tBuffer, "GVB", 3);
}

////////////////////////////////////////////////////////////////////////////////

wxTimeSpan GammaBlockFileRead::GetTimeEnd()
{
	wxFileOffset oldOffset = m_file.Tell();
	wxFileOffset fileOffset = 0;
	GammaItem item;

	do
	{
		fileOffset -= 12; // sizeof(GammaItem)
		m_file.Seek(fileOffset, wxFromEnd);
		m_file.Read(&item, sizeof(GammaItem));
		wxLogStatus("%s: fileOffset = %d, type = %c", 
			__FUNCTION__, ((wxLongLong)fileOffset).GetLo(), item.type);
	} while ( GAMMA_ITEM_TMARKER != item.type && m_file.Tell() > 0 );
	
	m_file.Seek(oldOffset, wxFromStart);

	wxString message("Time end: " 
		+ wxTimeSpan(0, 0, 0, item.data.time).Format("%H:%M:%S,%l"));
	wxMessageBox(message);

	return wxTimeSpan(0, 0, 0, item.data.time);
}
