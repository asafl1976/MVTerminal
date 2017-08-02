#pragma once
#include "hwmodulectrlunit.h"
#include "HwModuleCtrlGPIOUnitDrv.h"

class CHwModuleCtrlGPIOUnit : public CHwModuleCtrlUnit
{
protected:
	CHwModuleCtrlGPIOUnitDrv*     m_pGPIOUnitDrv;

public:
	CHwModuleCtrlGPIOUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlGPIOUnitDrv* pGPIOUnitDrv);
	virtual ~CHwModuleCtrlGPIOUnit(void);
	virtual CHwModuleCtrlUnitDrv*   GetDriver()	{return m_pGPIOUnitDrv;}

	virtual BOOL GetGPIOState(int nIndex, int& nStatus);
	virtual BOOL SetGPIOState(int nIndex, int  nStatus);

	virtual BOOL GetGPIOType(int nIndex, int& nType);
	virtual BOOL SetGPIOType(int nIndex, int  nType);
};
