#include "StdAfx.h"
#include ".\unknowni2chwmodule.h"
#include "UnknownI2CHwModuleModelessDlg.h"
#include "HwModuleCtrlI2CUnit.h"
#include "MuxHwModule.h"
#include "HwModuleModel.h"

CUnknownI2CHwModule::CUnknownI2CHwModule()
{
	m_sName = "Unknown (I2C)";
}

CUnknownI2CHwModule::CUnknownI2CHwModule(CHwModuleInfo* pInfo):CUnknownHwModule(pInfo)
{
	//Set display name
	m_sDisplayName = "0x";
	m_sDisplayName += GetID();
	m_sDisplayName += " - "; 
	m_sDisplayName += "Unknown"; 

	CString sAddress;
	sAddress = GetID();
	sscanf((LPCSTR)sAddress,"%x",&m_nHwModuleID);
}

CUnknownI2CHwModule::~CUnknownI2CHwModule()
{

}


CHwModule*   CUnknownI2CHwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CUnknownI2CHwModule(pInfo);
}


BOOL   CUnknownI2CHwModule::WriteData(UCHAR nSubAddress, int nBytes, UCHAR *pWriteData)
{
	CHwModuleCtrlI2CUnitDrv *pUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());

	if(pUnitDrv == NULL)
		return FALSE;

	return pUnitDrv->WriteData(m_nHwModuleID,nSubAddress, nBytes, pWriteData);
}

BOOL   CUnknownI2CHwModule::ReadData(UCHAR nSubAddress, int nBytes, UCHAR *pReadData)
{
	CHwModuleCtrlI2CUnitDrv *pUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());

	if(pUnitDrv == NULL)
		return FALSE;

	return pUnitDrv->ReadData(m_nHwModuleID,nSubAddress, nBytes, pReadData);
}

BOOL  CUnknownI2CHwModule::WriteData(int nBytes, UCHAR *pWriteData,BOOL bSendStopBit)
{
	CHwModuleCtrlI2CUnitDrv *pUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());

	if(pUnitDrv == NULL)
		return FALSE;

	return pUnitDrv->WriteData(m_nHwModuleID,nBytes,pWriteData,bSendStopBit);
}

BOOL  CUnknownI2CHwModule::ReadData(int nBytes, UCHAR *pReadData,BOOL bSendStopBit)
{
	CHwModuleCtrlI2CUnitDrv *pUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());

	if(pUnitDrv == NULL)
		return FALSE;

	return pUnitDrv->ReadData(m_nHwModuleID,nBytes,pReadData,bSendStopBit);
}




