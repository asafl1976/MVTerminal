#pragma once
#include "HwModuleCtrlI2CUnit.h"
#include "HwModuleCtrl.h"

class CUSB2I2CHwModuleCtrlI2CUnit : public CHwModuleCtrlI2CUnit
{

public:

	CUSB2I2CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv);
	virtual ~CUSB2I2CHwModuleCtrlI2CUnit(void);

};
