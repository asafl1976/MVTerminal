#include "StdAfx.h"
#include ".\hwmoduleboardpowerctrlstartegy.h"

CHwModuleBoardPowerCtrlStartegy::CHwModuleBoardPowerCtrlStartegy()
{
	m_pHwModuleItem = NULL;
	m_nIO = 0;
}

CHwModuleBoardPowerCtrlStartegy::~CHwModuleBoardPowerCtrlStartegy(void)
{
}

void CHwModuleBoardPowerCtrlStartegy::SetIO(int nIO)
{
	m_nIO = nIO;
}

void CHwModuleBoardPowerCtrlStartegy::SetIOInitialState(CString sIOInitialState)
{
	m_sIOInitialState = sIOInitialState;
}

void CHwModuleBoardPowerCtrlStartegy::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pHwModuleItem = pItem;
}


