#pragma once
#include "hwmoduleaccessstartegy.h"

class CHwModuleCtrlI2CUnitDrv;

class CMarvellDeviceHwModuleI2CAccessStartegy : public CHwModuleAccessStartegy
{
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;

public:
	CMarvellDeviceHwModuleI2CAccessStartegy(CHwModule *pHwModule,CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CMarvellDeviceHwModuleI2CAccessStartegy(void);

	virtual BOOL ReadRegister(UINT32 nRegAddress, UINT32 *pRegValue);
	virtual BOOL WriteRegister(UINT32 nRegAddress,UINT32 nRegValue);
};
