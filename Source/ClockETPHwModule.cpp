#include "StdAfx.h"
#include ".\clocketphwmodule.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "ClockETPHwModuleAccessStartegy.h"


CClockETPHwModule::CClockETPHwModule(void)
{
	m_sName           = "Clock ETP";
	m_pAccessStrategy = NULL;
}

CClockETPHwModule::CClockETPHwModule(CHwModuleInfo* pInfo)
:CFrequencyCtrlHwModule(pInfo)
{
	m_sName           = "Clock ETP";
	m_pAccessStrategy = NULL;
	
	CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;
	// Set startegy
	switch(m_eIFType)
	{
	case HWM_IF_I2C:
		pI2CUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());
		if(pI2CUnitDrv == NULL)
		{
			//Throw exception
		}
		m_pAccessStrategy = new CClockETPHwModuleAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}
}


CClockETPHwModule::~CClockETPHwModule(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}

BOOL 	 CClockETPHwModule::SetFrequancy(long nFreq, long nRef, long nRange)
{
	if(m_pAccessStrategy != NULL)
		return m_pAccessStrategy->SetFrequency(nFreq,nRef,nRange);
	return 0;
}

CHwModule*  CClockETPHwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CClockETPHwModule(pInfo);

}

