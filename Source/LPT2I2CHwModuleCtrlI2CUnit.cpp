#include "StdAfx.h"
#include ".\lpt2i2CHwModuleCtrlI2CUnit.h"
#include "LPT2I2CHwModuleCtrlDrv.h"
#include "HwModuleCtrl.h"
#include "HwModuleModel.h"

CLPT2I2CHwModuleCtrlI2CUnit::CLPT2I2CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,
														 CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv)
:CHwModuleCtrlI2CUnit(pHwModuleCtrl,pI2CUnitDrv)
{
	m_pI2CUnitDrv  = pI2CUnitDrv;
	m_sName		   = "LPT-2-I2C I2C";
	m_sDisplayName = "I2C";

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,pHwModuleCtrl);
}

CLPT2I2CHwModuleCtrlI2CUnit::~CLPT2I2CHwModuleCtrlI2CUnit(void)
{
	if(m_pI2CUnitDrv != NULL)
		delete m_pI2CUnitDrv;
}








