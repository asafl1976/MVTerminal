#pragma once
#include "CmdExeCmd.h"
#include "Markup.h"
#include "ICmdExeModelSubject.h"

class CCmdExeModel : public ICmdExeModelSubject
{
	vector<CCmdExeCmd*> m_cmdVec;
	BOOL				m_bInfiniteExecution;
	BOOL				m_bStopExecution;
	int 				m_nExecutionLoopCount;

	virtual BOOL        IsCommandNameExists(CString sCmdName);
	CCmdExeModel(void);
public:
	virtual ~CCmdExeModel(void);
	static  CCmdExeModel&       GetInstance();

	virtual BOOL        UpdateCommand(CCmdExeCmd *pCmd);
	virtual BOOL        AddCommand(CCmdExeCmd *pCmd);
	virtual BOOL        RemoveCommand(CCmdExeCmd *pCmd);
	virtual BOOL        RemoveCommand(CString sCmdName);
	virtual BOOL        RemoveAllCommands();
	virtual CCmdExeCmd* GetCommand(CString sCmdName);
	virtual BOOL        ExecuteCommand(CCmdExeCmd *pCmd);
	virtual BOOL        ExecuteCommands();
	virtual BOOL        StopExecution();
	virtual int         GetCommandCount();
	virtual void        SetInfiniteExecution(BOOL bInfiniteExecution);
	virtual BOOL        GetInfiniteExecution();

	virtual void        SetExecutionLoopCount(int bExecutionLoopCount);
	virtual int         GetExecutionLoopCount();
	virtual void        Log(CString sLogLine);

	virtual BOOL		LoadCommands(CMarkup& xmlReader);
	virtual BOOL		SaveCommands(CMarkup& xmlWriter);

	virtual BOOL		Serialize(CString sFile);
	virtual BOOL		DeSerialize(CString sFile);

};
