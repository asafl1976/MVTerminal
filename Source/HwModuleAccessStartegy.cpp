#include "StdAfx.h"
#include "hwmoduleaccessstartegy.h"
#include "HwModule.h"

CHwModuleAccessStartegy::CHwModuleAccessStartegy(CHwModule *pHWModule)
{
	m_pHwModule   = pHWModule;

	//Convert ID to int
	CString sID = m_pHwModule->GetID();
	sscanf((LPCTSTR)sID,"%x",&m_nHwModuleID);
}

CHwModuleAccessStartegy::~CHwModuleAccessStartegy(void)
{

}

