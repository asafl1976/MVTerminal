#pragma once
#include "cmdexecmditem.h"

class CCmdExeCmdItemBoardReset : public CCmdExeCmdItem
{
public:
	CCmdExeCmdItemBoardReset(void);
	virtual ~CCmdExeCmdItemBoardReset(void);

	virtual BOOL	     Serialize(CMarkup & xmlWriter);
	virtual BOOL	     DeSerialize(CMarkup & xmlReader);

	virtual BOOL         Execute();
	virtual CCmdExeItem* Clone();

};
