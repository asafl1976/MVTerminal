#include "StdAfx.h"
#include ".\summitsmm205hwmoduleaccessstartegy.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "HwModule.h"
#include "SummitSMM205HwModule.h"

CSummitSMM205HwModuleAccessStartegy::CSummitSMM205HwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv)
:CVoltageCtrlHwModuleAccessStartegy(pHwModule)
{
	m_pI2CUnitDrv = pI2CUnitDrv;
	m_eChannel    = VOLTAGE_CTRL_CHANNEL_A;
}

CSummitSMM205HwModuleAccessStartegy::~CSummitSMM205HwModuleAccessStartegy(void)
{
}

BOOL  CSummitSMM205HwModuleAccessStartegy::Unlock()
{
	UCHAR nData = 5;

	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,0x42,1,&nData) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL  CSummitSMM205HwModuleAccessStartegy::SetVoltage(int nVoltage)
{
	unsigned int   nVolRegLSB = 1, nVolRegMSB = 0,nVolLSB, nVolMSB, nExtRef;

	CSummitSMM205HwModule *pSummitSMM205HwModule;
	pSummitSMM205HwModule = dynamic_cast<CSummitSMM205HwModule*>(m_pHwModule);
	if(pSummitSMM205HwModule == NULL)
	{
		return FALSE;
	}

	if(Unlock() == FALSE)
	{
		AfxMessageBox("Failed to unlock device",NULL,MB_ICONERROR);
		return FALSE;
	}
	
	if(m_eChannel == VOLTAGE_CTRL_CHANNEL_A)
	{
		nVolRegMSB = 0;
		nVolRegLSB = 1;
	}
	if(m_eChannel == VOLTAGE_CTRL_CHANNEL_B)
	{
		nVolRegMSB = 2;
		nVolRegLSB = 3;
	}

	nExtRef = pSummitSMM205HwModule->GetExtReferenceValue();
	nVoltage = (900*nVoltage)/nExtRef;
	nVolMSB = (3600/nVoltage);
	nVolLSB = (1024*900/(unsigned int)nVoltage) - nVolMSB*256;

	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,nVolRegMSB,1,(UCHAR*)&nVolMSB) == FALSE)
	{
		return FALSE; 
	}
	Sleep(350);
	if(m_pI2CUnitDrv->WriteData(m_nHwModuleID,nVolRegLSB,1,(UCHAR*)&nVolLSB) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}



BOOL  CSummitSMM205HwModuleAccessStartegy::GetVoltage(int* pVoltage)
{
	unsigned int  nVolRegLSB = 1, nVolRegMSB = 0, nVolLSB = 0, nVolMSB = 0,nExtRef;

	CSummitSMM205HwModule *pSummitSMM205HwModule;
	pSummitSMM205HwModule = dynamic_cast<CSummitSMM205HwModule*>(m_pHwModule);
	if(pSummitSMM205HwModule == NULL)
	{
		return FALSE;
	}

	if(m_eChannel == VOLTAGE_CTRL_CHANNEL_A)
	{
		nVolRegMSB = 0;
		nVolRegLSB = 1;
	}
	if(m_eChannel == VOLTAGE_CTRL_CHANNEL_B)
	{
		nVolRegMSB = 2;
		nVolRegLSB = 3;
	}


	if(m_pI2CUnitDrv->ReadData(m_nHwModuleID,nVolRegMSB,1,(UCHAR*)&nVolMSB) == FALSE)
	{
		return FALSE;
	}
	Sleep(350);
	if(m_pI2CUnitDrv->ReadData(m_nHwModuleID,nVolRegLSB,1,(UCHAR*)&nVolLSB) == FALSE)
	{
		return FALSE;
	}
	nExtRef = pSummitSMM205HwModule->GetExtReferenceValue();
	*pVoltage = (1024*900)/(nVolLSB + nVolMSB*256);
	*pVoltage = ((*pVoltage)*nExtRef)/900;
	return TRUE;
}
