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

GammaBlockFileRead::GammaBlockFileRead(GammaManager* pManager) 
	: GammaPipeHead(pManager)
{
	wxLogStatus("%s", __FUNCTION__);
}

GammaBlockFileRead::~GammaBlockFileRead()
{
	wxLogStatus("%s", __FUNCTION__);
}

wxInt32 GammaBlockFileRead::setParam(GammaParam_e param, void* value)
{
	//wxMutexLocker locker(m_processDataMutex);

	switch(param)
	{
	case GAMMA_PARAM_FILE_NAME_READ:
		m_fileName.Assign(*static_cast<wxString*>(value)); break;
	case GAMMA_PARAM_FILE_SET_TIME:
		{
			wxTimeSpan reqTime = *static_cast<wxTimeSpan*>(value);
			setTime(reqTime); break;
		}
	default:
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

wxThread::ExitCode GammaBlockFileRead::Entry()
{
	m_fileName.Assign("D:\\git\\gamma-view\\data\\20120813_184157.gvb"); //jednorodnosc
	//m_fileName.Assign("20120813_200126.gvb"); //siatka
	//m_fileName.Assign("5066901116_002707.gif");
	
	wxLogStatus("%s - started", __FUNCTION__);

	wxLogStatus("File modification time: " 
		+ m_fileName.GetModificationTime().Format());
	m_file.Open(m_fileName.GetFullPath(), wxFile::read);
	
	if(!checkHeader())
	{
		return (wxThread::ExitCode)1;
	}

	wxTimeSpan endTime = getEndTime();
	getManager()->PresentationTierSetParam(GAMMA_PARAM_TIME_END, &endTime);

	wxDateTime startTime(wxDateTime::UNow());
	while(shouldBeRunning())
	{
		{
			//wxMutexLocker locker(m_processDataMutex);
			if(m_file.Eof())
			{
				break;
			}

			GammaItems* dataOut(new GammaItems);
			
			for(wxUint32 iItem = 0; iItem < 256; iItem++)
			{
				m_file.Read(&(dataOut->items[iItem]), sizeof(GammaItem));
			}

			pushData(dataOut);
			delete dataOut;
		}

/*		if( loaded < (100 * m_file.Tell() / m_file.Length()) )
		{
			loaded = (100 * m_file.Tell() / m_file.Length());
			wxLogStatus("%s - loading (%u%%) offset = %"wxLongLongFmtSpec"d", __FUNCTION__, loaded, m_file.Tell() % 12);
		}
*/	}
	wxDateTime stopTime(wxDateTime::UNow());

	wxLogStatus("%s - diffTime = %s", __FUNCTION__, stopTime.Subtract(startTime).Format("%H:%M:%S,%l").c_str());

	m_file.Close();

	wxLogStatus("%s - stoped", __FUNCTION__);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool GammaBlockFileRead::checkHeader()
{
	char tBuffer[3];

	return wxInvalidOffset != m_file.Read(tBuffer, 3) 
		&& 0 == strncmp(tBuffer, "GVB", 3);
}

////////////////////////////////////////////////////////////////////////////////

wxTimeSpan GammaBlockFileRead::getEndTime()
{
	wxFileOffset oldOffset = m_file.Tell();
	wxTimeSpan endTime;

	m_file.Seek(0, wxFromEnd);
	endTime = getTime();
	m_file.Seek(oldOffset, wxFromStart);

	return endTime;
}

bool GammaBlockFileRead::setTime(wxTimeSpan reqTime)
{
	//wxMutexLocker locker(m_processDataMutex);
	//wxFileOffset reqOffset;

	wxTimeSpan leftTime = wxTimeSpan(0);
	wxFileOffset leftOffset = 3;

	wxTimeSpan rightTime = getEndTime();
	wxFileOffset rightOffset = m_file.Length();

	wxTimeSpan tryTime = getTime();
	wxFileOffset tryOffset = m_file.Tell();

	while(!(reqTime - wxTimeSpan::Milliseconds(500) <= tryTime && tryTime <= reqTime + wxTimeSpan::Milliseconds(500)))
	{
		tryOffset = (leftOffset + rightOffset) / 2;
		tryOffset -= (tryOffset % 12) - 3; // Alignment

		m_file.Seek(tryOffset, wxFromStart);

		tryTime = getTime();
		tryOffset = m_file.Tell();

		if(reqTime < tryTime)
		{
			rightTime = tryTime;
			rightOffset = tryOffset;
		}
		else
		{
			leftTime = tryTime;
			leftOffset = tryOffset;
		}
/*		wxLogStatus("%s: reqTime = %s tryTime = %s tryOffset = %"wxLongLongFmtSpec"d", __FUNCTION__, 
			reqTime.Format("%H:%M:%S,%l").c_str(), tryTime.Format("%H:%M:%S,%l").c_str(), tryOffset);
*/	}
	
	return true;
}

wxTimeSpan GammaBlockFileRead::getTime()
{
	wxFileOffset difOffset = sizeof(GammaItem);
	wxFileOffset curOffset = m_file.Tell();
	wxFileOffset endOffset = m_file.Length() - difOffset;
	
	if(endOffset/2 < curOffset)
	{
		difOffset = -difOffset;
	}

	GammaItem item;
	do
	{
		curOffset += difOffset;
		if(!(0 <= curOffset && curOffset <= endOffset))
		{
			return wxTimeSpan(-1);
		}

		m_file.Seek(curOffset, wxFromStart);
		m_file.Read(&item, sizeof(GammaItem));

		//wxLogStatus("%s: offset = %"wxLongLongFmtSpec"d, type = %c", __FUNCTION__, curOffset, item.type);
	}
	while(GAMMA_ITEM_TYPE_TMARKER != item.type);

	return wxTimeSpan(0, 0, 0, item.data.time);
}
