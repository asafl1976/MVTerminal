#include "StdAfx.h"
#include "SysProfilerAlternateColor.h"
#include "SysProfilerEvent.h"
#include "ProfilerEventManager.h"

CSysProfilerAlternateColor::CSysProfilerAlternateColor(CSysProfilerEvent* pParentEvent)
{
	m_pParentEvent = pParentEvent;
}

CSysProfilerAlternateColor::~CSysProfilerAlternateColor(void)
{
}


BOOL  CSysProfilerAlternateColor::DeSerialize(CMarkup& xmlReader)
{
	xmlReader.IntoElem();

	if(xmlReader.FindElem("RegisterIO") == false)
		return FALSE;
	m_pRegIOSection = new CSysProfilerEventPhaseRegIOSection(m_pParentEvent);
	if(m_pRegIOSection->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	if(xmlReader.FindElem("Function") == false)
		return FALSE;

	m_pFunctionSection = new CSysProfilerEventPhaseFunctionSection(m_pParentEvent);
	if(m_pFunctionSection->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	xmlReader.OutOfElem();

	return TRUE;
}

BOOL  CSysProfilerAlternateColor::Execute()
{
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("  Start executing alternate color...",m_pParentEvent->GetName());
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	if(m_pRegIOSection != NULL)
	{
		if(m_pRegIOSection->Execute() == FALSE)
			return FALSE;
	}

	if(m_pFunctionSection != NULL)
	{
		if(m_pFunctionSection->Execute() == FALSE)
			return FALSE;
	}

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("  End executing alternate color...",m_pParentEvent->GetName());
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return TRUE;
}

COLORREF  CSysProfilerAlternateColor::GetColor()
{
	if(m_pFunctionSection != NULL)
	{
		UINT32 nResult = (UINT32)m_pFunctionSection->GetResult();
		char chColorKey = nResult & 0xff;
		CString sColorKey = "d";
		sColorKey.SetAt(0, chColorKey);
		return m_pParentEvent->GetColor(sColorKey);
	}
	return m_pParentEvent->GetColor("d");
}
