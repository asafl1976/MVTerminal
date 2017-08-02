#include "StdAfx.h"
#include "CmdExeItemFactory.h"
#include "CmdExeCmdItemClearWindow.h"
#include "CmdExeCmdItemDelay.h"
#include "CmdExeCmdItemExecuteCommand.h"
#include "CmdExeCmdItemCheckResult.h"
#include "CmdExeCmdItemWaitForString.h"
#include "CmdExeCmdItemToggleComConnection.h"
#include "CmdExeCmdItemUnknownI2CReadWrite.h"
#include "CmdExeCmdItemBoardReset.h"
#include "CmdExeCmdItemBoardPowerOn.h"
#include "CmdExeCmdItemBoardPowerOff.h"


CCmdExeItemFactory::CCmdExeItemFactory(void)
{
	//General
	m_cmdItems["Clear Window"]			= new CCmdExeCmdItemClearWindow();
	m_cmdItems["Delay"]			        = new CCmdExeCmdItemDelay();
	m_cmdItems["Execute Command"]	    = new CCmdExeCmdItemExecuteCommand();
	m_cmdItems["Check Result"]	        = new CCmdExeCmdItemCheckResult();
	m_cmdItems["Wait For String"]       = new CCmdExeCmdItemWaitForString();
	m_cmdItems["Toggle COM Connection"] = new CCmdExeCmdItemToggleComConnection();
	m_cmdItems["Board Reset"]           = new CCmdExeCmdItemBoardReset();
	m_cmdItems["Board Power On"]        = new CCmdExeCmdItemBoardPowerOn();
	m_cmdItems["Board Power Off"]       = new CCmdExeCmdItemBoardPowerOff();
	m_cmdItems["Unknown I2C - Read/Write"] = new CCmdExeCmdItemUnknownI2CReadWrite();
}

CCmdExeItemFactory::~CCmdExeItemFactory(void)
{
	map<CString,CCmdExeCmdItem*>::iterator iter,endIter;
	
	iter    = m_cmdItems.begin();
	endIter = m_cmdItems.end();

	for(;iter != endIter; iter++)
		if(iter->second != NULL)
			delete iter->second;
}

CCmdExeItemFactory& CCmdExeItemFactory::GetInstance()
{
	static CCmdExeItemFactory obj;
	return obj;
}

CCmdExeItemContainer* CCmdExeItemFactory::CreateItemContainer(CString sName)
{
	return NULL;
}

CCmdExeItem* CCmdExeItemFactory::CreateItem(CString sName)
{
	map<CString,CCmdExeCmdItem*>::iterator iter;
	
	iter = m_cmdItems.find(sName);
	if(iter != m_cmdItems.end())
	{
		return iter->second->Clone();
	}
	return NULL;
}

			