#include "StdAfx.h"
#include "SysProfilerBeforeEvents.h"

CSysProfilerBeforeEvents::CSysProfilerBeforeEvents(void)
{
	m_sName = "Before Events";
}

CSysProfilerBeforeEvents::~CSysProfilerBeforeEvents(void)
{
}

BOOL CSysProfilerBeforeEvents::DeSerialize(CMarkup& xmlReader)
{
	xmlReader.IntoElem();

	if(xmlReader.FindElem("InitPhase") == false)
		return FALSE;

	//Load init phase	
	CSysProfilerEventPhase* pInitPhase = new CSysProfilerEventInitPhase(this);
	if(pInitPhase->DeSerialize(xmlReader) == FALSE)
		return FALSE;
	m_phases.push_back(pInitPhase);

	xmlReader.OutOfElem();
	return TRUE;
}

BOOL CSysProfilerBeforeEvents::Run()
{
	return ExecutePhases();
}

