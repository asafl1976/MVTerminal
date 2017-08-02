#pragma once
#include "hwmoduleboardpowerctrlresetstartegy.h"

class CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy :
	public CHwModuleBoardPowerCtrlResetStartegy
{
public:
	CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy(void);
	virtual ~CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy(void);
	virtual BOOL Reset(int nPulseLength);

};
