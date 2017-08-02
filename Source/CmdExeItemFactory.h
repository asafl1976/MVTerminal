#pragma once
#include <map>
using namespace std;
#include "CmdExeCmdItem.h"

class CCmdExeItemContainer;

class CCmdExeItemFactory
{
	map<CString,CCmdExeCmdItem*>  m_cmdItems;
	CCmdExeItemFactory(void);

public:

	virtual ~CCmdExeItemFactory(void);
	static  CCmdExeItemFactory& GetInstance();
	virtual CCmdExeItem* CreateItem(CString sName);
	virtual CCmdExeItemContainer* CreateItemContainer(CString sName);
	
};

