#pragma once
#include "TemperatureSensorHwModuleAccessStartegy.h"

class CHwModuleCtrlI2CUnitDrv;
class CHwModule;

class CMax6699TempSensorHwModuleAccessStartegy : 	public CTemperatureSensorHwModuleAccessStartegy
{ 
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;

public:
	virtual BOOL  GetTemperature(int* pTemperature);

	CMax6699TempSensorHwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CMax6699TempSensorHwModuleAccessStartegy(void);
};



