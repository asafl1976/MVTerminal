#include "StdAfx.h"
#include "CmdExeCmdItem.h"
#include "MVTerminalView.h"
#include "CmdExeCmdItem.h"
#include "CmdExeCmd.h"

CCmdExeCmdItem::CCmdExeCmdItem(void)
{
	m_sName = "CmdExeCmdItem";
	m_pOwnerCommand = NULL;
}

CCmdExeCmdItem::~CCmdExeCmdItem(void)
{
}

void    CCmdExeCmdItem::Log(CString sLogLine)
{
	if(m_pOwnerCommand != NULL)
	{
		m_pOwnerCommand->Log(m_pOwnerCommand->GetDisplayName()+": " +  sLogLine);
	}
}

BOOL   CCmdExeCmdItem::Serialize(CMarkup & xmlWriter)
{
	return CCmdExeItem::Serialize(xmlWriter);
}

BOOL   CCmdExeCmdItem::DeSerialize(CMarkup & xmlReader)
{
	return CCmdExeItem::DeSerialize(xmlReader);
}

