#pragma once

#include "CmdExeItem.h"
#include "CmdExeCmdItem.h"
#include "BasicItemDlg.h"

#include <map>
using namespace std;

class CCmdExeItem;

class CCmdExeItemDlgFactory
{
	map<CString,CBasicItemDlg*>   m_cmdExeCmdItemDlg;
	CCmdExeItemDlgFactory(void);

public:
	static   CCmdExeItemDlgFactory& GetInstance();
	virtual ~CCmdExeItemDlgFactory(void);
	virtual  CBasicItemDlg*      Create(CCmdExeCmdItem* pItem);
};
