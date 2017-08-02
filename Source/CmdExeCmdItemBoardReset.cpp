#include "StdAfx.h"
#include ".\cmdexecmditemboardreset.h"
#include "BoardPowerCtrlManager.h"

CCmdExeCmdItemBoardReset::CCmdExeCmdItemBoardReset(void)
{
	m_sName        = "Board Reset";
	m_sDisplayName = "Board Reset";
}

CCmdExeCmdItemBoardReset::~CCmdExeCmdItemBoardReset(void)
{
}

BOOL         CCmdExeCmdItemBoardReset::Execute()
{	
	Log("Issue board reset...");
	return CBoardPowerCtrlManager::GetInstance().Reset(250);
}

CCmdExeItem* CCmdExeCmdItemBoardReset::Clone()
{
	return new CCmdExeCmdItemBoardReset();
}

BOOL  CCmdExeCmdItemBoardReset::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemBoardReset");
	CCmdExeCmdItem::Serialize(xmlWriter);

	return TRUE;
}

BOOL  CCmdExeCmdItemBoardReset::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	return TRUE;
}

