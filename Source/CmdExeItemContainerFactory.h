#pragma once
#include <map>
using namespace std;

class CCmdExeItemContainer;

class CCmdExeItemContainerFactory
{
	map<CString,CCmdExeItemContainer*>  m_containers;
	CCmdExeItemContainerFactory(void);

public:
	virtual ~CCmdExeItemContainerFactory(void);
	static  CCmdExeItemContainerFactory& GetInstance();
	virtual CCmdExeItemContainer* Create(CString sName);
};
