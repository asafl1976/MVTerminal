#include "StdAfx.h"
#include ".\sub20hwmodulectrladcunitdrv.h"

CSUB20HwModuleCtrlADCUnitDrv::CSUB20HwModuleCtrlADCUnitDrv(CSUB20HwModuleCtrlDrv *pDrv)
{
	m_pDrv = pDrv;
}

CSUB20HwModuleCtrlADCUnitDrv::~CSUB20HwModuleCtrlADCUnitDrv(void)
{
}

BOOL CSUB20HwModuleCtrlADCUnitDrv::Init()
{
	return m_pDrv->ADCInit();
}

BOOL  CSUB20HwModuleCtrlADCUnitDrv::GetVoltage(int nIndex, int* pVoltage)
{
	return m_pDrv->GetVoltage(nIndex,pVoltage);
}

BOOL  CSUB20HwModuleCtrlADCUnitDrv::GetCurrent(int nIndex,float nResistorVal, int* pCurrent)
{
	return m_pDrv->GetCurrent(nIndex,nResistorVal,pCurrent);
}
