#pragma once

#include "HwModuleItem.h"

#include <map>
using namespace std;

class CHwModuleItem;
class CBasicItemDlg;

class CHwModuleItemDlgFactory
{
	map<CString,CBasicItemDlg*>   m_hwModuleItemDlg;
	CHwModuleItemDlgFactory(void);

public:
	static   CHwModuleItemDlgFactory& GetInstance();
	virtual ~CHwModuleItemDlgFactory(void);
	virtual  CBasicItemDlg*      Create(CHwModuleItem* pitem);
};
