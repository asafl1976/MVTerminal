#include "StdAfx.h"
#include "HwModuleCtrl.h"
#include "HwModuleCtrlUnit.h"
#include "DragDropTreeCtrl.h"
#include "IHwModuleContainer.h"
#include "HwModuleModel.h"
#include "HwModuleCtrlUnitItem.h"


CHwModuleCtrl::CHwModuleCtrl(void)
{
	m_bConnected = FALSE;
}

CHwModuleCtrl::~CHwModuleCtrl(void)
{
	ReleaseUnits();
	CHwModuleModel::GetInstance().NotifyRemoveHwModuleCtrl(this);
}


void    CHwModuleCtrl::ReleaseUnits()
{
	vector<CHwModuleCtrlUnit*>::iterator iter, endIter;

	iter    = m_units.begin();
	endIter = m_units.end();

	for(;iter != endIter; iter++)
	{
		if((*iter) != NULL)
			delete (*iter);
	}
	m_units.clear();
}

void CHwModuleCtrl::AddUnit(CHwModuleCtrlUnit* pUnit)
{
	m_units.push_back(pUnit);
}


CHwModuleCtrlUnit*  CHwModuleCtrl::GetUnit(int i)
{
	if(i >= (int)m_units.size())
		return NULL;

	return m_units[i];
}

BOOL   CHwModuleCtrl::Reset()
{
	return TRUE;
}

BOOL   CHwModuleCtrl::DetectHwModules()
{
    //Loop all units and detect hw modules
	int nUnit,nUnitCount = GetUnitCount();

	// Checks if there is any slave connected on the Ctrl bus...
	if(IsTargetConnected() == FALSE)
		return TRUE;

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		CHwModuleCtrlUnit *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			IHwModuleContainer* pHwModuleContainer;
			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
			if(pHwModuleContainer != NULL)
			{
				if(pHwModuleContainer->DetectHwModules() == FALSE)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

void   CHwModuleCtrl::ReleaseHwModules()
{
    //Loop all units and detect hw modules
	int nUnit,nUnitCount = GetUnitCount();

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		CHwModuleCtrlUnit *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			IHwModuleContainer* pHwModuleContainer;
			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
			if(pHwModuleContainer != NULL)
			{
				pHwModuleContainer->ReleaseHwModules();
			}
		}
	}
}


BOOL CHwModuleCtrl::DeSerialize(CMarkup& xmlReader)
{
	CHwModuleCtrlUnit* pUnit;
	CString            sName;

	xmlReader.IntoElem();
    while ( xmlReader.FindElem("HwModuleCtrlUnit") )
	{
		sName = xmlReader.GetAttrib("Name");
		if((pUnit = GetUnit(sName)) == NULL)
		{
			return FALSE;
		}
		if(pUnit->DeSerialize(xmlReader) == FALSE)
		{
			return FALSE;
		}
	}
	xmlReader.OutOfElem();
	return TRUE;
}

CString CHwModuleCtrl::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem(   "HwModuleCtrl");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "ID",GetID());
	xmlWriter.AddAttrib( "VendorID",GetVendorID());
	xmlWriter.AddAttrib( "UniqueID",GetUniqueID());

	xmlWriter.IntoElem();
    //Loop all units and detect hw modules
	int nUnit,nUnitCount = GetUnitCount();

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		CHwModuleCtrlUnit  *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			xmlWriter.AddSubDoc(pUnit->Serialize());
		}
	}
	xmlWriter.OutOfElem();

	return xmlWriter.GetDoc();
}

CHwModuleCtrlUnit*  CHwModuleCtrl::GetHwModuleCtrlUnitItemUnit(CHwModuleCtrlUnitItem *pItem)
{
	int nUnit,nUnitCount = GetUnitCount();

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		CHwModuleCtrlUnit *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			if(pUnit->IsUnitItemExist(pItem) == TRUE)
			{
				return pUnit;
			}
		}
	}
	return NULL;
}

IHwModuleContainer* CHwModuleCtrl::GetHwModuleContainer(CHwModule *pHwModule)
{
    //Loop all units and detect hw modules
	int nUnit,nUnitCount = GetUnitCount();

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		IHwModuleContainer *pHwModuleContainer,*pTheContainer;
		CHwModuleCtrlUnit *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
			if(pHwModuleContainer != NULL)
			{
				if((pTheContainer = pHwModuleContainer->GetHwModuleContainer(pHwModule)) != NULL)
				{
					return pTheContainer;
				}
			}
		}
	}
	return NULL;
}

CHwModuleCtrlUnit*  CHwModuleCtrl::GetUnit(CString sName)
{
	vector<CHwModuleCtrlUnit*>::iterator iter, endIter;

	iter    = m_units.begin();
	endIter = m_units.end();

	for(;iter != endIter; iter++)
	{
		if((*iter)->GetName() == sName)
			return (*iter);
	}
	return NULL;
}

CHwModuleItem*   CHwModuleCtrl::GetSubHwModuleItem(list<CHwModuleItem>& ownerList) 
{
	vector<CHwModuleCtrlUnit*>::iterator iter, endIter;
	list<CHwModuleItem>::iterator        item;

	//Get the first element
	item = ownerList.begin();
	if(item == ownerList.end())
		return NULL;

	iter    = m_units.begin();
	endIter = m_units.end();

	//Search for the item
	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetID() == (*item).GetID()) && ((*iter)->GetName() == (*item).GetName()))
		{
			//Remove the item from list
			ownerList.erase(item);
			if(ownerList.empty() == true)
				return (*iter);
			//Pass the list on..
			return (*iter)->GetSubHwModuleItem(ownerList);
		}
	}
	return NULL;
}

CHwModule*	CHwModuleCtrl::GetHwModule(CString sName)
{
    //Loop all units and detect hw modules
	int nUnit,nUnitCount = GetUnitCount();

	for(nUnit = 0; nUnit < nUnitCount; nUnit++)
	{
		IHwModuleContainer *pHwModuleContainer;
		CHwModule          *pHwModule;
		CHwModuleCtrlUnit *pUnit = GetUnit(nUnit);
		if(pUnit != NULL)
		{
			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
			if(pHwModuleContainer != NULL)
			{
				if((pHwModule = pHwModuleContainer->GetHwModuleByName(sName,TRUE)) != NULL)
				{
					return pHwModule;
				}
			}
		}
	}
	return NULL;
}

CHwModuleCtrlUnitItem*	CHwModuleCtrl::GetHwModuleCtrlUnitItem(CString sUnitName, CString sName)
{
	CHwModuleCtrlUnit *pUnit = GetUnit(sUnitName);

	if(pUnit != NULL)
	{
		return pUnit->GetItem(sName);
	}

	return NULL;
}









