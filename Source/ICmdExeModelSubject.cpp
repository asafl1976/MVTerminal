#include "StdAfx.h"
#include ".\icmdexemodelsubject.h"

ICmdExeModelSubject::ICmdExeModelSubject(void)
{

}

ICmdExeModelSubject::~ICmdExeModelSubject(void)
{

}

void ICmdExeModelSubject::RegisterObserver(ICmdExeModelObserver *pObserver)
{
	m_Observers.push_back(pObserver);
}

void ICmdExeModelSubject::UnregisterObserver(ICmdExeModelObserver *pObserver)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

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


BOOL ICmdExeModelSubject::NotifyLogLine(CString sLogLine)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelLogLine(sLogLine) == FALSE)
			return FALSE;
	}
	return TRUE;
}
BOOL ICmdExeModelSubject::NotifyUpdateCommand(CCmdExeCmd *pCommand)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelUpdateCommand(pCommand) == FALSE)
			return FALSE;
	}
	return TRUE;
}

BOOL ICmdExeModelSubject::NotifyAddCommand(CCmdExeCmd *pCommand)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelAddCommand(pCommand) == FALSE)
			return FALSE;
	}
	return TRUE;
}


BOOL ICmdExeModelSubject::NotifyLoopCountChange(int nLoopCount)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelLoopCountChange(nLoopCount) == FALSE)
			return FALSE;
	}
	return TRUE;

}

BOOL ICmdExeModelSubject::NotifyRemoveCommand(CCmdExeCmd *pCommand)
{
	vector<ICmdExeModelObserver*>::iterator iter, endIter;

	iter    = m_Observers.begin();
	endIter = m_Observers.end();
	for(; iter != endIter; iter++)
	{
		if((*iter)->OnModelRemoveCommad(pCommand) == FALSE)
			return FALSE;
	}
	return TRUE;
}
