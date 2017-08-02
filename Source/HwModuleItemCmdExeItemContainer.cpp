#include "StdAfx.h"
#include "HwModuleItemCmdExeItemContainer.h"
#include "HwModuleItemCmdExeCmdItem.h"

CHwModuleItemCmdExeItemContainer::CHwModuleItemCmdExeItemContainer(CString sName):
CCmdExeItemContainer(sName)
{
}

CHwModuleItemCmdExeItemContainer::~CHwModuleItemCmdExeItemContainer(void)
{
}

CCmdExeItemContainer* CHwModuleItemCmdExeItemContainer::Clone()
{
	CHwModuleItemCmdExeItemContainer       *pNewContainer;
	map<CString,CCmdExeItem*>::iterator iter,endIter;

	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	pNewContainer = new CHwModuleItemCmdExeItemContainer(GetName());
	for(;iter != endIter;iter++)
	{
		pNewContainer->AddCmdItem(iter->second->Clone());
	}
	return pNewContainer;
}

void  CHwModuleItemCmdExeItemContainer::SetHwModuleItem(CHwModuleItem* pHwModuleItem)
{
	map<CString,CCmdExeItem*>::iterator iter,endIter;
	
	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(;iter != endIter;iter++)
	{
		CHwModuleItemCmdExeCmdItem*  pItem;
		pItem = dynamic_cast<CHwModuleItemCmdExeCmdItem*>(iter->second);
		if(pItem != NULL)
		{
			pItem->SetHwModuleItem(pHwModuleItem);
		}
	}
}



