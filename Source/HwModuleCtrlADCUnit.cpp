#include "StdAfx.h"
#include ".\hwmodulectrladcunit.h"
#include "HwModuleModel.h"

CHwModuleCtrlADCUnit::CHwModuleCtrlADCUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlADCUnitDrv* pADCUnitDrv):
CHwModuleCtrlUnit(pHwModuleCtrl)
{
	m_pADCUnitDrv  = pADCUnitDrv;
}

CHwModuleCtrlADCUnit::~CHwModuleCtrlADCUnit(void)
{

}
