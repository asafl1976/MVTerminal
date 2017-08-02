#pragma once
#include "TemperatureSensorHwModule.h"

class CTemperatureSensorHwModuleAccessStartegy;

class CMax6699Params
{
public:
	BOOL	m_bWrite;
	int  	m_nTemperature;

	CMax6699Params()
	{
		m_bWrite        = TRUE;
		m_nTemperature  = 0;
	}
};

class CMax6699TempSensorHwModule : public CTemperatureSensorHwModule
{
	CTemperatureSensorHwModuleAccessStartegy* m_pAccessStrategy;
	int m_nTemperature;
public:
	CMax6699TempSensorHwModule(void);
	CMax6699TempSensorHwModule(CHwModuleInfo* pInfo);
	virtual ~CMax6699TempSensorHwModule(void);

	virtual int			 GetTemperature(int* pTemperature);

	virtual  CString	 Serialize();
	virtual  BOOL   	 DeSerialize(CMarkup& xmlReader);

	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);
};

