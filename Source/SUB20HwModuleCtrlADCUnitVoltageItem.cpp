#include "StdAfx.h"
#include ".\sub20hwmodulectrladcunitvoltageitem.h"
#include "SUB20HwModuleCtrlADCUnitDrv.h"
#include "SUB20HwModuleCtrlADCUnit.h"

CSUB20HwModuleCtrlADCUnitVoltageItem::CSUB20HwModuleCtrlADCUnitVoltageItem(int nIndex, CSUB20HwModuleCtrlADCUnit *pUnit)
: CHwModuleCtrlADCUnitVoltageItem()
{
	m_pUnit = pUnit;
	m_sName  = "ADC Voltage";
	m_nIndex = nIndex;
	m_sID.Format("%d",m_nIndex);
	m_sDisplayName.Format("Voltage#%d",m_nIndex);

	m_nMin   = 0;
	m_nMax   = 3300;

}

CSUB20HwModuleCtrlADCUnitVoltageItem::~CSUB20HwModuleCtrlADCUnitVoltageItem(void)
{
}

void CSUB20HwModuleCtrlADCUnitVoltageItem::SetDisplayName(CString sName)
{
	m_sDisplayNameSuffix = "";
	if(sName.IsEmpty() == TRUE)
	{
		m_sDisplayName.Format("Voltage#%d",m_nIndex);
	}
	else
	{
		m_sDisplayNameSuffix = sName;
		m_sDisplayName.Format("Voltage#%d - %s",m_nIndex,m_sDisplayNameSuffix);
	}
}

BOOL CSUB20HwModuleCtrlADCUnitVoltageItem::GetVoltage(int *pVoltage)
{
	CHwModuleCtrlADCUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlADCUnitDrv*>(m_pUnit->GetDriver());
	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->GetVoltage(m_nIndex,pVoltage);
}

CString	CSUB20HwModuleCtrlADCUnitVoltageItem::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "VoltageItem");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "Index",m_nIndex);
	xmlWriter.AddAttrib( "DisplayName",m_sDisplayNameSuffix);
	xmlWriter.AddAttrib( "Min",m_nMin);
	xmlWriter.AddAttrib( "Max",m_nMax);

	return xmlWriter.GetDoc();
}
