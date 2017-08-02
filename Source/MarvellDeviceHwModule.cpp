#include "StdAfx.h"
#include ".\marvelldevicehwmodule.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "HwModuleCtrlUnit.h"

CMarvellDeviceHwModule::CMarvellDeviceHwModule(void)
{	
	m_sName           = "Marvell Device";
	m_pAccessStrategy = NULL;
}

CMarvellDeviceHwModule::CMarvellDeviceHwModule(CHwModuleInfo* pInfo):CHwModule(pInfo)
{
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
		m_pAccessStrategy = new CMarvellDeviceHwModuleI2CAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}

}

CMarvellDeviceHwModule::~CMarvellDeviceHwModule(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}

BOOL CMarvellDeviceHwModule::ReadRegister(UINT32 nRegAddress, UINT32 *pRegVal)
{
	if(m_pAccessStrategy != NULL)
		return m_pAccessStrategy->ReadRegister(nRegAddress,pRegVal);
	return FALSE;
}

BOOL	CMarvellDeviceHwModule::WriteRegister(UINT32 nRegAddress,UINT32 nVal)
{
	if(m_pAccessStrategy != NULL)
		return m_pAccessStrategy->WriteRegister(nRegAddress,nVal);
	return FALSE;
}

CHwModule*   CMarvellDeviceHwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CMarvellDeviceHwModule(pInfo);
}

CString	CMarvellDeviceHwModule::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "HwModule");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "IFType",GetInterfaceTypeName());
	xmlWriter.AddAttrib( "DisplayName",GetDisplayName());
	xmlWriter.AddAttrib( "ID",GetID());
	xmlWriter.AddAttrib( "ParentUnit",m_pParentUnit->GetName());

	return xmlWriter.GetDoc();
}

BOOL  CMarvellDeviceHwModule::DeSerialize(CMarkup& xmlReader)
{
	return TRUE;
}

