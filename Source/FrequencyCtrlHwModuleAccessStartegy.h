#pragma once
#include "hwmoduleaccessstartegy.h"

class CFrequencyCtrlHwModuleAccessStartegy : public CHwModuleAccessStartegy
{
public:

	virtual BOOL	SetFrequency(long nFreq, long nRef, long nRange) = 0;

	CFrequencyCtrlHwModuleAccessStartegy(CHwModule *pHwModule);
	virtual ~CFrequencyCtrlHwModuleAccessStartegy(void);
};
