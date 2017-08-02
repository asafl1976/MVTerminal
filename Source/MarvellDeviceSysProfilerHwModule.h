#pragma once
#include "hwmodule.h"
#include "MarvellDeviceHwModuleI2CAccessStartegy.h"


class CMarvellDeviceSysProfilerHwModule : 	public CHwModule
{
	CMarvellDeviceHwModuleI2CAccessStartegy* m_pAccessStrategy;

public:
	CMarvellDeviceSysProfilerHwModule(void);
	CMarvellDeviceSysProfilerHwModule(CHwModuleInfo* pInfo);
	virtual ~CMarvellDeviceSysProfilerHwModule(void);

	virtual BOOL ReadRegister(UINT32 nRegAddress, UINT32 *pRegVal);
	virtual BOOL WriteRegister(UINT32 nRegAddress,UINT32 nVal);

	virtual  CString	 Serialize();
	virtual  BOOL   	 DeSerialize(CMarkup& xmlReader);

	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);

};
