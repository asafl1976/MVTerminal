#include "StdAfx.h"
#include "CmdExeCmdItemDelay.h"

CCmdExeCmdItemDelay::CCmdExeCmdItemDelay()
{
	m_params.m_type     = CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY;
	m_params.m_nVal     = 0;
	m_params.m_nHighVal = 0;

	m_sName        = "Delay";
	m_sDisplayName = "Delay";

}

CCmdExeCmdItemDelay::~CCmdExeCmdItemDelay(void)
{

}

VOID    CCmdExeCmdItemDelay::Delay(UINT nMiliSec)
{
	int i,nMs, nLoopCount;

	nMs = nMiliSec%1000;
	nLoopCount = nMiliSec/1000;
	for(i = 0; i < nLoopCount; i++)
	{
		Sleep(1000);
	}
	if(nMs != 0)
		Sleep(nMs);
}

BOOL    CCmdExeCmdItemDelay::Execute()
{	
	int			nVal;

	/* Set Post command delay */
	Log(m_sDisplayName);
	switch(m_params.m_type)
	{
	case CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY:
		Delay(m_params.m_nVal);
		break;
	case CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY:
		nVal = m_params.m_nVal + (rand()%(m_params.m_nHighVal-m_params.m_nVal));
		Delay(nVal);
		break;
	default:
		break;
	}
	return TRUE;
}

CCmdExeItem*   CCmdExeCmdItemDelay::Clone()
{
	CCmdExeCmdItemDelay *pClone;
	pClone = new CCmdExeCmdItemDelay();
	pClone->SetParams(GetParams());
	return pClone;
}

BOOL  CCmdExeCmdItemDelay::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemDelay");
	CCmdExeCmdItem::Serialize(xmlWriter);

	xmlWriter.AddAttrib( "HighVal",m_params.m_nHighVal);
	xmlWriter.AddAttrib( "Val",m_params.m_nVal);
	xmlWriter.AddAttrib( "type",m_params.m_type);

	return TRUE;
}

BOOL  CCmdExeCmdItemDelay::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	CString sHighVal, sVal, sType;

	sHighVal = xmlReader.GetAttrib("HighVal");
	sVal     = xmlReader.GetAttrib("Val");
	sType    = xmlReader.GetAttrib("type");

	sscanf(sHighVal,"%d",&m_params.m_nHighVal);
	sscanf(sVal,"%d",&m_params.m_nVal);
	sscanf(sType,"%d",&m_params.m_type);

	return TRUE;
}





