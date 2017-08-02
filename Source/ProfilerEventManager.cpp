#include "StdAfx.h"
#include "ProfilerEventManager.h"
#include "PathDialog.h"
#include "Markup.h"

CProfilerEventManager::CProfilerEventManager(void)
{
	m_bInitialized  = FALSE;
	m_pBeforeEvents = NULL;
	m_pAfterEvents  = NULL;
	m_pSystemInit   = NULL;
}



CProfilerEventManager::~CProfilerEventManager(void)
{
	DeleteAllEvents();

	if(m_pBeforeEvents != NULL)
		delete m_pBeforeEvents;

	if(m_pAfterEvents != NULL)
		delete m_pAfterEvents;

	if(m_pSystemInit != NULL)
		delete m_pSystemInit;

	if(m_pTimeStampEvent != NULL)
		delete m_pTimeStampEvent;

}

void CProfilerEventManager::DeleteAllEvents()
{
	vector<CSysProfilerBasicEvent*>::iterator iter, endIter;

	iter = m_events.begin();
	endIter = m_events.end();

	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
}


CProfilerEventManager& CProfilerEventManager::GetInstance()
{
    /* Allocate system object once on the heap */
    static CProfilerEventManager *pObj = new CProfilerEventManager();
    return *pObj; 
}

BOOL  CProfilerEventManager::SystemInit()
{
	if(m_pSystemInit == NULL)
	{			
		throw string("m_pSystemInit == NULL!");
	}
	return m_pSystemInit->Run();
}

BOOL    CProfilerEventManager::InitTimeStamp()
{
	if(m_pTimeStampEvent != NULL)
		return m_pTimeStampEvent->ExecuteInitPhase();

	return TRUE;
}

double  CProfilerEventManager::GetTimeStamp()
{
	if(m_pTimeStampEvent != NULL)
		return m_pTimeStampEvent->GetTimeStamp();

	return 0;
}

void  CProfilerEventManager::SetHwModuleItem(CMarvellDeviceSysProfilerHwModule *pItem)
{
	m_pMarvellDeviceSysProfilerHwModule = pItem;
}

BOOL	CProfilerEventManager::ReadMarvellDeviceRegister(UINT32 nRegAddress, UINT32 *pRegVal)
{
	BOOL bRes = m_pMarvellDeviceSysProfilerHwModule->ReadRegister(nRegAddress,pRegVal);
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		if(bRes == FALSE)
		{
			s.Format("      *** Error during read register 0x%x!",nRegAddress);
		}
		else
		{
			s.Format("      Read register 0x%x --> 0x%x",nRegAddress,*pRegVal);
		}
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}
	if(bRes == FALSE)
	{
		throw string("Error while reading register!");
	}
	return bRes;
}

BOOL	CProfilerEventManager::WriteMarvellDeviceRegister(UINT32 nRegAddress,UINT32 nVal)
{	
	BOOL bRes = m_pMarvellDeviceSysProfilerHwModule->WriteRegister(nRegAddress,nVal);

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		if(bRes == FALSE)
		{
			s.Format("      *** Error during write register 0x%x!",nRegAddress);
		}
		else
		{
			s.Format("      Write register 0x%x <-- 0x%x",nRegAddress,nVal);
		}
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}
	if(bRes == FALSE)
	{
		throw string("Error while writing register!");
	}
	return bRes;
}

void	CProfilerEventManager::SetGlobalDataArrayValue(int nIndex, double nData)
{
	if( (nIndex < 0) || (nIndex >= 64) )
	{
		throw string("Invalid global data array index!");
	}
	m_globalDataArray[nIndex] = nData;

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		//CString s;
		//s.Format("        Set global data array[%d] <-- %0.2f",nIndex,nData);
		//CProfilerEventManager::GetInstance().AddLogLine(s);
	}
}


double	CProfilerEventManager::GetGlobalDataArrayValue(int nIndex)
{
	if( (nIndex < 0) || (nIndex >= 64) )
	{
		throw string("Invalid global data array index!");
	}

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		//CString s;
		//s.Format("        Get global data array[%d] --> %0.2f",nIndex,m_globalDataArray[nIndex]);
		//CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return m_globalDataArray[nIndex];
}

void CProfilerEventManager::AddEvent(CSysProfilerEvent* pEvent)
{
	m_events.push_back(pEvent);
	NotifySysProfilerModelAddedEvent(pEvent);
}

void  CProfilerEventManager::OnEventsBegin()
{
	if(m_pBeforeEvents != NULL)
	{
		if(m_pBeforeEvents->Run() == FALSE)
		{
			throw string("Error while executing 'BeforeEvents' phase!");
		}
	}
}

void CProfilerEventManager::OnEventsEnd()
{
	if(m_pAfterEvents != NULL)
	{
		if(m_pAfterEvents->Run() == FALSE)
		{
			throw string("Error while executing 'AfterEvents' phase!");		
		}
	}
}

void CProfilerEventManager::RemoveAllEvents()
{
	DeleteAllEvents();
	m_events.clear();
	NotifySysProfilerModelRemoveAllEvents();
}

