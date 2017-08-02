#pragma once

#include "HwModule.h"

#include <map>
using namespace std;



class CHwModuleFactory
{
	map<CString,CHwModule*>  m_hwModules;
	CHwModuleFactory(void);

public:
	static CHwModuleFactory& GetInstance();
	virtual ~CHwModuleFactory(void);
	virtual CHwModule* Create(CHwModuleInfo* pInfo);
	virtual int			      GetPrototypeCount();
	virtual CString  	      GetPrototypeName(int i);
};
