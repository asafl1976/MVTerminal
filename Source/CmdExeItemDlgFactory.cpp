#include "StdAfx.h"
#include "CmdExeItemDlgFactory.h"
#include "CmdExeCmdItemClearWindowDlg.h"
#include "CmdExeCmdItemCheckResultDlg.h"
#include "CmdExeCmdItemExecuteCommandDlg.h"
#include "CmdExeCmdItemToggleComConnectionDlg.h"
#include "CmdExeCmdItemWaitForStringDlg.h"
#include "CmdExeCmdItemDelayDlg.h"
#include "CmdExeCmdItemUnknownI2CReadWriteDlg.h"

CCmdExeItemDlgFactory::CCmdExeItemDlgFactory(void)
{
	m_cmdExeCmdItemDlg["Clear Window"]          = new CCmdExeCmdItemClearWindowDlg();
	m_cmdExeCmdItemDlg["Check Result"]          = new CCmdExeCmdItemCheckResultDlg();
	m_cmdExeCmdItemDlg["Toggle COM Connection"] = new CCmdExeCmdItemToggleComConnectionDlg();
	m_cmdExeCmdItemDlg["Wait For String"]       = new CCmdExeCmdItemWaitForStringDlg();
	m_cmdExeCmdItemDlg["Delay"]                 = new CCmdExeCmdItemDelayDlg();
	m_cmdExeCmdItemDlg["Execute Command"]       = new CCmdExeCmdItemExecuteCommandDlg();

	//Hw Modules
	m_cmdExeCmdItemDlg["Unknown I2C - Read/Write"] = new CCmdExeCmdItemUnknownI2CReadWriteDlg();
	
}

CCmdExeItemDlgFactory::~CCmdExeItemDlgFactory(void)
{
	map<CString,CBasicItemDlg*>::iterator iter, endIter;

	iter    = m_cmdExeCmdItemDlg.begin();
	endIter = m_cmdExeCmdItemDlg.end();

	for(;iter != endIter; iter++)
	{
		delete (*iter).second;
	}
}

CCmdExeItemDlgFactory& CCmdExeItemDlgFactory::GetInstance()
{
	static CCmdExeItemDlgFactory obj;
	return obj;
}

CBasicItemDlg* CCmdExeItemDlgFactory::Create(CCmdExeCmdItem* pItem)
{
	map<CString,CBasicItemDlg*>::iterator iter;
	ICmdExeCmdItemDlg*                    pCmdItemDlg;
	CBasicItemDlg*						  pDlg;
	
	iter = m_cmdExeCmdItemDlg.find(pItem->GetName());
	if(iter != m_cmdExeCmdItemDlg.end())
	{
		pDlg = (iter->second)->Clone();
		pCmdItemDlg = dynamic_cast<ICmdExeCmdItemDlg*>(pDlg);
		if(pCmdItemDlg != NULL)
		{
			pCmdItemDlg->SetCmdItem(pItem);
			return pDlg;
		}
		delete pDlg;
	}
	return NULL;
}



