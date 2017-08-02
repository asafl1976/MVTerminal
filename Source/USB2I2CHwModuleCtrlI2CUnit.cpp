#include "StdAfx.h"
#include ".\usb2i2CHwModuleCtrlI2CUnit.h"
#include "USB2I2CHwModuleCtrlDrv.h"
#include "HwModuleModel.h"

CUSB2I2CHwModuleCtrlI2CUnit::CUSB2I2CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv)
:CHwModuleCtrlI2CUnit(pHwModuleCtrl,pI2CUnitDrv)
{
	m_sName		   = "USB-2-I2C I2C";
	m_sDisplayName = "I2C";

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,pHwModuleCtrl);
}

CUSB2I2CHwModuleCtrlI2CUnit::~CUSB2I2CHwModuleCtrlI2CUnit(void)
{

}

