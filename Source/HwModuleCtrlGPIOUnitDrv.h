#pragma once
#include "hwmodulectrlunitdrv.h"


class CHwModuleCtrlGPIOUnitDrv : public CHwModuleCtrlUnitDrv
{
public:
	CHwModuleCtrlGPIOUnitDrv(void);
	virtual ~CHwModuleCtrlGPIOUnitDrv(void);

	virtual BOOL GetGPIOState(int nIndex, int& nStatus) = 0;
	virtual BOOL SetGPIOState(int nIndex, int  nStatus) = 0;

	virtual BOOL GetGPIOType(int nIndex, int& nType) = 0;
	virtual BOOL SetGPIOType(int nIndex, int  nType) = 0;
};
