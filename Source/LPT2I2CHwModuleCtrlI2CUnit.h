#pragma once
#include "HwModuleCtrlI2CUnit.h"


class CLPT2I2CHwModuleCtrlI2CUnit : public CHwModuleCtrlI2CUnit
{
protected:

public:
	CLPT2I2CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv);
	virtual ~CLPT2I2CHwModuleCtrlI2CUnit(void);
};
