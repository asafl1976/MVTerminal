#pragma once
#include "hwmodulectrladcunitdrv.h"
#include "SUB20HwModuleCtrlDrv.h"

class CSUB20HwModuleCtrlADCUnitDrv : public CHwModuleCtrlADCUnitDrv
{

	CSUB20HwModuleCtrlDrv *m_pDrv;

public:
	CSUB20HwModuleCtrlADCUnitDrv(CSUB20HwModuleCtrlDrv *pDrv);
	virtual ~CSUB20HwModuleCtrlADCUnitDrv(void);
	virtual BOOL Init();
	virtual BOOL  GetVoltage(int nIndex, int *pVoltage);
	virtual BOOL  GetCurrent(int nIndex,float nResistorVal, int *pCurrent);
};
