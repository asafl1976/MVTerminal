#pragma once
#include "cmdexecmditem.h"
#include "Markup.h"

class CHwModuleItem;

class CHwModuleItemCmdExeCmdItem : 	public CCmdExeCmdItem
{
protected:

	CHwModuleItem*		     m_pHwModuleItem;

public:
	CHwModuleItemCmdExeCmdItem(void);
	virtual ~CHwModuleItemCmdExeCmdItem(void);

	virtual BOOL			 SerializeHwModuleItem(CMarkup& xmlWriter);
	virtual BOOL			 DeSerializeHwModuleItem(CMarkup& xmlReader);

	virtual BOOL			 OnError(CString sErrorMsg);

	virtual BOOL			 OnStartExecution()                            = 0;
	virtual BOOL			 OnEndExecution()                              = 0;
	virtual void			 SetHwModuleItem(CHwModuleItem* pHwModuleItem) = 0;
	virtual CHwModuleItem*   GetHwModuleItem()	{return m_pHwModuleItem;}

};
