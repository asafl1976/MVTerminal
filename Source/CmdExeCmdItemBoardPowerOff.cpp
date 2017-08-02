#include "StdAfx.h"
#include ".\cmdexecmditemboardpoweroff.h"
#include "BoardPowerCtrlManager.h"

CCmdExeCmdItemBoardPowerOff::CCmdExeCmdItemBoardPowerOff(void)
{
	m_sName        = "Board Power Off";
	m_sDisplayName = "Board Power Off";
}

CCmdExeCmdItemBoardPowerOff::~CCmdExeCmdItemBoardPowerOff(void)
{
}

BOOL         CCmdExeCmdItemBoardPowerOff::Execute()
{	
	Log("Issue board power off...");
	return CBoardPowerCtrlManager::GetInstance().PowerOff();
}

CCmdExeItem* CCmdExeCmdItemBoardPowerOff::Clone()
{
	return new CCmdExeCmdItemBoardPowerOff();
}

BOOL  CCmdExeCmdItemBoardPowerOff::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemBoardPowerOff");
	CCmdExeCmdItem::Serialize(xmlWriter);

	return TRUE;
}

BOOL  CCmdExeCmdItemBoardPowerOff::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	return TRUE;
}



