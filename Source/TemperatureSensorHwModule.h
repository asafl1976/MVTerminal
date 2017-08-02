#pragma once
#include "hwmodule.h"

class CTemperatureSensorHwModule : 	public CHwModule
{
public:
	CTemperatureSensorHwModule(void);
	CTemperatureSensorHwModule(CHwModuleInfo* pInfo);

	virtual ~CTemperatureSensorHwModule(void);

	virtual BOOL  GetTemperature(int* pTemperature) = 0;

};
