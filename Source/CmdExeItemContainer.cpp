#include "StdAfx.h"
#include "CmdExeItemContainer.h"
#include "CmdExeCmdItem.h"
#include "CmdExeItemFactory.h"

CCmdExeItemContainer::CCmdExeItemContainer(CString sName)
{
	m_sName  = sName;
	m_sDisplayName = m_sName;
}

CCmdExeItemContainer::~CCmdExeItemContainer(void)
{
	map<CString,CCmdExeItem*>::iterator iter,endIter;
	
	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(;iter != endIter;iter++)
	{
		if(iter->second != NULL)
			delete iter->second;
	}
}
void CCmdExeItemContainer::SetDisplayName(CString sDisplayName)
{
	m_sDisplayName = sDisplayName;
}

CString CCmdExeItemContainer::GetName() 
{
	return m_sName;
}

void CCmdExeItemContainer::AddCmdItem(CCmdExeItem *pItem)
{
	m_cmdItems[pItem->GetName()] = pItem;
}

int CCmdExeItemContainer::GetCmdItemCount()
{
	return (int)m_cmdItems.size();
}

void CCmdExeItemContainer::GetCmdItemNameList(vector<CString>& cmdList)
{
	map<CString,CCmdExeItem*>::iterator iter,endIter;
	
	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(;iter != endIter;iter++)
	{
		cmdList.push_back(iter->first);
	}
}

CCmdExeItemContainer* CCmdExeItemContainer::Clone()
{
	CCmdExeItemContainer			   *pNewContainer;
	map<CString,CCmdExeItem*>::iterator iter,endIter;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	pNewContainer = new CCmdExeItemContainer(GetName());
	for(;iter != endIter;iter++)
	{
		pNewContainer->AddCmdItem(iter->second->Clone());
	}
	return pNewContainer;
}

CCmdExeItem*  CCmdExeItemContainer::CreateCmdItem(CString sName)
{
	map<CString,CCmdExeItem*>::iterator iter;
	
	iter = m_cmdItems.find(sName);
	if(iter != m_cmdItems.end())
	{
		return iter->second->Clone();
	}
	return NULL;
}
