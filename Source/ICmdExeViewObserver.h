#pragma once
#include "Markup.h"

class CCmdExeCmd;

class ICmdExeViewObserver
{
public:
	ICmdExeViewObserver(void);
	virtual ~ICmdExeViewObserver(void);

	
	virtual BOOL     OnViewLoadCommands(CMarkup& xmlReader)     {return TRUE;} 
	virtual BOOL     OnViewSaveCommands(CMarkup& xmlWriter)     {return TRUE;} 
	virtual BOOL     OnViewNotifyRemoveAllCommands()             {return TRUE;} 
	virtual BOOL     OnViewRemoveCommand(CCmdExeCmd *pCommand)   {return TRUE;} 
	virtual BOOL     OnViewRemoveAllCommands()                   {return TRUE;} 
	virtual BOOL     OnViewAddCommand(CCmdExeCmd *pCommand)      {return TRUE;}
	virtual BOOL     OnViewUpdateCommand(CCmdExeCmd *pCommand)   {return TRUE;}
	

};
