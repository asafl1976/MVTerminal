#include "StdAfx.h"
#include ".\usb2i2chwmodulectrl.h"
#include "USB2I2CHwModuleCtrlI2CUnit.h"
#include "USB2I2CHwModuleCtrlI2CUnitDrv.h"
#include "HwModuleModel.h"

CUSB2I2CHwModuleCtrl::CUSB2I2CHwModuleCtrl(CString sUniqueID)
{
	m_pOwner       = NULL;
	
	m_sVendorID    = USB_ID_USB_2_I2C;
	m_sUniqueID    = sUniqueID;
	m_sID		   = m_sVendorID + m_sUniqueID;

	m_sName		   = "USB-2-I2C";
	m_sDisplayName = "USB-2-I2C";

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,NULL);

	int nSerialNumber;
	sscanf(sUniqueID,"%d",&nSerialNumber);
	m_pDriver	= new CUSB2I2CHwModuleCtrlDrv(nSerialNumber);

	CUSB2I2CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;
	pI2CUnitDrv = new CUSB2I2CHwModuleCtrlI2CUnitDrv((CUSB2I2CHwModuleCtrlDrv*)m_pDriver);
	AddUnit(new CUSB2I2CHwModuleCtrlI2CUnit(this,pI2CUnitDrv));
}

CUSB2I2CHwModuleCtrl::~CUSB2I2CHwModuleCtrl(void)
{
	Disconnect();
	if(m_pDriver != NULL)
		delete m_pDriver;
}

void  CUSB2I2CHwModuleCtrl::EnableDriverErrorMessages(BOOL bEnable)
{
	m_pDriver->EnableErrorMessages(bEnable);
}


BOOL  CUSB2I2CHwModuleCtrl::IsTargetConnected()
{
	CUSB2I2CHwModuleCtrlDrv::USB2I2C_ERROR_CODE eEc;
	UCHAR              nTestAddr = 0x10,nChar;

	if(m_bConnected == FALSE)
		return FALSE;

    nTestAddr <<= 1;//Shift the address to make a room for read/write bit.

	//Test if bus is connected to I2C slave (not just the USB module)
	EnableDriverErrorMessages(FALSE);
    m_pDriver->ReadData(nTestAddr, 1, &nChar,TRUE);
	EnableDriverErrorMessages(TRUE);
	eEc = m_pDriver->GetLastErrorCode();
    if (eEc == CUSB2I2CHwModuleCtrlDrv::USB2I2C_I2C_TIMEOUT)
    {
        return FALSE;
    }	
	return TRUE;
}

BOOL  CUSB2I2CHwModuleCtrl::Disconnect()
{	
    if (IsConnected())
    {
        m_bConnected = FALSE;
    }
    
	return TRUE;
}

BOOL  CUSB2I2CHwModuleCtrl::Init()
{
	if(m_pDriver == NULL)
		return FALSE;

	if(m_pDriver->Load() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL  CUSB2I2CHwModuleCtrl::Connect()
{
	if(IsConnected() == TRUE)
	{
		return TRUE;
	}
	if(m_pDriver->OpenDevice() == 0)
	{
		return FALSE;
	}	
	m_bConnected = TRUE;
	return m_bConnected;
}