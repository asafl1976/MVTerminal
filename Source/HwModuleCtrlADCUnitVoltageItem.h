#pragma once
#include "HwModuleCtrlADCUnitItem.h"

class CHwModuleCtrlADCUnitVoltageItem : public CHwModuleCtrlADCUnitItem
{
public:
	CHwModuleCtrlADCUnitVoltageItem(void);
	virtual ~CHwModuleCtrlADCUnitVoltageItem(void);

	virtual BOOL GetVoltage(int *pVoltage) = 0;
};
