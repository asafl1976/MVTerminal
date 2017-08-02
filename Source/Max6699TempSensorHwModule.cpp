#include "StdAfx.h"
#include ".\Max6699TempSensorHwModule.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include ".\Max6699TempSensorHwModuleAccessStartegy.h"

CMax6699TempSensorHwModule::CMax6699TempSensorHwModule(void)
{
	m_sName           = "Temperature Sensor (MAX6699)";
	m_pAccessStrategy = NULL;

}
CMax6699TempSensorHwModule::CMax6699TempSensorHwModule(CHwModuleInfo* pInfo):CTemperatureSensorHwModule(pInfo)
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
		m_pAccessStrategy = new CMax6699TempSensorHwModuleAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}
}

CMax6699TempSensorHwModule::~CMax6699TempSensorHwModule(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}

BOOL CMax6699TempSensorHwModule::GetTemperature(int* pTemperature)
{
	if(m_pAccessStrategy != NULL)
		return m_pAccessStrategy->GetTemperature(pTemperature);
	return FALSE;
}

CHwModule*   CMax6699TempSensorHwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CMax6699TempSensorHwModule(pInfo);
}

CString	CMax6699TempSensorHwModule::Serialize()
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

BOOL  CMax6699TempSensorHwModule::DeSerialize(CMarkup& xmlReader)
{

	return TRUE;
}
