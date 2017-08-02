#pragma once
#include "CmdExeItem.h"

class CCmdExeCmd;

class CCmdExeCmdItem : public CCmdExeItem
{	
protected:
	CCmdExeCmd* 		    m_pOwnerCommand;

public:

	CCmdExeCmdItem(void);
	virtual ~CCmdExeCmdItem(void);

	virtual CCmdExeCmd*	    GetOwnerCommand()                  {return m_pOwnerCommand;}
	virtual void       		SetOwnerCommand(CCmdExeCmd* pCmd)  {m_pOwnerCommand = pCmd;}
	virtual void	   	    Log(CString sLogLine);

	virtual BOOL	        Serialize(CMarkup & xmlWriter);
	virtual BOOL	        DeSerialize(CMarkup & xmlReader);

};
