#pragma once
#include "hwmodulectrladcunititem.h"

class CHwModuleCtrlADCUnitCurrentItem : public CHwModuleCtrlADCUnitItem
{
public:
	CHwModuleCtrlADCUnitCurrentItem(void);
	virtual ~CHwModuleCtrlADCUnitCurrentItem(void);

	virtual BOOL     GetCurrent(float nResistorVal, int *pCurrent) = 0;

};
