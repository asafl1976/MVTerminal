#pragma once
#include "hwmoduleaccessstartegy.h"

class CTemperatureSensorHwModuleAccessStartegy : 	public CHwModuleAccessStartegy
{
public:
	virtual BOOL  GetTemperature(int* pTemperature) = 0;

	CTemperatureSensorHwModuleAccessStartegy(CHwModule *pHwModule);
	virtual ~CTemperatureSensorHwModuleAccessStartegy(void);
};
