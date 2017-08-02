#include "StdAfx.h"
#include ".\Max6699TempSensorHwModuleAccessStartegy.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "HwModule.h"
#include "Max6699TempSensorHwModule.h"

CMax6699TempSensorHwModuleAccessStartegy::CMax6699TempSensorHwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CTemperatureSensorHwModuleAccessStartegy(pHwModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
}

CMax6699TempSensorHwModuleAccessStartegy::~CMax6699TempSensorHwModuleAccessStartegy(void)
{

}

BOOL  CMax6699TempSensorHwModuleAccessStartegy::GetTemperature(int* pTemperature)
{

	return m_pI2CUnitDrv->ReadData(m_nHwModuleID,1,1,(UCHAR*)pTemperature);
}