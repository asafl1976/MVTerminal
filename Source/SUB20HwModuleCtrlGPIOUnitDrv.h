#pragma once
#include "hwmodulectrlgpiounitdrv.h"
#include "SUB20HwModuleCtrlDrv.h"



class CSUB20HwModuleCtrlGPIOUnitDrv : public CHwModuleCtrlGPIOUnitDrv
{
	CSUB20HwModuleCtrlDrv *m_pDrv;

public:
	CSUB20HwModuleCtrlGPIOUnitDrv(CSUB20HwModuleCtrlDrv *pDrv);
	virtual ~CSUB20HwModuleCtrlGPIOUnitDrv(void);

	virtual BOOL GetGPIOState(int nIndex, int& nState);
	virtual BOOL SetGPIOState(int nIndex, int  nState);

	virtual BOOL GetGPIOType(int nIndex, int& nType);
	virtual BOOL SetGPIOType(int nIndex, int  nType);
};
