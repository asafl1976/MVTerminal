#include "StdAfx.h"
#include ".\boardpowerctrlmanager.h"
#include "HwModuleBoardPowerCtrlPowerStartegy.h"
#include "HwModuleBoardPowerCtrlResetStartegy.h"
#include "SUB20GPIOHwModuleBoardPowerCtrlPowerStartegy.h"
#include "SUB20GPIOHwModuleBoardPowerCtrlResetStartegy.h"
#include "HwModuleItem.h"
#include "MVTerminal.h"


CBoardPowerCtrlManager::CBoardPowerCtrlManager(void)
{
	m_pPowerStartegy = NULL;
	m_pResetStartegy = NULL;

	m_powerStartegyMap["SUB-20 GPIO"] = new CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy();
	m_resetStartegyMap["SUB-20 GPIO"] = new CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy();
}

CBoardPowerCtrlManager::~CBoardPowerCtrlManager(void)
{
	map<CString,CHwModuleBoardPowerCtrlPowerStartegy*>::iterator powerIter, powerEndIter;
	map<CString,CHwModuleBoardPowerCtrlResetStartegy*>::iterator resetIter, resetEndIter;

	powerIter    = m_powerStartegyMap.begin();
	powerEndIter = m_powerStartegyMap.end();

	for(; powerIter != powerEndIter; powerIter++)
	{
		delete powerIter->second;
	}

	resetIter    = m_resetStartegyMap.begin();
	resetEndIter = m_resetStartegyMap.end();

	for(; resetIter != resetEndIter; resetIter++)
	{
		delete resetIter->second;
	}
}

CBoardPowerCtrlManager& CBoardPowerCtrlManager::GetInstance()
{
    /* Allocate system object once on the heap */
    static CBoardPowerCtrlManager *pObj = new CBoardPowerCtrlManager();
    return *pObj;
}
VOID CBoardPowerCtrlManager::ResetPowerStartegy() 
{
	m_pPowerStartegy = NULL;
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlPower(FALSE);
}

VOID CBoardPowerCtrlManager::ResetResetStartegy() 
{
	m_pResetStartegy = NULL;
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlReset(FALSE);
}


BOOL CBoardPowerCtrlManager::SetPowerStartegy(CHwModuleItem* pItem, int nIO, CString sIOInitialState)
{
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlPower(FALSE);

	if(pItem == NULL)
		return FALSE;

	map<CString,CHwModuleBoardPowerCtrlPowerStartegy*>::iterator iter;
	iter = m_powerStartegyMap.find(pItem->GetName());
	if(iter == m_powerStartegyMap.end())
	{
		return FALSE;	
	}

	m_pPowerStartegy = iter->second;
	m_pPowerStartegy->SetHwModuleItem(pItem);
	m_pPowerStartegy->SetIO(nIO);
	m_pPowerStartegy->SetIOInitialState(sIOInitialState);
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlPower(TRUE);
	return TRUE;
}

BOOL CBoardPowerCtrlManager::SetResetStartegy(CHwModuleItem* pItem, int nIO, CString sIOInitialState)
{
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlReset(FALSE);

	if(pItem == NULL)
		return FALSE;

	map<CString,CHwModuleBoardPowerCtrlResetStartegy*>::iterator iter;
	iter = m_resetStartegyMap.find(pItem->GetName());
	if(iter == m_resetStartegyMap.end())
		return FALSE;	

	m_pResetStartegy = iter->second;
	m_pResetStartegy->SetHwModuleItem(pItem);
	m_pResetStartegy->SetIO(nIO);
	m_pPowerStartegy->SetIOInitialState(sIOInitialState);
	((CMVTerminalApp*)AfxGetApp())->EnablePowerCtrlReset(TRUE);

	return TRUE;
}

BOOL CBoardPowerCtrlManager::PowerOn()
{
	if(m_pPowerStartegy == NULL)
	{
		return FALSE;
	}
	if( m_pPowerStartegy->PowerOn() == TRUE)
	{
		((CMVTerminalApp*)AfxGetApp())->SetPowerCtrlPowerCheck(1);
		return TRUE;
	}
	return FALSE;
}

BOOL CBoardPowerCtrlManager::PowerOff()
{
	if(m_pPowerStartegy == NULL)
	{
		return FALSE;
	}
	if(m_pPowerStartegy->PowerOff() == TRUE)
	{
		((CMVTerminalApp*)AfxGetApp())->SetPowerCtrlPowerCheck(0);
		return TRUE;
	}
	return FALSE;
}

BOOL CBoardPowerCtrlManager::Reset(int nPulseLen)
{
	if(m_pResetStartegy == NULL)
		return FALSE;

	((CMVTerminalApp*)AfxGetApp())->SetPowerCtrlResetCheck(1);
	BOOL bRet = m_pResetStartegy->Reset(nPulseLen);
	((CMVTerminalApp*)AfxGetApp())->SetPowerCtrlResetCheck(0);
	return bRet;
}

