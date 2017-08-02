#pragma once
#include "hwmodulectrlunitdrv.h"

class CHwModuleCtrlADCUnitDrv : public CHwModuleCtrlUnitDrv
{
public:
	CHwModuleCtrlADCUnitDrv(void);
	virtual ~CHwModuleCtrlADCUnitDrv(void);
	virtual BOOL GetVoltage(int nIndex, int *pVolatge) = 0;
	virtual BOOL GetCurrent(int nIndex,float nResistorVal, int *pCurrent) = 0;
};
