#include "StdAfx.h"
#include "HwModuleInfo.h"
#include "HwModuleItem.h"

CHwModuleInfo::CHwModuleInfo(void)
{
	m_sName	      = "";
	m_sID         = "";
	m_eIFType     = HWM_IF_UNKNOWN;
	m_pParentUnit = NULL;
	m_pOwner      = NULL;
}

CHwModuleInfo::CHwModuleInfo(CString sName,CString sID, HWM_IF_TYPE eIFType, 
							 CHwModuleCtrlUnit *pParentUnit, CHwModuleItem* pOwner)
{
	m_sName	      = sName;
	m_sID         = sID;
	m_eIFType     = eIFType;
	m_pParentUnit = pParentUnit;
	m_pOwner      = pOwner;
}

CHwModuleInfo::~CHwModuleInfo(void)
{
}

