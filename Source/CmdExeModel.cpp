#include "StdAfx.h"
#include ".\cmdexemodel.h"

CCmdExeModel::CCmdExeModel(void)
{
	m_bStopExecution      = FALSE;
	m_bInfiniteExecution  = FALSE;
	m_nExecutionLoopCount = 1;
}

CCmdExeModel::~CCmdExeModel(void)
{
	vector<CCmdExeCmd*>::iterator  iter, endIter;

	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	
	for(;iter != endIter ; iter++)
	{
		delete (*iter);
	}
	m_cmdVec.clear();
}

CCmdExeModel& CCmdExeModel::GetInstance()
{
    /* Allocate system object once on the heap */
    static CCmdExeModel *pObj = new CCmdExeModel();
    return *pObj;
}

BOOL		CCmdExeModel::IsCommandNameExists(CString sCmdName)
{
	return FALSE;

	vector<CCmdExeCmd*>::iterator  iter, endIter;

	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	
	for(;iter != endIter ; iter++)
	{
		if((*iter)->GetName() == sCmdName)
			return TRUE;
	}
	return FALSE;
}



BOOL        CCmdExeModel::UpdateCommand(CCmdExeCmd *pCmd)
{		
	return NotifyUpdateCommand(pCmd);

}

BOOL        CCmdExeModel::AddCommand(CCmdExeCmd *pCmd)
{		
	if(IsCommandNameExists(pCmd->GetName()) == FALSE)
	{
        m_cmdVec.push_back(pCmd);
		NotifyAddCommand(pCmd);
		return TRUE;
	}
	return FALSE;
}

BOOL        CCmdExeModel::RemoveAllCommands()
{
	vector<CCmdExeCmd*>::iterator  iter, endIter;

	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	
	for(;iter != endIter ; iter++)
	{
		if(NotifyRemoveCommand(*iter) == FALSE)
			return FALSE;

		delete (*iter);
	}
	m_cmdVec.clear();
	return TRUE;
}

BOOL        CCmdExeModel::RemoveCommand(CCmdExeCmd *pCmd)
{
	vector<CCmdExeCmd*>::iterator  iter, endIter;

	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	
	for(;iter != endIter ; iter++)
	{
		if((*iter) == pCmd)
		{
			NotifyRemoveCommand(pCmd);
			delete (*iter);
			m_cmdVec.erase(iter);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL        CCmdExeModel::RemoveCommand(CString sCmdName)
{
	CCmdExeCmd *pCmd = GetCommand(sCmdName);
	if(pCmd != NULL)
		return RemoveCommand(pCmd);
	return FALSE;
}


CCmdExeCmd* CCmdExeModel::GetCommand(CString sCmdName)
{
	vector<CCmdExeCmd*>::iterator  iter, endIter;

	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	
	for(;iter != endIter ; iter++)
	{
		if((*iter)->GetName() == sCmdName)
		{
			return (*iter);
		}
	}
	return NULL;
}

BOOL        CCmdExeModel::ExecuteCommand(CCmdExeCmd *pCmd)
{
	return TRUE;
}



BOOL        CCmdExeModel::StopExecution()
{
	m_bStopExecution = TRUE;
	return TRUE;
}

void        CCmdExeModel::SetInfiniteExecution(BOOL bInfiniteExecution)
{
	m_bInfiniteExecution = bInfiniteExecution;
}

BOOL        CCmdExeModel::GetInfiniteExecution()
{
	return m_bInfiniteExecution;
}

void        CCmdExeModel::SetExecutionLoopCount(int nExecutionLoopCount)
{
	m_nExecutionLoopCount = nExecutionLoopCount;
}

int         CCmdExeModel::GetExecutionLoopCount()
{
	return m_nExecutionLoopCount;
}

int         CCmdExeModel::GetCommandCount()
{
	return m_cmdVec.size();
}


BOOL		CCmdExeModel::LoadCommands(CMarkup& xmlReader)
{

	if(xmlReader.FindElem("CmdExeCommands") == false)
		return FALSE;

	xmlReader.IntoElem();

	while ( xmlReader.FindElem("CmdExeCmd") )
	{	
		CCmdExeCmd* pCmd = new CCmdExeCmd();

		if(pCmd->DeSerialize(xmlReader) == TRUE)
		{
			AddCommand(pCmd);
		}
		else
		{
			delete pCmd;
		}
	}
	xmlReader.OutOfElem();
	return TRUE;
}

BOOL		CCmdExeModel::SaveCommands(CMarkup& xmlWriter)
{
	xmlWriter.AddElem("CmdExeCommands");//root
	xmlWriter.IntoElem();

	vector<CCmdExeCmd*>::iterator  iter, endIter;
	iter    = m_cmdVec.begin();
	endIter = m_cmdVec.end();
	for(;iter != endIter ; iter++)
	{
		if((*iter)->Serialize(xmlWriter) == FALSE)
			return FALSE;
	}

	xmlWriter.OutOfElem();
	return TRUE;
}

BOOL		CCmdExeModel::Serialize(CString sFile)
{
	return TRUE;
}

BOOL		CCmdExeModel::DeSerialize(CString sFile)
{
	return TRUE;
}

void        CCmdExeModel::Log(CString sLogLine)
{
	NotifyLogLine(sLogLine);
}

BOOL CCmdExeModel::ExecuteCommands()
{
	int	 nCmd;

	//Reset stop Stop Execution flag
	m_bStopExecution = FALSE;

	do
	{
		NotifyLoopCountChange(0);
		for(int i = 0; (i < m_nExecutionLoopCount) || (m_bInfiniteExecution == TRUE); i++)
		{		
			CString sStatMsg;

			sStatMsg.Format("\n**** Start Execution#%d ****\n",i);
			Log(sStatMsg);
			vector<CCmdExeCmd*>::iterator  iter, endIter;

			iter    = m_cmdVec.begin();
			endIter = m_cmdVec.end();
			nCmd = 0;
			for(;iter != endIter ; iter++)
			{
				CCmdExeCmd  *pCmd;
				pCmd = m_cmdVec[nCmd];
				if(m_bStopExecution == TRUE)
				{
		    		sStatMsg.Format("\n****  Execution#%d Aborted ****\n",i);
					Log(sStatMsg);
					return TRUE;
				}

				int nCmdLoopCount;
				nCmdLoopCount = pCmd->GetLoopCount();
				for(int nCount = 0; nCount < nCmdLoopCount;nCount++)
				{				
					if((*iter)->Execute() == FALSE)
					{
	    				sStatMsg.Format("\n****  Execution#%d Ended With ERROR!!! ****\n",i);
						Log(sStatMsg);
						return FALSE;
					}
					if(m_bStopExecution == TRUE)
					{
		    			sStatMsg.Format("\n****  Execution#%d Aborted ****\n",i);
						Log(sStatMsg);
					    return TRUE;
					}
				}
				nCmd++;
			}
			sStatMsg.Format("\n****  End Execution#%d  ****\n",i);
			Log(sStatMsg);
			NotifyLoopCountChange(i);
		}


	}while((m_bInfiniteExecution == TRUE) && (m_bStopExecution == FALSE));

	return TRUE;
}