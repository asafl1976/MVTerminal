#include "StdAfx.h"
#include ".\cmdexeitemcontainerfactory.h"
#include "CmdExeItemFactory.h"
#include "CmdExeItemContainer.h"
#include "HwModuleItemCmdExeItemContainer.h"

CCmdExeItemContainerFactory::CCmdExeItemContainerFactory(void)
{
	CCmdExeItemContainer *pItemContainer;
	CCmdExeItem			 *pItem; 

	//Create general cmd items prototype container
	pItemContainer = new CCmdExeItemContainer("General");
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Clear Window");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Delay");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Execute Command");

	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Check Result");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Wait For String");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Toggle COM Connection");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Board Reset");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Board Power On");
	pItemContainer->AddCmdItem(pItem);
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Board Power Off");
	pItemContainer->AddCmdItem(pItem);
	m_containers["General"] = pItemContainer;

	CHwModuleItemCmdExeItemContainer *pHwModuleCmdItemContainer;

	//Create USB-2-I2C cmd items prototype container
	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("USB-2-I2C");
	m_containers["USB-2-I2C"] = pHwModuleCmdItemContainer;

	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("I2C");
	m_containers["USB-2-I2C I2C"] = pHwModuleCmdItemContainer;

	//Create LPT-2-I2C cmd items prototype container
	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("LPT-2-I2C");
	m_containers["LPT-2-I2C"] = pHwModuleCmdItemContainer;

	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("I2C");
	m_containers["LPT-2-I2C I2C"] = pHwModuleCmdItemContainer;

	//Create SUB-20 cmd items prototype container
	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("SUB-20");
	m_containers["SUB-20"] = pHwModuleCmdItemContainer;

	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("I2C");
	m_containers["SUB-20 I2C"] = pHwModuleCmdItemContainer;

	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("Mux Channel");
	m_containers["Mux Channel"] = pHwModuleCmdItemContainer;

	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("Mux 9544");
	m_containers["Mux 9544"] = pHwModuleCmdItemContainer;

	//Create Unknown I2C cmd items prototype container
	pHwModuleCmdItemContainer = new CHwModuleItemCmdExeItemContainer("Unknown (I2C)");
	pItem = CCmdExeItemFactory::GetInstance().CreateItem("Unknown I2C - Read/Write");
	pHwModuleCmdItemContainer->AddCmdItem(pItem);
	m_containers["Unknown (I2C)"] = pHwModuleCmdItemContainer;
}

CCmdExeItemContainerFactory::~CCmdExeItemContainerFactory(void)
{
}

CCmdExeItemContainerFactory& CCmdExeItemContainerFactory::GetInstance()
{
	static CCmdExeItemContainerFactory obj;
	return obj;
}

CCmdExeItemContainer* CCmdExeItemContainerFactory::Create(CString sName)
{
	map<CString,CCmdExeItemContainer*>::iterator iter;
	
	iter = m_containers.find(sName);
	if(iter != m_containers.end())
	{
		return iter->second->Clone();
	}
	return NULL;
}

