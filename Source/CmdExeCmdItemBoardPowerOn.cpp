#include "StdAfx.h"
#include ".\cmdexecmditemboardpoweron.h"
#include "BoardPowerCtrlManager.h"

CCmdExeCmdItemBoardPowerOn::CCmdExeCmdItemBoardPowerOn(void)
{
	m_sName        = "Board Power On";
	m_sDisplayName = "Board Power On";
}

CCmdExeCmdItemBoardPowerOn::~CCmdExeCmdItemBoardPowerOn(void)
{
}
BOOL         CCmdExeCmdItemBoardPowerOn::Execute()
{	
	Log("Issue board power on...");
	return CBoardPowerCtrlManager::GetInstance().PowerOn();
}

CCmdExeItem* CCmdExeCmdItemBoardPowerOn::Clone()
{
	return new CCmdExeCmdItemBoardPowerOn();
}

BOOL  CCmdExeCmdItemBoardPowerOn::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemBoardPowerOn");
	CCmdExeCmdItem::Serialize(xmlWriter);

	return TRUE;
}

BOOL  CCmdExeCmdItemBoardPowerOn::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	return TRUE;
}


