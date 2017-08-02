#pragma once
#include "HwModuleCtrlI2CUnit.h"
#include "HwModuleCtrl.h"

class CSUB20HwModuleCtrlI2CUnit : public CHwModuleCtrlI2CUnit
{

public:
	CSUB20HwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv);
	virtual BOOL Init();
	virtual ~CSUB20HwModuleCtrlI2CUnit(void);
};
