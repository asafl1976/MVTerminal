#include "StdAfx.h"
#include "SysProfilerInit.h"

CSysProfilerInit::CSysProfilerInit(void)
{
}

CSysProfilerInit::~CSysProfilerInit(void)
{
}

BOOL CSysProfilerInit::DeSerialize(CMarkup& xmlReader)
{
	m_sName	 = xmlReader.GetAttrib("Name");

	xmlReader.IntoElem();

	if(xmlReader.FindElem("Phases") == false)
		return FALSE;

	xmlReader.IntoElem();

	if(xmlReader.FindElem("InitPhase") == false)
		return FALSE;

	//Load init phase	
	CSysProfilerEventPhase* pInitPhase = new CSysProfilerEventInitPhase(this);
	if(pInitPhase->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	m_phases.push_back(pInitPhase);

	if(xmlReader.FindElem("DataPhase") == false)
		return FALSE;
	//Load init phase
	CSysProfilerEventPhase* pDataPhase = new CSysProfilerEventDataPhase(this);
	if(pDataPhase->DeSerialize(xmlReader) == FALSE)
		return FALSE;

	m_phases.push_back(pDataPhase);

	xmlReader.OutOfElem();
	xmlReader.OutOfElem();
	return TRUE;
}

BOOL CSysProfilerInit::Run()
{
	return ExecutePhases();
}

