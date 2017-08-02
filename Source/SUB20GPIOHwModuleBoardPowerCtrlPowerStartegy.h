#pragma once
#include "HwModuleBoardPowerCtrlPowerStartegy.h"

class CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy :
	public CHwModuleBoardPowerCtrlPowerStartegy
{
public:
	CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy();
	virtual ~CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy(void);

	virtual BOOL PowerOn();
	virtual BOOL PowerOff();
	virtual CHwModuleBoardPowerCtrlStartegy* Clone();


};
