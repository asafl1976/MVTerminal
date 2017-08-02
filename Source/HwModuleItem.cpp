#include "StdAfx.h"
#include "HwModuleItem.h"
#include "HwModuleFactory.h"
#include "DragDropTreeCtrl.h"

CHwModuleItem::CHwModuleItem(void)
{
	m_pOwner = NULL;
}

CHwModuleItem::CHwModuleItem(CString sName, CString sID, CHwModuleItem* pOwner)
{
	m_sName        = sName;
	m_sDisplayName = sName;
	m_sID		   = sID;
	m_pOwner       = pOwner;
}


CHwModuleItem::~CHwModuleItem(void)
{
}

CString CHwModuleItem::SerializeOwnerList()
{
	CMarkup xmlWriter;
	int		nIntoElem = 0;

	xmlWriter.AddElem( "HwModuleItemOwnerChain");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "ID",GetID());
	xmlWriter.IntoElem();

	CHwModuleItem* pOwner = GetOwner();
	while(pOwner != NULL)
	{
		xmlWriter.AddElem( "Owner");
		xmlWriter.AddAttrib( "Name",pOwner->GetName());
		xmlWriter.AddAttrib( "ID",pOwner->GetID());
		xmlWriter.IntoElem();
		nIntoElem++;

		pOwner = pOwner->GetOwner();
	}
	while(nIntoElem != 0)
	{
		xmlWriter.OutOfElem();
		nIntoElem--;
	}

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

void CHwModuleItem::DeSerializeOwnerList(CMarkup& xmlReader, list<CHwModuleItem>& ownerList)
{
	int		nIntoElem = 0;
	CString sName,sID;

	if(xmlReader.FindElem( "HwModuleItemOwnerChain") == false)
		return;

	sName = xmlReader.GetAttrib( "Name");
	sID   = xmlReader.GetAttrib( "ID");
	CHwModuleItem thisHwModuleItem(sName,sID,NULL);
	ownerList.push_front(thisHwModuleItem);
	xmlReader.IntoElem();

	while(xmlReader.FindElem( "Owner") == true)
	{
		sName = xmlReader.GetAttrib( "Name");
		sID   = xmlReader.GetAttrib( "ID");
		CHwModuleItem hwModuleItem(sName,sID,NULL);
		ownerList.push_front(hwModuleItem);
		xmlReader.IntoElem();
		nIntoElem++;
	}
	while(nIntoElem != 0)
	{
		xmlReader.OutOfElem();
		nIntoElem--;
	}
	xmlReader.OutOfElem();

}



