#include "StdAfx.h"
#include "SUB20HwModuleCtrlDrv.h"
#include "SUB20HwModuleCtrlADCUnit.h"
#include "HwModuleModel.h"
#include "SUB20HwModuleCtrlADCUnitVoltageItem.h"
#include "SUB20HwModuleCtrlADCUnitCurrentItem.h"


CSUB20HwModuleCtrlADCUnit::CSUB20HwModuleCtrlADCUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlADCUnitDrv* pADCUnitDrv)
:CHwModuleCtrlADCUnit(pHwModuleCtrl,pADCUnitDrv)
{
	int nVoltage,nCurrent;

	m_sName		   = "SUB-20 ADC";
	m_sDisplayName = "ADC";

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(this,pHwModuleCtrl);

	for(nVoltage = 0; nVoltage < 8; nVoltage++)
	{
		CSUB20HwModuleCtrlADCUnitVoltageItem *pItem;
		pItem = new CSUB20HwModuleCtrlADCUnitVoltageItem(nVoltage,this);
		m_unitItems.push_back(pItem);
		CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(pItem,this);
	}
	for(nCurrent = 0; nCurrent < 2; nCurrent++)
	{
		CSUB20HwModuleCtrlADCUnitCurrentItem *pItem;
		pItem = new CSUB20HwModuleCtrlADCUnitCurrentItem(nCurrent,this);
		m_unitItems.push_back(pItem);
		CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(pItem,this);
	}
}

CSUB20HwModuleCtrlADCUnit::~CSUB20HwModuleCtrlADCUnit(void)
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
}

BOOL CSUB20HwModuleCtrlADCUnit::Init()
{
	return m_pADCUnitDrv->Init();
}

BOOL CSUB20HwModuleCtrlADCUnit::IsUnitItemExist(CHwModuleCtrlUnitItem* pItem)
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == pItem)
		{
			return TRUE;
		}
	}
	return FALSE;
}
CHwModuleCtrlUnitItem* CSUB20HwModuleCtrlADCUnit::GetItem(CString sName)
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->GetDisplayNameSuffix() == sName)
		{
			return (*iter);
		}
	}
	return NULL;
}

BOOL CSUB20HwModuleCtrlADCUnit::SetItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName) 
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == pItem)
		{
			pItem->SetDisplayName(sDisplayName);
			CHwModuleModel::GetInstance().NotifyHwModuleCtrlUnitItemDisplayNameChange(pItem);
			return TRUE;
		}
	}
	return FALSE;
}
CSUB20HwModuleCtrlADCUnitVoltageItem* CSUB20HwModuleCtrlADCUnit::GetVoltageItem(int nIndex)
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	CString											  sIndex;

	sIndex.Format("%d",nIndex);
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		if(((*iter)->GetName() == "ADC Voltage") && ((*iter)->GetID() == sIndex))
			return (CSUB20HwModuleCtrlADCUnitVoltageItem*)(*iter);
	}
	return NULL;
}
CSUB20HwModuleCtrlADCUnitCurrentItem* CSUB20HwModuleCtrlADCUnit::GetCurrentItem(int nIndex)
{
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;
	CString											  sIndex;

	sIndex.Format("%d",nIndex);
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		if(((*iter)->GetName() == "ADC Current") && ((*iter)->GetID() == sIndex))
			return (CSUB20HwModuleCtrlADCUnitCurrentItem*)(*iter);
	}
	return NULL;
}
CString	CSUB20HwModuleCtrlADCUnit::Serialize()
{
	CMarkup								        xmlWriter;
	vector<CHwModuleCtrlADCUnitItem*>::iterator iter,endIter;

	xmlWriter.AddElem( "HwModuleCtrlUnit");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.IntoElem();

	xmlWriter.AddElem( "VCItems");
	xmlWriter.IntoElem();
	iter    = m_unitItems.begin();
	endIter = m_unitItems.end();
	for(; iter != endIter; iter++)
	{
		xmlWriter.AddSubDoc((*iter)->Serialize());
	}
	xmlWriter.OutOfElem();

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL  CSUB20HwModuleCtrlADCUnit::DeSerialize(CMarkup& xmlReader)
{
	CString								  sName,sDisplayName,sIndex,sMin,sMax,sRes;
	int								      nIndex, nMin, nMax;
	CSUB20HwModuleCtrlADCUnitVoltageItem *pVolItem;
	CSUB20HwModuleCtrlADCUnitCurrentItem *pCurrItem;
	float                                 nRes;

	xmlReader.IntoElem();

	if(xmlReader.FindElem("VCItems") == false)
		return FALSE;

	xmlReader.IntoElem();
    while ( xmlReader.FindElem("VoltageItem") )
	{
		sName        = xmlReader.GetAttrib("Name");
		sIndex       = xmlReader.GetAttrib("Index");
		sDisplayName = xmlReader.GetAttrib("DisplayName");
		sMin         = xmlReader.GetAttrib("Min");
		sMax         = xmlReader.GetAttrib("Max");

		sscanf(sIndex,"%d",&nIndex);
		if((pVolItem = GetVoltageItem(nIndex)) == NULL)
		{
			//Notify user of replace failure
			AfxMessageBox("Error while loading voltage item!",NULL,MB_ICONWARNING);
			continue;
		}
		SetItemDisplayName(pVolItem,sDisplayName);

		sscanf(sMin,"%d",&nMin);
		sscanf(sMax,"%d",&nMax);
		pVolItem->SetMin(nMin);
		pVolItem->SetMax(nMax);
	}
	xmlReader.OutOfElem();
	xmlReader.IntoElem();
	while ( xmlReader.FindElem("CurrentItem") )
	{
		sName        = xmlReader.GetAttrib("Name");
		sIndex       = xmlReader.GetAttrib("Index");
		sDisplayName = xmlReader.GetAttrib("DisplayName");
		sMin         = xmlReader.GetAttrib("Min");
		sMax         = xmlReader.GetAttrib("Max");
		sRes         = xmlReader.GetAttrib("Resistor");

		sscanf(sIndex,"%d",&nIndex);
		if((pCurrItem = GetCurrentItem(nIndex)) == NULL)
		{
			//Notify user of replace failure
			AfxMessageBox("Error while loading voltage item!",NULL,MB_ICONWARNING);
			continue;
		}
		SetItemDisplayName(pCurrItem,sDisplayName);

		sscanf(sMin,"%d",&nMin);
		sscanf(sMax,"%d",&nMax);
		sscanf(sRes,"%f",&nRes);
		pCurrItem->SetMin(nMin);
		pCurrItem->SetMax(nMax);
		pCurrItem->SetResistorValue(nRes);
	}
	xmlReader.OutOfElem();
	xmlReader.OutOfElem();
	return TRUE;
}


