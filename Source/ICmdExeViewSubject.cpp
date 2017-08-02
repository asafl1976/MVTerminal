#include "StdAfx.h"
#include ".\icmdexeviewsubject.h"

ICmdExeViewSubject::ICmdExeViewSubject(void)
{
}

ICmdExeViewSubject::~ICmdExeViewSubject(void)
{
}

void ICmdExeViewSubject::RegisterObserver(ICmdExeViewObserver *pObserver)
{
	m_Observers.push_back(pObserver);
}

void ICmdExeViewSubject::UnregisterObserver(ICmdExeViewObserver *pObserver)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

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

BOOL ICmdExeViewSubject::NotifyUpdateCommand(CCmdExeCmd* pCommand)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewUpdateCommand(pCommand) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ICmdExeViewSubject::NotifyAddCommand(CCmdExeCmd* pCommand)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewAddCommand(pCommand) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ICmdExeViewSubject::NotifyLoadCommands(CMarkup& xmlReader)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewLoadCommands(xmlReader) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ICmdExeViewSubject::NotifySaveCommands(CMarkup& xmlWriter)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewSaveCommands(xmlWriter) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ICmdExeViewSubject::NotifyRemoveAllCommands()
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewRemoveAllCommands() == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ICmdExeViewSubject::NotifyRemoveCommand(CCmdExeCmd* pCommand)
{
	vector<ICmdExeViewObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnViewRemoveCommand(pCommand) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}
