#include "StdAfx.h"
#include ".\mux9544module.h"
#include "HwModuleAccessStartegy.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleFactory.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "Mux9544HwModuleAccessStartegy.h"

CMux9544Module::CMux9544Module(void)
{
	m_sName           = "Mux 9544";
	m_nChannelCount   = 4;
	m_pAccessStrategy = NULL;
}

CMux9544Module::CMux9544Module(CHwModuleInfo* pInfo) : CMuxHwModule(pInfo)
{
	CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;

	m_nChannelCount = 4;
	// Set startegy
	switch(m_eIFType)
	{
	case HWM_IF_I2C:
		pI2CUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());
		if(pI2CUnitDrv == NULL)
		{
			//Throw exception
		}
		m_pAccessStrategy = new CMux9544HwModuleAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}
}

CMux9544Module::~CMux9544Module(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}

CHwModule*    CMux9544Module::Clone(CHwModuleInfo* pInfo)
{
	return new CMux9544Module(pInfo);
}