#include "StdAfx.h"
#include ".\sub20hwmodulectrl.h"
#include "SUB20HwModuleCtrlDrv.h"
#include "SUB20HwModuleCtrlI2CUnit.h"
#include "SUB20HwModuleCtrlADCUnit.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"
#include "SUB20HwModuleCtrlI2CUnitDrv.h"
#include "SUB20HwModuleCtrlADCUnitDrv.h"
#include "SUB20HwModuleCtrlGPIOUnitDrv.h"
#include "HwModuleModel.h"

CSUB20HwModuleCtrl::CSUB20HwModuleCtrl(sub_handle handle,CString sUniqueID)
{
	CSUB20HwModuleCtrlI2CUnitDrv *pI2CUnitDrv;
	CSUB20HwModuleCtrlADCUnitDrv *pADCUnitDrv;
	CSUB20HwModuleCtrlGPIOUnitDrv *pGPIOUnitDrv;

	m_sVendorID    = USB_ID_SUB_20;
	m_sUniqueID    = sUniqueID;
	m_sID		   = m_sVendorID + m_sUniqueID;

	m_pOwner       = NULL;
	m_sName		   = "SUB-20";
	m_sDisplayName = "SUB-20";

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,NULL);

	m_pDriver   = new CSUB20HwModuleCtrlDrv(handle);
	pGPIOUnitDrv = new CSUB20HwModuleCtrlGPIOUnitDrv(m_pDriver);
	pI2CUnitDrv = new CSUB20HwModuleCtrlI2CUnitDrv(m_pDriver);
	pADCUnitDrv = new CSUB20HwModuleCtrlADCUnitDrv(m_pDriver);

	AddUnit(new CSUB20HwModuleCtrlI2CUnit(this,pI2CUnitDrv));
	AddUnit(new CSUB20HwModuleCtrlADCUnit(this,pADCUnitDrv));
	AddUnit(new CSUB20HwModuleCtrlGPIOUnit(this,pGPIOUnitDrv));
}

CSUB20HwModuleCtrl::~CSUB20HwModuleCtrl(void)
{
	Disconnect();
	if(m_pDriver != NULL)
	{
		delete m_pDriver;
		m_pDriver = NULL;
	}
}

void  CSUB20HwModuleCtrl::EnableDriverErrorMessages(BOOL bEnable)
{
	if(m_pDriver != NULL)
		m_pDriver->EnableErrorMessages(bEnable);
}

BOOL  CSUB20HwModuleCtrl::Reset()
{
	if(m_pDriver == NULL)
		return FALSE;

	if(m_pDriver->Reset() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL  CSUB20HwModuleCtrl::Init()
{
	if(m_pDriver == NULL)
		return FALSE;

	if(m_pDriver->Load() == FALSE)
	{
		return FALSE;
	}
	m_bConnected = TRUE;

	//Init units
	vector<CHwModuleCtrlUnit*>::iterator iter, endIter;
	iter    = m_units.begin();
	endIter = m_units.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) != NULL)
		{
			if((*iter)->Init() == FALSE)
			{
				return FALSE;
			}
		}
	}

	//AfxMessageBox("reset");
	//Reset();

	return TRUE;
}

BOOL  CSUB20HwModuleCtrl::IsTargetConnected()
{
	return TRUE;
}

BOOL  CSUB20HwModuleCtrl::Connect()
{
	int nI2CFreq = 100000;// Hz

	if(IsConnected() == TRUE)
		return TRUE;
	m_bConnected = TRUE;

	//Set I2C frequancy 
	m_pDriver->I2CSetFrequency(&nI2CFreq);

	return m_bConnected;
}

BOOL  CSUB20HwModuleCtrl::Disconnect()
{
    m_bConnected = FALSE;
	return TRUE;
}
