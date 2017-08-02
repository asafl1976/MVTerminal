#pragma once
#include "hwmoduleitem.h"

class CHwModuleCtrlUnitItem : public CHwModuleItem
{
protected:
	CString m_sDisplayNameSuffix;

public:
	CHwModuleCtrlUnitItem(void);
	virtual ~CHwModuleCtrlUnitItem(void);
	virtual CString GetDisplayNameSuffix() {return m_sDisplayNameSuffix;}
	virtual void SetDisplayName(CString sDisplayName);
};
