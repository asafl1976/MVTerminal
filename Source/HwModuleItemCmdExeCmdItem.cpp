#include "StdAfx.h"
#include "HwModuleItemCmdExeCmdItem.h"
#include "HwModuleItem.h"
#include "HwModuleModel.h"
#include <list>
using namespace std;

CHwModuleItemCmdExeCmdItem::CHwModuleItemCmdExeCmdItem(void)
{
}

CHwModuleItemCmdExeCmdItem::~CHwModuleItemCmdExeCmdItem(void)
{
}

BOOL CHwModuleItemCmdExeCmdItem::DeSerializeHwModuleItem(CMarkup& xmlReader)
{
	CString sName,sID,sDisplayName;

	//First, set HwModuleItem to NULL
	m_pHwModuleItem = NULL;

	xmlReader.IntoElem();
	if(xmlReader.FindElem("HwModuleInfo") == false)
		return FALSE;

	sName        = xmlReader.GetAttrib("Name");
	sDisplayName = xmlReader.GetAttrib("DisplayName");
	sID          = xmlReader.GetAttrib("ID");
	CHwModuleItem hwModuleItem(sName,sID,NULL);
	xmlReader.IntoElem();

	list<CHwModuleItem>    ownerList;
	hwModuleItem.DeSerializeOwnerList(xmlReader,ownerList);

	m_pHwModuleItem = CHwModuleModel::GetInstance().GetHwModuleItem(ownerList);
	if(m_pHwModuleItem == NULL)
	{
		AfxMessageBox("Unable to restore Hw Module item " + sDisplayName,NULL,MB_ICONERROR);
		return FALSE;
	}

	xmlReader.OutOfElem();
	xmlReader.OutOfElem();

	return TRUE;
}

BOOL CHwModuleItemCmdExeCmdItem::SerializeHwModuleItem(CMarkup& xmlWriter)
{
	xmlWriter.IntoElem();
	xmlWriter.AddElem( "HwModuleInfo");
	xmlWriter.AddAttrib( "Name",m_pHwModuleItem->GetName());
	xmlWriter.AddAttrib( "DisplayName",m_pHwModuleItem->GetDisplayName());
	xmlWriter.AddAttrib( "ID",m_pHwModuleItem->GetID());
	xmlWriter.IntoElem();

	xmlWriter.AddSubDoc( m_pHwModuleItem->SerializeOwnerList() );

	xmlWriter.OutOfElem();
	xmlWriter.OutOfElem();

	return TRUE;
}

BOOL CHwModuleItemCmdExeCmdItem::OnError(CString sErrorMsg)
{
	AfxMessageBox(sErrorMsg,NULL,MB_ICONERROR);
	return FALSE;
}


