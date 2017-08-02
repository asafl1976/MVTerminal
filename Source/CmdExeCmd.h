#pragma once
#include <vector>
#include <map>
using namespace std;

#include "CmdExeItem.h"
#include "CmdExeCmdSettingsDlg.h"
#include "CmdExeCmd.h"

class CCmdExePane;

class CCmdExeCmd : public CCmdExeItem
{
	vector<CCmdExeItem*>    m_cmdItems;
	int					    m_nLoopCount;
	BOOL				    m_bStopExecution;
	
public:
	CCmdExeCmd();
	CCmdExeCmd(CCmdExeCmd& cmd);
	virtual ~CCmdExeCmd(void);
	virtual void   			SetLoopCount(int nLoopCount)      {m_nLoopCount = nLoopCount;}
	virtual int    			GetLoopCount()      			  {return m_nLoopCount;}

	virtual void	        RemoveItems();
	virtual void	        DeleteItems();
	virtual int    			GetCmdItemCount();
	virtual CCmdExeItem*    GetCmdItem(int nIndex);
	virtual void   			AddCmdItem(CCmdExeItem* pCmd);
	virtual BOOL   			DeleteItem(int nIndex);
	virtual BOOL   			DeleteItem(CCmdExeItem* pItem);
	virtual void			Log(CString sLogLine);

	virtual BOOL			Serialize(CMarkup & xmlWriter);
	virtual BOOL			DeSerialize(CMarkup & xmlReader);
	virtual BOOL            Execute();
	virtual CCmdExeItem*    Clone();
	virtual void            operator=(CCmdExeCmd & pCmd);


};

class CCmdExeViewClipboard
{
public:
	CCmdExeViewClipboard()
	{
		m_bEmpty = TRUE;
		m_pCmd     = NULL;
	}
	virtual ~CCmdExeViewClipboard()
	{
		map<HTREEITEM,TCHAR*>::iterator iter,endIter;
		iter    = m_pszTextMap.begin();
		endIter = m_pszTextMap.end();
		for(;iter != endIter; iter++)
		{
			if(iter->second != NULL)
			{
				delete iter->second;
			}
		}
	}
	map<HTREEITEM,TCHAR*>  m_pszTextMap;

	BOOL	               m_bEmpty;
	TVITEM				   m_tvItem;
	CCmdExeCmd   *m_pCmd; 
};