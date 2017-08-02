#include "StdAfx.h"
#include ".\ihwmoduleviewsubject.h"

IHwModuleViewSubject::IHwModuleViewSubject(void)
{
}

IHwModuleViewSubject::~IHwModuleViewSubject(void)
{
}

BOOL IHwModuleViewSubject::NotifyReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewReplaceHwModule(pHwModule,sNewHwModuleName) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleViewSubject::NotifyRemoveHwModule(CHwModule *pObj)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewRemoveHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL IHwModuleViewSubject::NotifySetHwModuleDisplayName(CHwModule *pObj, CString sDisplayName)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewSetHwModuleDisplayName(pObj,sDisplayName) == FALSE)
			return FALSE;
	}
	return TRUE;
}
BOOL IHwModuleViewSubject::NotifySelectHwModule(CHwModule *pObj)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewSelectHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleViewSubject::NotifyLoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewLoadHwModuleModelSettings(sFile,pCtrl) == FALSE)
			return FALSE;
	}
	return TRUE;
}
BOOL IHwModuleViewSubject::NotifySaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewSaveHwModuleModelSettings(sFile,pCtrl) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL IHwModuleViewSubject::NotifySetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewSetHwModuleCtrlUnitItemDisplayName(pItem,sDisplayName) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL IHwModuleViewSubject::NotifyResetHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewResetHwModuleCtrl(pCtrl) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleViewSubject::NotifyRefreshHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewRefreshHwModuleCtrl(pCtrl) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleViewSubject::NotifyDeSelectHwModule(CHwModule *pObj)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewDeSelectHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}
void IHwModuleViewSubject::RegisterObserver(IHwModuleViewObserver *pObserver)
{
	m_Observers.push_back(pObserver);
}

void IHwModuleViewSubject::UnregisterObserver(IHwModuleViewObserver *pObserver)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == pObserver)
		{
			m_Observers.erase(iter);
			break;
		}
	}
}

BOOL IHwModuleViewSubject::NotifyAddUsbDeviceMsg(CString sDevId)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewAddUsbDeviceMsg(sDevId) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL IHwModuleViewSubject::NotifyRemoveUsbDeviceMsg(CString sDevId)
{
	vector<IHwModuleViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewRemoveUsbDeviceMsg(sDevId) == FALSE)
			return FALSE;
	}
	return TRUE;
}
