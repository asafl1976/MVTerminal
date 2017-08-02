#include "StdAfx.h"
#include "SysProfilerEvent.h"
#include "Markup.h"

CSysProfilerEvent::CSysProfilerEvent(void)
{
	m_pAlternateColor = NULL;
	m_nRC			  = 0;
	m_colorMap["y"] = RGB(255,250,70);
	m_colorMap["Y"] = RGB(255,250,70);

	m_colorMap["g"] = RGB(50,255,45);
	m_colorMap["G"] = RGB(50,255,45);

	m_colorMap["w"] = RGB(255,255,255);
	m_colorMap["W"] = RGB(255,255,255);

	m_colorMap["r"] = RGB(255,50,50);
	m_colorMap["R"] = RGB(255,50,50);

	m_colorMap["b"] = RGB(60,228,253);
	m_colorMap["B"] = RGB(60,228,253);

	m_colorMap["d"] = RGB(255,250,70);
	m_colorMap["D"] = RGB(255,250,70);

	m_bUseAlternateColor = TRUE;
	m_bEnableTwoDotLine  = FALSE;
}

CSysProfilerEvent::~CSysProfilerEvent(void)
{
	if(m_pAlternateColor != NULL)
		delete m_pAlternateColor;
}

COLORREF  CSysProfilerEvent::GetColor(CString sKey) 
{
	map<CString, COLORREF>::iterator iter;
	
	iter = m_colorMap.find(sKey);
	if(iter != m_colorMap.end())
	{
		return iter->second;
	}
	return m_colorMap.find("D")->second;
}

COLORREF  CSysProfilerEvent::GetGraphLineColor() 
{
	if(GetUseAlternateColor() == FALSE)
		return GetColor(m_sColorKey);

	if(m_pAlternateColor != NULL)
		return m_pAlternateColor->GetColor();
	else
		return GetColor(m_sColorKey);
}

COLORREF  CSysProfilerEvent::GetDefaultColor()
{
	return m_colorMap.find("D")->second;
}


BOOL CSysProfilerEvent::DeSerialize(CMarkup& xmlReader)
{

	m_sName		   = xmlReader.GetAttrib("Name");
	m_sColorKey    = xmlReader.GetAttrib("Color");
	m_sUnits       = xmlReader.GetAttrib("Units");
	CString sRange = xmlReader.GetAttrib("Range");

	sscanf(sRange,"%d,%d",&m_nMin,&m_nMax);

	xmlReader.IntoElem();

	if(xmlReader.FindElem("AlternateColor") == true)
	{
		m_pAlternateColor = new CSysProfilerAlternateColor(this);
		if(m_pAlternateColor->DeSerialize(xmlReader) == FALSE)
			return FALSE;
	}

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

BOOL CSysProfilerEvent::ExecutePhases()
{
	if(m_pAlternateColor != NULL)
	{
		if(m_pAlternateColor->Execute() == FALSE)
			return FALSE;
	}
	if(CSysProfilerBasicEvent::ExecutePhases() == FALSE)
		return FALSE;
	return TRUE;
}

