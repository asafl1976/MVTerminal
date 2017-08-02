#pragma once
#include "hwmoduleaccessstartegy.h"

class CVoltageCtrlHwModuleAccessStartegy : 	public CHwModuleAccessStartegy
{
public:
	virtual BOOL  	SetVoltage(int nVoltage)  = 0;
	virtual BOOL 	GetVoltage(int* pVoltage) = 0;

	CVoltageCtrlHwModuleAccessStartegy(CHwModule *pHwModule);
	virtual ~CVoltageCtrlHwModuleAccessStartegy(void);
};
