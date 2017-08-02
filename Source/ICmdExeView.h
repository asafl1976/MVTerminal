#pragma once

class CCmdExeCmd;

class ICmdExeView
{
public:
	ICmdExeView(void);
	virtual ~ICmdExeView(void);

	//Triggered by presenter
	virtual BOOL OnAddCommand(CCmdExeCmd *pCommand)      = 0;
	virtual BOOL OnRemoveCommand(CCmdExeCmd *pCommand)   = 0;

	//Triggered by user
	virtual BOOL RemoveCommand(CCmdExeCmd *pCommand) = 0;
	virtual BOOL AddCommand(CCmdExeCmd *pCommand)    = 0;


};
