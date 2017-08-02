#pragma once
#include "hwmodulectrlunit.h"
#include "HwModuleCtrlADCUnitDrv.h"


class CHwModuleCtrlADCUnit : public CHwModuleCtrlUnit
{
protected:
	CHwModuleCtrlADCUnitDrv*     m_pADCUnitDrv;

public:
	CHwModuleCtrlADCUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlADCUnitDrv* pADCUnitDrv);
	virtual ~CHwModuleCtrlADCUnit(void);
	virtual CHwModuleCtrlUnitDrv*   GetDriver(){return m_pADCUnitDrv;}

};
 