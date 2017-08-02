#include "StdAfx.h"
#include ".\lpt2i2chwmodulectrl.h"
#include "LPT2I2CHwModuleCtrlI2CUnit.h"
#include "LPT2I2CHwModuleCtrlI2CUnitDrv.h"
#include "HwModuleModel.h"

CLPT2I2CHwModuleCtrl::CLPT2I2CHwModuleCtrl()
{ 
	m_nLptNumber = 1;
	m_pOwner       = NULL;
	m_sName        = "LPT-2-I2C";
	m_sDisplayName = "LPT-2-I2C";
	m_sID.Format("%d",m_nLptNumber);
	m_sUniqueID = m_sID;
	m_pDriver    = new CLPT2I2CHwModuleCtrlDrv();
	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,NULL);
	m_bConnected = FALSE;

	//Add its I2C unit
	CLPT2I2CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;
	pI2CUnitDrv = new CLPT2I2CHwModuleCtrlI2CUnitDrv((CLPT2I2CHwModuleCtrlDrv*)m_pDriver);
	AddUnit(new CLPT2I2CHwModuleCtrlI2CUnit(this,pI2CUnitDrv));	
}

CLPT2I2CHwModuleCtrl::~CLPT2I2CHwModuleCtrl(void)
{
	Disconnect();
	if(m_pDriver != NULL)
	{
		delete m_pDriver;
	}
}

void  CLPT2I2CHwModuleCtrl::EnableDriverErrorMessages(BOOL bEnable)
{
	m_pDriver->EnableErrorMessages(bEnable);
}

BOOL  CLPT2I2CHwModuleCtrl::Init()
{
	if(m_pDriver == NULL)
	{
		return FALSE;
	}
	if(m_pDriver->Load() == FALSE)
	{
		return FALSE;
	}
	if(Connect() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL  CLPT2I2CHwModuleCtrl::Connect()
{
	CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_ERROR_CODE	   code;

	if(m_pDriver == NULL)
		return FALSE;

	if(IsConnected() == TRUE)
		return TRUE;

	m_bConnected = FALSE;
	switch(code = LPTToI2CConnect())
	{
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_DLL_LOAD_ERROR:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_REQ_DEV_IS_BUSY:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_WRONG_DRV_STATUS:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_REQ_LPT_IS_BUSY:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_HW_PROBLEM:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_NORMAL_MODE_PROBLEM:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_SLOW_MODE_PROBLEM:
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_FREQ_PROBLEM:
		//ShowErrorMessageHigh(code);
		break;
	case CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_NO_DRV_PROBLEM:
		m_bConnected = TRUE;
		return TRUE;
	default:
		break;
	}
	return m_bConnected;
}

CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_ERROR_CODE  CLPT2I2CHwModuleCtrl::LPTToI2CConnect()
{
	short int bRes = 0;

	if (IsConnected() == FALSE)
	{
		//Starting the I2C driver
		bRes = m_pDriver->m_StartI2CDriverFunc(0);
		if (!bRes)
		{
			bRes = m_pDriver->m_StartI2CDriverFunc(1);
			if (!bRes)
			{
				return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_REQ_DEV_IS_BUSY;
			}
		}
		//Check the driver status
		bRes = m_pDriver->m_CheckDriverStatusFunc();
		if (!bRes)
		{
			return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_WRONG_DRV_STATUS;
		}

		//Setting the LPT port number
		bRes = m_pDriver->m_SetLPTNumberFunc(m_nLptNumber);
		if (!bRes)
		{
			return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_REQ_LPT_IS_BUSY;
		}		

		//Check if the HW is ok
		bRes = m_pDriver->m_HardwareDetectFunc();
		if (!bRes)
		{
			return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_HW_PROBLEM;
		}		

		if (m_pDriver->GetNormalMode() == TRUE)
		{
			bRes = m_pDriver->m_SetNormalModeFunc();
			if (!bRes)
			{
				return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_NORMAL_MODE_PROBLEM;
			}
		}
		else
		{
			bRes = m_pDriver->m_SetSlowModeFunc();
			if (bRes)
			{
				return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_SLOW_MODE_PROBLEM;
			}
		}

		int iRealFreq = m_pDriver->m_SetI2CFrequencyFunc(m_pDriver->GetFrequency());
		if (iRealFreq < m_pDriver->GetFrequency())
		{
			return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_FREQ_PROBLEM;
		}
		m_pDriver->m_MillisecondsFunc(m_pDriver->GetDelay());
		m_bConnected = TRUE;
	}
	return CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_NO_DRV_PROBLEM;
}

BOOL  CLPT2I2CHwModuleCtrl::Disconnect()
{		
    if (IsConnected())
    {
		m_pDriver->m_StopI2CDriverFunc();
        m_bConnected = FALSE;
    }
	return TRUE;
}

BOOL  CLPT2I2CHwModuleCtrl::IsTargetConnected()
{
	return TRUE;
}




