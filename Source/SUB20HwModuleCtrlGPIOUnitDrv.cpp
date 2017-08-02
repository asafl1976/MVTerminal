#include "StdAfx.h"
#include ".\sub20hwmodulectrlgpiounitdrv.h"

CSUB20HwModuleCtrlGPIOUnitDrv::CSUB20HwModuleCtrlGPIOUnitDrv(CSUB20HwModuleCtrlDrv *pDrv)
{
	m_pDrv = pDrv;
}

CSUB20HwModuleCtrlGPIOUnitDrv::~CSUB20HwModuleCtrlGPIOUnitDrv(void)
{
}

BOOL  CSUB20HwModuleCtrlGPIOUnitDrv::GetGPIOState(int nIndex, int& nState)
{
	return m_pDrv->GetGPIOState(nIndex,nState);
}

BOOL  CSUB20HwModuleCtrlGPIOUnitDrv::SetGPIOState(int nIndex, int  nState)
{
	return m_pDrv->SetGPIOState(nIndex,nState);
}

BOOL  CSUB20HwModuleCtrlGPIOUnitDrv::SetGPIOType(int nIndex, int nType)
{
	return m_pDrv->SetGPIOType(nIndex,nType);
}

BOOL  CSUB20HwModuleCtrlGPIOUnitDrv::GetGPIOType(int nIndex, int& nType)
{
	return m_pDrv->GetGPIOType(nIndex,nType);
}
