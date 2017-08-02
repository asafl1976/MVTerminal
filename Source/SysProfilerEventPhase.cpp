#include "StdAfx.h"
#include "SysProfilerEventPhase.h"
#include "ProfilerEventManager.h"
#include "SysProfilerEvent.h"
#include "SysProfilerEventPhaseRegIO.h"

//////////////////////////////////// Phase //////////////////////////////////////////////////

CSysProfilerEventPhase::CSysProfilerEventPhase(CSysProfilerBasicEvent* pParentEvent)
{
	m_bExecuted        = FALSE;
	m_pParentEvent     = pParentEvent;
	m_pRegIOSection    = NULL;
	m_pFunctionSection = NULL;
}

CSysProfilerEventPhase::~CSysProfilerEventPhase(void)
{
	if(m_pRegIOSection != NULL)
		delete m_pRegIOSection ;

	if(m_pFunctionSection != NULL)
		delete m_pFunctionSection ;
}

BOOL  CSysProfilerEventPhase::LoadRegisterIO(CMarkup& xmlReader)
{
	if(xmlReader.FindElem("RegisterIO") == false)
		return FALSE;

	m_pRegIOSection = new CSysProfilerEventPhaseRegIOSection(m_pParentEvent);
	return m_pRegIOSection->DeSerialize(xmlReader);
}

BOOL  CSysProfilerEventPhase::LoadFunction(CMarkup& xmlReader)
{	
	if(xmlReader.FindElem("Function") == false)
		return FALSE;
	m_pFunctionSection = new CSysProfilerEventPhaseFunctionSection(m_pParentEvent);
	return m_pFunctionSection->DeSerialize(xmlReader);
}

BOOL CSysProfilerEventPhase::ExecuteFunction()
{
	if(m_pFunctionSection != NULL)
		return m_pFunctionSection->Execute();
	return TRUE;
}

BOOL CSysProfilerEventPhase::ExecuteRegIO()
{
	if(m_pRegIOSection != NULL)
		return m_pRegIOSection->Execute();
	return TRUE;
}

BOOL CSysProfilerEventPhase::Execute()
{
	if((m_bExecuted == FALSE) || (GetExecuteOnce() == FALSE))
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("--> Start '%s' for event '%s'...",m_sName,m_pParentEvent->GetName());
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}

		ExecuteRegIO();
		ExecuteFunction();
		m_bExecuted = TRUE;
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("<-- End %s for event '%s'...",m_sName,m_pParentEvent->GetName());
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
	}
	return TRUE;
}


//////////////////////////////////// Init Phase //////////////////////////////////////////////////
CSysProfilerEventInitPhase::CSysProfilerEventInitPhase(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhase(pParentEvent)
{
	m_bExecuteOnce = TRUE;
	m_sName        = "Init Phase";
}

CSysProfilerEventInitPhase::~CSysProfilerEventInitPhase(void)
{
}

BOOL  CSysProfilerEventInitPhase::DeSerialize(CMarkup& xmlReader)
{
	CString sExecuteOnce = xmlReader.GetAttrib("ExecuteOnce");
	if(sExecuteOnce == "TRUE")
		m_bExecuteOnce = TRUE;
	else
		m_bExecuteOnce = FALSE;

	xmlReader.IntoElem();
	LoadRegisterIO(xmlReader);
	xmlReader.OutOfElem();
	return TRUE;
}


//////////////////////////////////// Data Phase //////////////////////////////////////////////////
CSysProfilerEventDataPhase::CSysProfilerEventDataPhase(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhase(pParentEvent)
{
	m_bExecuteOnce = FALSE;
	m_sName        = "Data Phase";
}

CSysProfilerEventDataPhase::~CSysProfilerEventDataPhase(void)
{
}

double  CSysProfilerEventDataPhase::GetSampleData()
{
	if(Execute() == FALSE)
	{
		throw string("Event DataPhase - Error while getting sample data!");		
	}
	if(m_pFunctionSection == NULL)
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("  Result = NULL  -->  0");
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		return 0.0;
	}
	return m_pFunctionSection->GetResult();
}

BOOL  CSysProfilerEventDataPhase::DeSerialize(CMarkup& xmlReader)
{
	xmlReader.IntoElem();
	LoadRegisterIO(xmlReader);
	LoadFunction(xmlReader);
	xmlReader.OutOfElem();
	return TRUE;
}



