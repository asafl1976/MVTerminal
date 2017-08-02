#pragma once
#include "MuxHwModule.h"
#include "HwModuleAccessStartegy.h"

class CMux9548Module : 	public CMuxHwModule
{

public:
	CMux9548Module(void);
	CMux9548Module(CHwModuleInfo* pInfo);
	virtual ~CMux9548Module(void);

	virtual CHwModule*     Clone(CHwModuleInfo* pInfo);
};
