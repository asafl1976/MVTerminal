#pragma once
#include "MuxHwModule.h"
#include "HwModuleAccessStartegy.h"

class CMux9544Module : 	public CMuxHwModule
{

public:
	CMux9544Module(void);
	CMux9544Module(CHwModuleInfo* pInfo);
	virtual ~CMux9544Module(void);

	virtual CHwModule*     Clone(CHwModuleInfo* pInfo);
};
