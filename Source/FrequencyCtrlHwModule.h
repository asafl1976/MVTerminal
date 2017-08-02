#pragma once
#include "hwmodule.h"

class CFrequencyCtrlHwModule : public CHwModule
{
public:
	CFrequencyCtrlHwModule(void);
	CFrequencyCtrlHwModule(CHwModuleInfo* pInfo);
	virtual ~CFrequencyCtrlHwModule(void);

	virtual BOOL    SetFrequancy(long nFreq, long nRef, long nRange) = 0;

};
