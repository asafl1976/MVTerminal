#pragma once


class CCmdExeCmd;

class ICmdExeModelObserver
{
public:
	ICmdExeModelObserver(void);
	virtual ~ICmdExeModelObserver(void);

	
	virtual BOOL OnModelLogLine(CString sLogLine)            {return TRUE;}
	virtual BOOL OnModelUpdateCommand(CCmdExeCmd *pCommand)  {return TRUE;}
	virtual BOOL OnModelRemoveCommad(CCmdExeCmd *pCommand)   {return TRUE;} 
	virtual BOOL OnModelLoopCountChange(int nLoopCount)		 {return TRUE;} 
	virtual BOOL OnViewRemoveAllCommands()                   {return TRUE;} 
	virtual BOOL OnModelAddCommand(CCmdExeCmd *pCommand)     {return TRUE;}

};
