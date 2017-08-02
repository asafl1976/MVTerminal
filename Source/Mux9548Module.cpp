#include "StdAfx.h"
#include ".\mux9548module.h"
#include "HwModuleAccessStartegy.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleFactory.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "Mux9548HwModuleAccessStartegy.h"

CMux9548Module::CMux9548Module(void)
{
	m_sName           = "Mux 9548";
	m_nChannelCount   = 8;
	m_pAccessStrategy = NULL;
}

CMux9548Module::CMux9548Module(CHwModuleInfo* pInfo) : CMuxHwModule(pInfo)
{
	CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv;

	m_nChannelCount = 8;
	// Set startegy
	switch(m_eIFType)
	{
	case HWM_IF_I2C:
		pI2CUnitDrv = dynamic_cast<CHwModuleCtrlI2CUnitDrv*>(m_pParentUnit->GetDriver());
		if(pI2CUnitDrv == NULL)
		{
			//Throw exception
		}
		m_pAccessStrategy = new CMux9548HwModuleAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}
}

CMux9548Module::~CMux9548Module(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}

CHwModule*    CMux9548Module::Clone(CHwModuleInfo* pInfo)
{
	return new CMux9548Module(pInfo);
}