BOOL CProfilerEventManager::LoadConfigFile(CString sConfigFilePath)
{
	m_sConfigFilePath = sConfigFilePath;

	if(m_sConfigFilePath.IsEmpty() == FALSE)
	{
		CMarkup xmlReader;

		//Open and load the file
		if(xmlReader.Load(m_sConfigFilePath) == false)
		{
			AfxMessageBox("Error while loading file!",NULL,MB_ICONERROR);
			return FALSE;
		}

		if(xmlReader.FindElem("SystemProfilerConfiguration") == false)
		{
			AfxMessageBox("Error - time stamp xml data not found!",NULL,MB_ICONERROR);
			return FALSE;
		}
		xmlReader.IntoElem();

		if(xmlReader.FindElem("SystemInit") == false)
		{
			AfxMessageBox("Error - time stamp xml data not found!",NULL,MB_ICONERROR);
			return FALSE;
		}
		//Get the time stamp
		m_pSystemInit = new CSysProfilerInit();
		if(m_pSystemInit->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Error while loading system init xml data!",NULL,MB_ICONERROR);
			return FALSE;
		}

		if(xmlReader.FindElem("TimeStampEvent") == false)
		{
			AfxMessageBox("Error - time stamp xml data not found!",NULL,MB_ICONERROR);
			return FALSE;
		}

		//Get the time stamp
		m_pTimeStampEvent = new CSysProfilerTimeStampEvent();
		if(m_pTimeStampEvent->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Error while loading time stamp xml data!",NULL,MB_ICONERROR);
			return FALSE;
		}

		//Before Events tag
		if(xmlReader.FindElem("BeforeEvents") == false)
		{
			AfxMessageBox("Error - BeforeEvents xml data not found!",NULL,MB_ICONERROR);
			return FALSE;
		}
		//Load Before Events tag data
		m_pBeforeEvents = new CSysProfilerBeforeEvents();
		if(m_pBeforeEvents->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Error while loading BeforeEvents xml data!",NULL,MB_ICONERROR);
			return FALSE;
		}

		if(xmlReader.FindElem("SystemProfilerEvents") == false)
			return FALSE;

		xmlReader.IntoElem();

		while ( xmlReader.FindElem("SystemProfilerEvent") )
		{	
			CSysProfilerEvent* pProfilerEvent = new CSysProfilerEvent();
			if(pProfilerEvent->DeSerialize(xmlReader) == TRUE)
			{
				AddEvent(pProfilerEvent);
			}
			else
			{
				delete pProfilerEvent;
			}
		}
		xmlReader.OutOfElem();

		//After Events tag
		if(xmlReader.FindElem("AfterEvents") == false)
		{
			AfxMessageBox("Error - AfterEvents xml data not found!",NULL,MB_ICONERROR);
			return FALSE;
		}
		//Load After Events tag data
		m_pAfterEvents = new CSysProfilerAfterEvents();
		if(m_pAfterEvents->DeSerialize(xmlReader) == FALSE)
		{
			AfxMessageBox("Error while loading AfterEvents xml data!",NULL,MB_ICONERROR);
			return FALSE;
		}

		xmlReader.OutOfElem();
	}

	return TRUE;
}

int  CProfilerEventManager::GetEventCount()
{
	return m_events.size();
}

CSysProfilerBasicEvent*  CProfilerEventManager::GetEvent(int nIndex)
{
	return m_events[nIndex];
}

void  CProfilerEventManager::RegisterObserver(ISysProfilerObserver *pObs)
{
	vector<ISysProfilerObserver*>::iterator iter, endIter;

	iter    = m_observers.begin();
	endIter = m_observers.end();
	for(; iter != endIter ; iter++)
	{
		if((*iter) == pObs)
			return;
	}
	m_observers.push_back(pObs);
}

void  CProfilerEventManager::UnRegisterObserver(ISysProfilerObserver *pObs)
{
	vector<ISysProfilerObserver*>::iterator iter, endIter;

	iter    = m_observers.begin();
	endIter = m_observers.end();
	for(; iter != endIter ; iter++)
	{
		if((*iter) == pObs)
		{
			m_observers.erase(iter);
			return;
		}
	}
}


void  CProfilerEventManager::AddLogLine(CString sLine)
{
	if(m_bEnableLogging == TRUE)
	{
		NotifySysProfilerModelAddedLogLine(sLine);
	}
}

void  CProfilerEventManager::NotifySysProfilerModelRemoveAllEvents()
{
	vector<ISysProfilerObserver*>::iterator iter, endIter;

	iter    = m_observers.begin();
	endIter = m_observers.end();
	for(; iter != endIter ; iter++)
	{
		(*iter)->OnSysProfilerModelRemoveAllEvents();
	}
}

void  CProfilerEventManager::NotifySysProfilerModelAddedEvent(CSysProfilerEvent *pEvent)
{
	vector<ISysProfilerObserver*>::iterator iter, endIter;

	iter    = m_observers.begin();
	endIter = m_observers.end();
	for(; iter != endIter ; iter++)
	{
		(*iter)->OnSysProfilerModelAddedEvent(pEvent);
	}
}

void  CProfilerEventManager::NotifySysProfilerModelAddedLogLine(CString sLogLine)
{
	vector<ISysProfilerObserver*>::iterator iter, endIter;

	iter    = m_observers.begin();
	endIter = m_observers.end();
	for(; iter != endIter ; iter++)
	{
		(*iter)->OnSysProfilerModelAddedLogLine(sLogLine);
	}
}



