#pragma once
#include "hwmodule.h"
#include "MarvellDeviceHwModuleI2CAccessStartegy.h"


class CMarvellDeviceHwModuleParams
{
public:
	BOOL	m_bRead;
	UINT32 	m_nValue;
	int 	m_nOffset;

	CMarvellDeviceHwModuleParams()
	{
		m_bRead = TRUE;
		m_nValue = 0;
		m_nOffset = 0;
	}
};

class CMarvellDeviceHwModule : 	public CHwModule
{
	CMarvellDeviceHwModuleI2CAccessStartegy* m_pAccessStrategy;

public:
	CMarvellDeviceHwModule(void);
	CMarvellDeviceHwModule(CHwModuleInfo* pInfo);
	virtual ~CMarvellDeviceHwModule(void);

	virtual BOOL ReadRegister(UINT32 nRegAddress, UINT32 *pRegVal);
	virtual BOOL WriteRegister(UINT32 nRegAddress,UINT32 nVal);

	virtual  CString	 Serialize();
	virtual  BOOL   	 DeSerialize(CMarkup& xmlReader);

	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);

};
