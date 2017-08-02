#include "StdAfx.h"
#include ".\hwmodulectrlgpiounit.h"

CHwModuleCtrlGPIOUnit::CHwModuleCtrlGPIOUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlGPIOUnitDrv* pGPIOUnitDrv)
:CHwModuleCtrlUnit(pHwModuleCtrl)
{
	m_pGPIOUnitDrv = pGPIOUnitDrv;
}

CHwModuleCtrlGPIOUnit::~CHwModuleCtrlGPIOUnit(void)
{
}

BOOL CHwModuleCtrlGPIOUnit::GetGPIOState(int nIndex, int& nStatus)
{
	return m_pGPIOUnitDrv->GetGPIOState(nIndex,nStatus);
}

BOOL CHwModuleCtrlGPIOUnit::SetGPIOState(int nIndex, int  nStatus)
{
	return m_pGPIOUnitDrv->SetGPIOState(nIndex,nStatus);
}

BOOL CHwModuleCtrlGPIOUnit::GetGPIOType(int nIndex, int& nType)
{
	return m_pGPIOUnitDrv->GetGPIOType(nIndex,nType);
}

BOOL CHwModuleCtrlGPIOUnit::SetGPIOType(int nIndex, int  nType)
{
	return m_pGPIOUnitDrv->SetGPIOType(nIndex,nType);
}
