#pragma once
#include "cmdexecmditem.h"

class CCmdExeCmdItemBoardPowerOn :
	public CCmdExeCmdItem
{
public:
	CCmdExeCmdItemBoardPowerOn(void);
	virtual ~CCmdExeCmdItemBoardPowerOn(void);

	virtual BOOL	     Serialize(CMarkup & xmlWriter);
	virtual BOOL	     DeSerialize(CMarkup & xmlReader);

	virtual BOOL         Execute();
	virtual CCmdExeItem* Clone();

};
