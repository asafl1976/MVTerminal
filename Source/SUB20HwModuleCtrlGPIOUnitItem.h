#pragma once
#include "HwModuleCtrlGPIOUnitItem.h"

class CSUB20HwModuleCtrlGPIOUnit;

class CSUB20HwModuleCtrlGPIOUnitItem : 	public CHwModuleCtrlGPIOUnitItem
{
protected:
	CSUB20HwModuleCtrlGPIOUnit* m_pUnit;

public:
	CSUB20HwModuleCtrlGPIOUnitItem(CSUB20HwModuleCtrlGPIOUnit *pUnit);
	virtual ~CSUB20HwModuleCtrlGPIOUnitItem(void);
};
