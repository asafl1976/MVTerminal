#include "StdAfx.h"
#include ".\sub20hwmodulectrladcunitcurrentitem.h"
#include "HwModuleCtrlADCUnitDrv.h"
#include "SUB20HwModuleCtrlADCUnit.h"

CSUB20HwModuleCtrlADCUnitCurrentItem::CSUB20HwModuleCtrlADCUnitCurrentItem(int nIndex, CSUB20HwModuleCtrlADCUnit *pUnit)
: CHwModuleCtrlADCUnitCurrentItem()
{
	m_pUnit = pUnit;
	m_sName  = "ADC Current";
	m_nIndex = nIndex;
	m_sID.Format("%d",m_nIndex);
	m_sDisplayName.Format("Current#%d",m_nIndex);

	m_nMin			 = 0;
	m_nMax			 = 15000;
	m_nResistorValue = 47;
	
}

CSUB20HwModuleCtrlADCUnitCurrentItem::~CSUB20HwModuleCtrlADCUnitCurrentItem(void)
{
}


void CSUB20HwModuleCtrlADCUnitCurrentItem::SetDisplayName(CString sName)
{
	m_sDisplayNameSuffix = "";
	if(sName.IsEmpty() == TRUE)
	{
		m_sDisplayName.Format("Current#%d",m_nIndex);
	}
	else
	{
		m_sDisplayNameSuffix = sName;
		m_sDisplayName.Format("Current#%d - %s",m_nIndex,m_sDisplayNameSuffix);
	}
}

BOOL CSUB20HwModuleCtrlADCUnitCurrentItem::GetCurrent(float nResistorVal, int* pCurrent)
{
	CHwModuleCtrlADCUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlADCUnitDrv*>(m_pUnit->GetDriver());
	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->GetCurrent(m_nIndex,nResistorVal,pCurrent);
}

CString	CSUB20HwModuleCtrlADCUnitCurrentItem::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "CurrentItem");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "Index",m_nIndex);
	xmlWriter.AddAttrib( "DisplayName",m_sDisplayNameSuffix);
	xmlWriter.AddAttrib( "Min",m_nMin);
	xmlWriter.AddAttrib( "Max",m_nMax);
	CString sResValue;
	sResValue.Format("%f",m_nResistorValue);
	xmlWriter.AddAttrib( "Resistor",sResValue);
	return xmlWriter.GetDoc();
}

