#include "StdAfx.h"
#include "odpghwmodule.h"
#include "MV88F6281ODPGConfigStrategy.h"
#include "MV88F6601ODPGConfigStrategy.h"
#include "HwModuleAccessStartegy.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "ODPGHwModuleI2CAccessStartegy.h"
#include "ODPGHwModuleAccessStartegy.h"

CODPGHwModule::CODPGHwModule(void)
{
	m_sName           = "ODPG";
	m_pAccessStrategy = NULL;
}


CODPGHwModule::CODPGHwModule(CHwModuleInfo* pInfo) : CHwModule(pInfo)
{
	CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;


	CIODPGConfigStrategy *pODPGConfigStrategy;

	//Set default config startegy
	pODPGConfigStrategy = new CMV88F6281ODPGConfigStrategy(this);
	m_configStrategy[pODPGConfigStrategy->GetDeviceName()] = pODPGConfigStrategy;

	pODPGConfigStrategy = new CMV88F6601ODPGConfigStrategy(this);
	m_configStrategy[pODPGConfigStrategy->GetDeviceName()] = pODPGConfigStrategy;

	SetDevice( pODPGConfigStrategy->GetDeviceName() );

	// Set startegy
	switch(m_eIFType)
	{
	case HWM_IF_I2C:
		pI2CUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());
		if(pI2CUnitDrv == NULL)
		{
			//Throw exception
		}
		m_pAccessStrategy = new CODPGHwModuleI2CAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}

}

CHwModule*    CODPGHwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CODPGHwModule(pInfo);
}

CODPGHwModule::~CODPGHwModule(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}

	map<CString,CIODPGConfigStrategy*>::iterator iter,endIter;
	iter    = m_configStrategy.begin();
	endIter = m_configStrategy.end();
	for(;iter != endIter; iter++)
	{
		delete iter->second;
	}
	

}

void  CODPGHwModule::SetDevice(CString sDevice)
{
	m_sSelectedDevice = sDevice;
}

void  CODPGHwModule::GetDeviceList(vector<CString> & devList)
{
	map<CString,CIODPGConfigStrategy*>::iterator iter,endIter;
	iter    = m_configStrategy.begin();
	endIter = m_configStrategy.end();
	for(;iter != endIter; iter++)
	{
		devList.push_back(iter->first);
	}
}
CString  CODPGHwModule::GetDeviceBusWidth(CString sDevice)
{
	map<CString,CIODPGConfigStrategy*>::iterator iter,endIter;
	iter    = m_configStrategy.begin();
	endIter = m_configStrategy.end();
	for(;iter != endIter; iter++)
	{
		if(iter->first == sDevice)
		{
			return iter->second->GetDeviceBusWidth();
		}
	}
	return CString("32");
}


BOOL  CODPGHwModule::Set()
{
	if(m_sSelectedDevice.IsEmpty() == TRUE)
		return FALSE;

	map<CString,CIODPGConfigStrategy*>::iterator iter;
	
	iter = m_configStrategy.find(m_sSelectedDevice);

	if(iter != m_configStrategy.end())
	{
		return iter->second->InitODPG();
	}
	return FALSE;
}

BOOL  CODPGHwModule::Start()
{
	if(m_sSelectedDevice.IsEmpty() == TRUE)
		return FALSE;

	map<CString,CIODPGConfigStrategy*>::iterator iter;
	
	iter = m_configStrategy.find(m_sSelectedDevice);
	if(iter != m_configStrategy.end())
	{
		return iter->second->StartODPG();
	}
	return FALSE;
}

CIODPGConfigStrategy*  CODPGHwModule::GetConfigStrategy()
{
	if(m_sSelectedDevice.IsEmpty() == TRUE)
		return NULL;

	map<CString,CIODPGConfigStrategy*>::iterator iter;
	
	iter = m_configStrategy.find(m_sSelectedDevice);
	if(iter != m_configStrategy.end())
	{
		return iter->second;
	}
	return NULL;
}

BOOL  CODPGHwModule::Stop()
{
	BOOL				  bRet = FALSE;
	CIODPGConfigStrategy *pCfgStartegy = NULL;

	pCfgStartegy = GetConfigStrategy();
	if(pCfgStartegy != NULL)
		return pCfgStartegy->StopODPG();
	
	return FALSE;
}

int   CODPGHwModule::GetBufferLineByteCount()
{
	CIODPGConfigStrategy *pCfgStartegy = NULL;

	pCfgStartegy = GetConfigStrategy();
	if(pCfgStartegy != NULL)
		return pCfgStartegy->GetBufLineByteCount();

	return 0;
}

int   CODPGHwModule::GetBufferLineCount()
{
	CIODPGConfigStrategy *pCfgStartegy = NULL;

	pCfgStartegy = GetConfigStrategy();
	if(pCfgStartegy != NULL)
		return pCfgStartegy->GetBufLineCount();

	return 0;
}


BOOL  CODPGHwModule::WriteRegister32(UINT nRegOffset, UINT nRegValue, BOOL bReadModifyWrite)
{	
	CIODPGConfigStrategy *pCfgStartegy = NULL;

	pCfgStartegy = GetConfigStrategy();
	if(pCfgStartegy != NULL)
	{
		if(m_pAccessStrategy != NULL)
			return m_pAccessStrategy->WriteRegister32(pCfgStartegy->GetDeviceRegBase(),nRegOffset,nRegValue,bReadModifyWrite);
	}
	return FALSE;
}

BOOL  CODPGHwModule::ReadRegister32(UINT nRegOffset,UINT *pRegValue)
{	
	CIODPGConfigStrategy *pCfgStartegy = NULL;

	pCfgStartegy = GetConfigStrategy();
	if(pCfgStartegy != NULL)
	{
		if(m_pAccessStrategy != NULL)
			return m_pAccessStrategy->ReadRegister32(pCfgStartegy->GetDeviceRegBase(),nRegOffset,pRegValue);
	}
	return FALSE;
}


