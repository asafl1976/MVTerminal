#include "StdAfx.h"
#include ".\IHwModuleModelSubject.h"

IHwModuleModelSubject::IHwModuleModelSubject(void)
{
}

IHwModuleModelSubject::~IHwModuleModelSubject(void)
{
}

void IHwModuleModelSubject::RegisterObserver(IHwModuleModelObserver *pObserver)
{
	m_Observers.push_back(pObserver);
}

void IHwModuleModelSubject::UnregisterObserver(IHwModuleModelObserver *pObserver)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

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
BOOL IHwModuleModelSubject::NotifyDeSelectHwModule(CHwModule *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelDeSelectHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}
BOOL IHwModuleModelSubject::NotifySelectHwModule(CHwModule *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelSelectHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL IHwModuleModelSubject::NotifyRemoveHwModuleCtrl(CHwModuleCtrl *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelRemoveHwModuleCtrl(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}
BOOL IHwModuleModelSubject::NotifyRemoveHwModule(CHwModule *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelRemoveHwModule(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleModelSubject::NotifyHwModuleDisplayNameChange(CHwModule *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelHwModuleDisplayNameChange(pObj) == FALSE)
			return FALSE;
	}
	return TRUE;

}

void IHwModuleModelSubject::NotifyHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem *pObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		(*iter)->OnModelHwModuleCtrlUnitItemDisplayNameChange(pObj);
	}
}

void IHwModuleModelSubject::NotifyCreateHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		(*iter)->OnModelCreateHwModuleItem(pObj,pParentObj);
	}
}

void IHwModuleModelSubject::NotifyCreateHwModuleItem(CHwModule *pObj, CHwModuleItem *pParentObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		(*iter)->OnModelCreateHwModuleItem(pObj,pParentObj);
	}
}
void IHwModuleModelSubject::NotifyCreateHwModuleItem(CHwModuleCtrl *pObj, CHwModuleItem *pParentObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		(*iter)->OnModelCreateHwModuleItem(pObj,pParentObj);
	}
}
void IHwModuleModelSubject::NotifyCreateHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		(*iter)->OnModelCreateHwModuleItem(pObj,pParentObj);
	}
}
BOOL IHwModuleModelSubject::NotifyReplaceHwModule(CHwModule *pObj, CHwModule *pNewObj)
{
	vector<IHwModuleModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelReplaceHwModule(pObj,pNewObj) == FALSE)
			return FALSE;
	}
	return TRUE;
}



