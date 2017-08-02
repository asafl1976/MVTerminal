#pragma once
#include "cmdexecmditem.h"

class CCmdExeCmdItemBoardPowerOff :
	public CCmdExeCmdItem
{
public:
	CCmdExeCmdItemBoardPowerOff(void);
	virtual ~CCmdExeCmdItemBoardPowerOff(void);

	virtual BOOL	     Serialize(CMarkup & xmlWriter);
	virtual BOOL	     DeSerialize(CMarkup & xmlReader);

	virtual BOOL         Execute();
	virtual CCmdExeItem* Clone();

};
