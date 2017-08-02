#pragma once
#include "hwmoduleboardpowerctrlstartegy.h"

class CHwModuleBoardPowerCtrlResetStartegy :
	public CHwModuleBoardPowerCtrlStartegy
{
public:
	CHwModuleBoardPowerCtrlResetStartegy();
	virtual ~CHwModuleBoardPowerCtrlResetStartegy(void);

	virtual BOOL Reset(int nPulseLength) = 0;
};
