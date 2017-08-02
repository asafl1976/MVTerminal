#include "StdAfx.h"
#include "Summitsmm205hwmodule.h"
#include "SummitSMM205HwModuleModelessDlg.h"
#include "HwModuleAccessStartegy.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "SummitSMM205HwModuleAccessStartegy.h"

CSummitSMM205HwModule::CSummitSMM205HwModule(void)
{
	m_sName           = "Summit SMM205";
	m_pAccessStrategy = NULL;
}

CSummitSMM205HwModule::CSummitSMM205HwModule(CHwModuleInfo* pInfo):CVoltageCtrlHwModule(pInfo)
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
		m_pAccessStrategy = new CSummitSMM205HwModuleAccessStartegy(this,pI2CUnitDrv);
		break;
	default:
		break;
	}

	//Set min/max deafult value
	m_nMin = 901;
	m_nMax = 1100;//mV
	m_nExtReference = 900;//mV
}

CSummitSMM205HwModule::~CSummitSMM205HwModule(void)
{
	if(m_pAccessStrategy != NULL)
	{
		delete m_pAccessStrategy;
		m_pAccessStrategy = NULL;
	}
}
void   CSummitSMM205HwModule::SetActiveChannel(VOLTAGE_CTRL_CHANNEL eChannel)
{
	if(m_pAccessStrategy != NULL)
	{
		m_pAccessStrategy->SetActiveChannel(eChannel);
	}
}


VOLTAGE_CTRL_CHANNEL   CSummitSMM205HwModule::GetActiveChannel()
{
	if(m_pAccessStrategy != NULL)
	{
		return m_pAccessStrategy->GetActiveChannel();
	}
	return VOLTAGE_CTRL_CHANNEL_UNKNOWN;
}

BOOL CSummitSMM205HwModule::SetVoltage(int nVoltage)
{
	if((nVoltage >= m_nMin) && (nVoltage <= m_nMax))
	{
		if(m_pAccessStrategy != NULL)
			return m_pAccessStrategy->SetVoltage(nVoltage);
	}
	return FALSE;
}

BOOL CSummitSMM205HwModule::GetVoltage(int* pVoltage)
{
	if(m_pAccessStrategy != NULL)
		return m_pAccessStrategy->GetVoltage(pVoltage);
	return FALSE;
}

CHwModule*   CSummitSMM205HwModule::Clone(CHwModuleInfo* pInfo)
{
	return new CSummitSMM205HwModule(pInfo);
}

CString	CSummitSMM205HwModule::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "HwModule");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "IFType",GetInterfaceTypeName());
	xmlWriter.AddAttrib( "DisplayName",GetDisplayName());
	xmlWriter.AddAttrib( "ID",GetID());
	xmlWriter.AddAttrib( "Min",m_nMin);
	xmlWriter.AddAttrib( "Max",m_nMax);
	xmlWriter.AddAttrib( "ExtRef",m_nExtReference);
	xmlWriter.AddAttrib( "ParentUnit",m_pParentUnit->GetName());

	return xmlWriter.GetDoc();
}

BOOL  CSummitSMM205HwModule::DeSerialize(CMarkup& xmlReader)
{
	CString sMin, sMax, sExtRef;

	sMin    = xmlReader.GetAttrib("Min");
	sMax    = xmlReader.GetAttrib("Max");
	sExtRef = xmlReader.GetAttrib("ExtRef");

	sscanf(sMin,"%d",&m_nMin);
	sscanf(sMax,"%d",&m_nMax);
	sscanf(sExtRef,"%d",&m_nExtReference);

	return TRUE;
}




