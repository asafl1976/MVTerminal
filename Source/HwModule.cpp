#include "StdAfx.h"
#include "HwModule.h"
#include "MuxHwModule.h"
#include "HwModuleModel.h"
#include "DragDropTreeCtrl.h"
#include "HwModuleCtrlUnit.h"

CHwModule::CHwModule()
{
	m_eIFType  = HWM_IF_UNKNOWN;
}

CHwModule::CHwModule(CHwModuleInfo* pInfo)
{
	m_eIFType     = pInfo->GetInterfaceType();
	m_sID         = pInfo->GetID();
	m_sName       = pInfo->GetName();
	m_pParentUnit = pInfo->GetParentUnit();
	m_pOwner      = pInfo->GetOwner();

	//Set display name
	m_sDisplayName = "0x";
	m_sDisplayName += m_sID.MakeUpper();
	m_sDisplayName += " - "; 
	m_sDisplayName += GetName(); 
}

CHwModule::~CHwModule(void)
{
}

void CHwModule::SetDisplayName(CString sDisplayName)
{
	CString sTemp = sDisplayName;

	sTemp.MakeUpper();
	//If new name DO NOT contain the "0xID - "...add it
	if(sTemp.Find("0X" + GetID().MakeUpper() + " - ") == -1)
	{
		CString s = "0x" + GetID().MakeUpper() + " - ";
		m_sDisplayName = "0x";
		m_sDisplayName += GetID().MakeUpper();
		m_sDisplayName += " - "; 
		m_sDisplayName += sDisplayName; 
	}
	else//Already have the "0xID - "
	{
		m_sDisplayName = sDisplayName; 
	}
}


CString		CHwModule::GetInterfaceTypeName()
{
	switch(m_eIFType)
	{
	case HWM_IF_I2C:
		return CString("I2C");
	case HWM_IF_USB:
		return CString("USB");
	case HWM_IF_UNKNOWN:
		return CString("Unknown");
	}
	return CString("Unknown"); 
}

CString	CHwModule::Serialize()
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

BOOL  CHwModule::DeSerialize(CMarkup& xmlReader)
{
	return TRUE;
}






