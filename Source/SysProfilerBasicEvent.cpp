#include "StdAfx.h"
#include "SysProfilerBasicEvent.h"
#include "ProfilerEventManager.h"

CSysProfilerBasicEvent::CSysProfilerBasicEvent(void)
{
	ResetMinMaxSampleData();
}

CSysProfilerBasicEvent::~CSysProfilerBasicEvent(void)
{
	vector<CSysProfilerEventPhase*>::iterator iter, endIter;

	iter = m_phases.begin();
	endIter = m_phases.end();

	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
}

void  CSysProfilerBasicEvent::ResetMinMaxSampleData()
{
	m_nMinSample = 1000000.0;
	m_nMaxSample = -1000000.0;
}

void   CSysProfilerBasicEvent::SetArrayValue(int nIndex, double nValue)
{
	if( (nIndex < 0) || (nIndex >= 64) )
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			//CString s;
			//s.Format("        ***Error - invalid index (%d) for SetArrayValue!",nIndex);
			//CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		return;
	}

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		//CString s;
		//s.Format("        Set data array[%d] <-- %0.2f",nIndex,nValue);
		//CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	m_varArray[nIndex] = nValue;
}

double CSysProfilerBasicEvent::GetArrayValue(int nIndex)
{
	if( (nIndex < 0) || (nIndex >= 64) )
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("        ***Error - invalid index (%d) for GetArrayValue!",nIndex);
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		throw string("Invalid data-array index!");		
	}
	
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		//CString s;
		//s.Format("        Get data array[%d] --> %0.2f",nIndex,m_varArray[nIndex]);
		//CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return m_varArray[nIndex];
}

BOOL CSysProfilerBasicEvent::ExecutePhases()
{
	vector<CSysProfilerEventPhase*>::iterator iter, endIter;

	iter = m_phases.begin();
	endIter = m_phases.end();

	for(; iter != endIter; iter++)
	{
		if((*iter)->Execute() == FALSE)
		{
			throw string("Error while executing event phases!");		
		}
	}
	return TRUE;
}

double  CSysProfilerBasicEvent::GetData()
{
	if(ExecutePhases() == FALSE)
	{
		throw string("Error while getting event data!");		
	}
	
	//Get data phase and return sample data
	vector<CSysProfilerEventPhase*>::iterator iter, endIter;

	iter = m_phases.begin();
	endIter = m_phases.end();

	for(; iter != endIter; iter++)
	{
		CSysProfilerEventDataPhase* pDataPhase = dynamic_cast<CSysProfilerEventDataPhase*>(*iter);
		if(pDataPhase != NULL)
		{
			double fSample = pDataPhase->GetSampleData();
			m_nMinSample = (fSample < m_nMinSample)? fSample:m_nMinSample;
			m_nMaxSample = (fSample > m_nMaxSample)? fSample:m_nMaxSample;;

			return fSample;
		}
	}
	throw string("Error while getting event data (data phase no found)!");		
	return -1.0;
}




