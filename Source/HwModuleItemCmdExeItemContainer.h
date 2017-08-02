#pragma once
#include "cmdexeitemcontainer.h"

class CHwModuleItem;

class CHwModuleItemCmdExeItemContainer : public CCmdExeItemContainer
{
public:
	CHwModuleItemCmdExeItemContainer(CString sName);
	virtual ~CHwModuleItemCmdExeItemContainer(void);

	virtual CCmdExeItemContainer* Clone();
	virtual void				  SetHwModuleItem(CHwModuleItem* pHwModuleItem);

};
