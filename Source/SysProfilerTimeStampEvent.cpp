#include "StdAfx.h"
#include "SysProfilerTimeStampEvent.h"
#include "SysProfilerEventPhase.h"

CSysProfilerTimeStampEvent::CSysProfilerTimeStampEvent(void)
{
	m_pInitPhase = NULL;
	m_pDataPhase = NULL;;

	//Set inital value to 0xffffffff
	m_fLastTimeStamp  = 4294967295;

}

CSysProfilerTimeStampEvent::~CSysProfilerTimeStampEvent(void)
{

}

BOOL  CSysProfilerTimeStampEvent::DeSerialize(CMarkup& xmlReader)
{
	m_sName		  = xmlReader.GetAttrib("Name");
	CString sFreq = xmlReader.GetAttrib("Freq");
	sscanf(sFreq,"%d",&m_nFreq);

	xmlReader.IntoElem();

	if(xmlReader.FindElem("Phases") == false)
		return FALSE;
	xmlReader.IntoElem();

	if(xmlReader.FindElem("InitPhase") == false)
		return FALSE;
	//Load init phase	
	m_pInitPhase = new CSysProfilerEventInitPhase(this);
	if(m_pInitPhase->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	if(xmlReader.FindElem("DataPhase") == false)
		return FALSE;
	//Load init phase
	m_pDataPhase = new CSysProfilerEventDataPhase(this);
	if(m_pDataPhase->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	xmlReader.OutOfElem();
	xmlReader.OutOfElem();
	return TRUE;
}

BOOL  CSysProfilerTimeStampEvent::ExecuteInitPhase()
{
	if(m_pInitPhase != NULL)
	{
		m_fLastTimeStamp  = 4294967295;
		return m_pInitPhase->Execute();
	}
	return TRUE;
}

BOOL  CSysProfilerTimeStampEvent::ExecuteDataPhase()
{
	if(m_pDataPhase != NULL)
	{
		return m_pDataPhase->Execute();
	}
	return TRUE;
}

double  CSysProfilerTimeStampEvent::GetData()
{
	return GetTimeStamp();
}

double  CSysProfilerTimeStampEvent::GetTimeStamp()
{
	double fSampleRateMS, fTimeStamp = m_pDataPhase->GetSampleData();
	fSampleRateMS = ((m_fLastTimeStamp - fTimeStamp)/25000000)*1000;
	m_fLastTimeStamp = fTimeStamp;
	return fSampleRateMS;
}




