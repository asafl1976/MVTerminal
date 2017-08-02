#pragma once
#include "hwmoduleboardpowerctrlstartegy.h"

class CHwModuleBoardPowerCtrlPowerStartegy : public CHwModuleBoardPowerCtrlStartegy
{
public:

	CHwModuleBoardPowerCtrlPowerStartegy();
	virtual ~CHwModuleBoardPowerCtrlPowerStartegy(void);

	virtual BOOL PowerOn()	 = 0;
	virtual BOOL PowerOff()  = 0;
};
