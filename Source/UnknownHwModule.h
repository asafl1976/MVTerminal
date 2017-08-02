#pragma once
#include "HwModule.h"



class CUnknownHwModule : public CHwModule
{

public:
	CUnknownHwModule();
	CUnknownHwModule(CHwModuleInfo* pInfo);
    virtual ~CUnknownHwModule();
};


