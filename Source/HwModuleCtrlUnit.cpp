#include "StdAfx.h"
#include "HwModuleCtrlUnit.h"
#include "HwModule.h"
#include "HwModuleModel.h"

CHwModuleCtrlUnit::CHwModuleCtrlUnit(CHwModuleCtrl *pParent)
{
	m_pParent = pParent;
	m_pOwner  = pParent;
}

CHwModuleCtrlUnit::~CHwModuleCtrlUnit(void)
{
}

CString	CHwModuleCtrlUnit::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem( "HwModuleCtrlUnit");
	xmlWriter.AddAttrib( "Name",GetName());

	return xmlWriter.GetDoc();
}

BOOL  CHwModuleCtrlUnit::DeSerialize(CMarkup& xmlReader)
{
	return TRUE;
}




