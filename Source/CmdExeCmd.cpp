#include "StdAfx.h"
#include "CmdExeCmd.h"
#include "MVTerminalView.h"
#include "CmdExeItemFactory.h"
#include "MVTerminal.h"
#include "CmdExePane.h"

CCmdExeCmd::CCmdExeCmd()
{
	m_nLoopCount     = 1;
	m_bStopExecution = FALSE;
	m_sName = "CmdExeCmd";
}

CCmdExeCmd::CCmdExeCmd(CCmdExeCmd & cmd)
{
	int	 i, nCount;

	nCount = cmd.GetCmdItemCount();
	//Copy all items
	for(i = 0; i < nCount; i++)
	{
		CCmdExeItem *pCmdItem;
		pCmdItem = cmd.GetCmdItem(i);
		AddCmdItem(pCmdItem->Clone());
	} 

	m_bStopExecutionOnError = cmd.GetStopExecutionOnError();
	m_bEnable               = cmd.GetEnable();
	m_sName                 = cmd.GetName();
	m_sDisplayName          = cmd.GetDisplayName();
	m_nLoopCount            = cmd.GetLoopCount();
	m_bErrorOccured         = cmd.GetErrorOccured();
}


CCmdExeCmd::~CCmdExeCmd(void)
{
	DeleteItems();
}

void   CCmdExeCmd::RemoveItems()
{
	m_cmdItems.clear();
}

void CCmdExeCmd::DeleteItems()
{
	vector<CCmdExeItem*>::iterator iter,endIter;
	int								  i = 0;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
	m_cmdItems.clear();
}

int CCmdExeCmd::GetCmdItemCount()
{
	return (int)m_cmdItems.size();
}

CCmdExeItem*    CCmdExeCmd::GetCmdItem(int nIndex)
{
	vector<CCmdExeItem*>::iterator iter,endIter;
	int								  i = 0;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(; iter != endIter; iter++)
	{
		if(i == nIndex)
		{
			return (*iter);
		}
		i++;
	}
	return NULL;
}

void CCmdExeCmd::Log(CString sLogLine)
{
	CCmdExeModel::GetInstance().Log(sLogLine);
}

void CCmdExeCmd::AddCmdItem(CCmdExeItem* pCmdItem)
{
	m_cmdItems.push_back(pCmdItem);
	CCmdExeCmdItem *pCmdExeCmdItem = dynamic_cast<CCmdExeCmdItem*>(pCmdItem);
	if(pCmdExeCmdItem != NULL)
	{
		pCmdExeCmdItem->SetOwnerCommand(this);
	}
}

BOOL CCmdExeCmd::DeleteItem(int nIndex)
{
	vector<CCmdExeItem*>::iterator iter,endIter;
	int										 i = 0;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(; iter != endIter; iter++)
	{
		if(i == nIndex)
		{
			delete (*iter);
			m_cmdItems.erase(iter);
			return TRUE;
		}
		i++;
	}
	return FALSE;
}

BOOL CCmdExeCmd::DeleteItem(CCmdExeItem* pItem)
{
	vector<CCmdExeItem*>::iterator iter,endIter;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(; iter != endIter; iter++)
	{
		if(pItem == (*iter))
		{
			delete (*iter);
			m_cmdItems.erase(iter);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CCmdExeCmd::Execute()
{
	vector<CCmdExeItem*>::iterator iter,endIter;
	int								  nCmdItem = 0;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	if(m_bEnable == FALSE)
		return TRUE;
	
	for(; iter != endIter; iter++)
	{	
		if(m_bStopExecution == TRUE)
		{
			return TRUE;
		}		
		if((*iter)->GetEnable() == FALSE)
		{
			continue;
		}
		/* Clear state error flag indication */
		(*iter)->SetErrorOccured(FALSE);
		if((*iter)->Execute() == FALSE)
		{
			(*iter)->SetErrorOccured(TRUE);
			SetErrorOccured(FALSE);
			return FALSE;
		}
		nCmdItem++;
		//Delay command item execution (for visual purpose)
		Sleep(50);
	}
	return TRUE;
}


BOOL    CCmdExeCmd::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmd");
	CCmdExeItem::Serialize(xmlWriter);
	xmlWriter.AddAttrib( "LoopCount",m_nLoopCount);
	xmlWriter.AddAttrib( "StopExecution",m_bStopExecution);

	xmlWriter.IntoElem();
	xmlWriter.AddElem( "CmdExeCmdItems");
	xmlWriter.IntoElem();
	vector<CCmdExeItem*>::iterator iter = m_cmdItems.begin(),endIter = m_cmdItems.end();
	for(; iter != endIter; iter++)
	{	
		(*iter)->Serialize(xmlWriter);
	}
	xmlWriter.OutOfElem();
	xmlWriter.OutOfElem();

	return TRUE;
}

BOOL    CCmdExeCmd::DeSerialize(CMarkup & xmlReader)
{
	CString sLoopCount;

	if(CCmdExeItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	sLoopCount = xmlReader.GetAttrib("LoopCount");
	sscanf(sLoopCount,"%d",&m_nLoopCount);
	m_bStopExecution = (xmlReader.GetAttrib("StopExecution") == "0")? FALSE:TRUE;

	xmlReader.IntoElem();
	if(xmlReader.FindElem("CmdExeCmdItems") == false)
		return FALSE;
	xmlReader.IntoElem();

	//Load cmd items
	while ( xmlReader.FindElem("CmdExeCmdItem") )
	{	
		CString      sName;
		CCmdExeItem *pItem;

		sName = xmlReader.GetAttrib("Name");
		pItem = CCmdExeItemFactory::GetInstance().CreateItem(sName);
		if(pItem != NULL)
		{
			if(pItem->DeSerialize(xmlReader) == TRUE)
			{
				AddCmdItem(pItem);
			}
			else
			{
				delete pItem;
				return FALSE;
			}
		}
	}
	xmlReader.OutOfElem();
	xmlReader.OutOfElem();
	return TRUE;

}

CCmdExeItem*   CCmdExeCmd::Clone()
{
	CCmdExeCmd *pCmd;
	pCmd = new CCmdExeCmd();
	return pCmd;
}

void    CCmdExeCmd::operator=(CCmdExeCmd & cmd)
{	
	int					 i, nCount;

	nCount =  cmd.GetCmdItemCount();
	//Copy all items
	for(i = 0; i < nCount; i++)
	{
        CCmdExeItem *pCmdItem = cmd.GetCmdItem(i);
		CCmdExeItem *pClone = pCmdItem->Clone();//Deep copy
	   *pClone = *pCmdItem;//Shallow copy
		AddCmdItem(pClone);
	} 
	m_bStopExecutionOnError = cmd.GetStopExecutionOnError();
	m_bEnable               = cmd.GetEnable();
	m_sName                 = cmd.GetName();
	m_sDisplayName          = cmd.GetDisplayName();
	m_nLoopCount            = cmd.GetLoopCount();
	m_bErrorOccured         = cmd.GetErrorOccured();
}


