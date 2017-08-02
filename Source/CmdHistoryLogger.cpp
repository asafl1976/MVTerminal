#include "StdAfx.h"
#include ".\cmdhistorylogger.h"

#define  VK_CTRL_C						TCHAR(3)
#define  VK_CTRL_V						TCHAR(22)


CCmdHistoryLogger::CCmdHistoryLogger(void)
{
	m_nMaxCommandCount     = 30;
	m_nCommandMinLenThreshold = 2;
	m_nCommandMaxLenThreshold = 64;
	m_nCmdIndex		       = -1;
    m_bWasAtGetPrev        = false;
	m_bWasAtGetNext        = false;
	SetEnable(TRUE); 
}

CCmdHistoryLogger::~CCmdHistoryLogger(void)
{
}

void     CCmdHistoryLogger::Log(CString sCh)
{
	m_sCommand += sCh;
}

void     CCmdHistoryLogger::EndLog()
{
	AddCommand();
	m_sCommand.Empty();
}

BOOL   CCmdHistoryLogger::IsCmdExist(CString sCmd)
{
	list<CString>::iterator  iter, endIter = m_cmdList.end();

	for(iter = m_cmdList.begin(); iter != endIter; iter++)
	{
		if((*iter) == sCmd)
			return TRUE;
	}
	return FALSE;	
}

void   CCmdHistoryLogger::DeleteCommand(CString sCmd)
{
	list<CString>::iterator  iter;

	for(iter = m_cmdList.begin(); iter != m_cmdList.end(); iter++)
	{
		if((*iter) == sCmd)
		{
			m_cmdList.erase(iter);
			return;
		}
	}
}


void     CCmdHistoryLogger::Release()
{
	m_sCommand.Empty();
}

void	 CCmdHistoryLogger::GetInput(CString sCh)
{
	if(m_bEnable == FALSE)
		return;

	switch(sCh.GetAt(0))
	{
	case '\b':
		if(m_sCommand.IsEmpty() == FALSE)
		{
			m_sCommand = m_sCommand.Left(m_sCommand.GetLength() - 1);
		}
		break;
	case '\r':
		if(m_sCommand.IsEmpty() == FALSE)
		{
			EndLog();
		}
		break;
	case VK_CTRL_C:
	case VK_CTRL_V:
		break;
	default:
		Log(sCh);
		break;
	}
}

void	CCmdHistoryLogger::AddCommand()
{
	AddCommand(m_sCommand);
}

void	CCmdHistoryLogger::AddCommand(CString sCmd)
{
	if(Filter(sCmd) == TRUE)
		return;

	if((sCmd.IsEmpty() == FALSE) && 
		(sCmd.GetLength() > GetCommandMinLenThreshold()) && 
		(sCmd.GetLength() < GetCommandMaxLenThreshold()))
	{
		m_cmdList.push_front(sCmd);
		m_nCmdIndex = -1;
		m_bWasAtGetNext = false;
		m_bWasAtGetPrev = false;
	}
	if(GetCommandCount() > m_nMaxCommandCount)
	{
		list<CString>::iterator lastItem = m_cmdList.end();
		lastItem--;
		m_cmdList.erase(lastItem);
	}
}
int	CCmdHistoryLogger::GetCommandCount()
{
	return (int)m_cmdList.size();
}

CString	 CCmdHistoryLogger::GetCurrentCommand()
{
	return m_sCommand;
}


CString	CCmdHistoryLogger::GetCommandByIndex(int nCmd)
{
	list<CString>::iterator  iter, endIter = m_cmdList.end();
	int			nIndex = 0;

	for(iter = m_cmdList.begin(); iter != endIter; iter++)
	{
		if(nIndex == nCmd)
		{
			return (*iter);
		}
		nIndex++;
	}
	return CString("");
}

CString	CCmdHistoryLogger::GetCommandByDirection(int nDirection)
{
	CString sCmd;
	int     nCmdCount = GetCommandCount();

	switch(nDirection)
	{
	case PREVIOUS_COMMAND:
		if(m_nCmdIndex > (nCmdCount-1))
		{
			if(m_bWasAtGetNext == true)
				m_nCmdIndex = (nCmdCount-1) - 1;
			else
				m_nCmdIndex = (nCmdCount-1);	
		}
		else
		{
			if(m_nCmdIndex > -1)
			{
				m_nCmdIndex--;
				m_bWasAtGetNext = false;
				m_bWasAtGetPrev = true;
			}
		}
		break;
	case NEXT_COMMAND:
		if(m_nCmdIndex == -1)
		{
			if(m_bWasAtGetPrev == true)
				m_nCmdIndex = 1;
			else
				m_nCmdIndex = 0;
		}
		else
		{
			if(m_nCmdIndex <= (nCmdCount-1))
			{
				m_nCmdIndex++;
				m_bWasAtGetNext = true;
				m_bWasAtGetPrev = false;
			}
		}
		break;
	default:
		return sCmd;
	}
	sCmd = GetCommandByIndex(m_nCmdIndex);
	if(sCmd.IsEmpty() == FALSE)
	{
		for(int nBackspace = 0; nBackspace < m_sLastCmd.GetLength(); nBackspace++)
			sCmd.Insert(0,VK_BACK);
		m_sLastCmd = sCmd;
	}
	return sCmd;
}

BOOL	CCmdHistoryLogger::Filter(CString sCmd)
{
	if(m_sFilter.Left(sCmd.GetLength() + 1) == (sCmd + ";"))
		return TRUE;

	if(m_sFilter.Find(";" + sCmd + ";") != -1)
		return TRUE;

	return FALSE;
}


