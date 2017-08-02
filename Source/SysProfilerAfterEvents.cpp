#include "StdAfx.h"
#include "SysProfilerAfterEvents.h"

CSysProfilerAfterEvents::CSysProfilerAfterEvents(void)
{
	m_sName = "After Events";
}

CSysProfilerAfterEvents::~CSysProfilerAfterEvents(void)
{
}

BOOL CSysProfilerAfterEvents::DeSerialize(CMarkup& xmlReader)
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
BOOL CSysProfilerAfterEvents::Run()
{
	return ExecutePhases();
}
