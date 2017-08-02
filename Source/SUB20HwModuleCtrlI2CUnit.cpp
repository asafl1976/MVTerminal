#include "StdAfx.h"
#include "SUB20HwModuleCtrlDrv.h"
#include "SUB20HwModuleCtrlI2CUnit.h"
#include "HwModuleModel.h"

CSUB20HwModuleCtrlI2CUnit::CSUB20HwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv)
:CHwModuleCtrlI2CUnit(pHwModuleCtrl,pI2CUnitDrv)
{
	m_sName		   = "SUB-20 I2C";
	m_sDisplayName = "I2C";
	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,pHwModuleCtrl);
}

CSUB20HwModuleCtrlI2CUnit::~CSUB20HwModuleCtrlI2CUnit(void)
{

}

BOOL CSUB20HwModuleCtrlI2CUnit::Init()
{
	//By default set frequency to 100K
	return SetFrequency(100000);	
}




