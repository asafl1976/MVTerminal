#include "StdAfx.h"
#include ".\cmdexeitem.h"

CCmdExeItem::CCmdExeItem(void)
{
	m_bEnable               = TRUE;
	m_bStopExecutionOnError = TRUE;
	m_bErrorOccured         = FALSE;
}

CCmdExeItem::~CCmdExeItem(void)
{
}


BOOL	CCmdExeItem::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "DisplayName",GetDisplayName());
	xmlWriter.AddAttrib( "Enable",m_bEnable);
	xmlWriter.AddAttrib( "StopExecutionOnError",m_bStopExecutionOnError);
	xmlWriter.AddAttrib( "ErrorOccured",m_bErrorOccured);

	return TRUE;
}

BOOL	CCmdExeItem::DeSerialize(CMarkup & xmlReader)
{
	m_sName                 = xmlReader.GetAttrib("Name");
	m_sDisplayName          = xmlReader.GetAttrib("DisplayName");
	m_bEnable               = (xmlReader.GetAttrib("Enable") == "0")? FALSE:TRUE;
	m_bStopExecutionOnError = (xmlReader.GetAttrib("StopExecutionOnError") == "0")? FALSE:TRUE;
	m_bErrorOccured         = (xmlReader.GetAttrib("ErrorOccured") == "0")? FALSE:TRUE;

	return TRUE;
}
