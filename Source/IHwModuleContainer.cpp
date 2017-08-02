#include "StdAfx.h"
#include ".\ihwmodulecontainer.h"
#include "HwModuleModel.h"

IHwModuleContainer::IHwModuleContainer(void)
{
}

IHwModuleContainer::~IHwModuleContainer(void)
{
	ReleaseHwModules();
}

void IHwModuleContainer::ReleaseHwModules()
{
	vector<CHwModule*>::iterator iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) != NULL)
		{
			DeSelectHwModule((*iter));
			NotifyRemoveHwModule(*iter);
			delete (*iter);
		}
	}
	m_HwModules.clear();
}

void   IHwModuleContainer::NotifyRemoveHwModule(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifyRemoveHwModule(pHwModule);
}
void   IHwModuleContainer::NotifyReplaceHwModule(CHwModule* pHwModule,CHwModule* pNewHwModule)
{
	CHwModuleModel::GetInstance().NotifyReplaceHwModule(pHwModule,pNewHwModule);
}

void   IHwModuleContainer::NotifyDeSelectHwModule(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifyDeSelectHwModule(pHwModule);
}
void   IHwModuleContainer::NotifySelectHwModule(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifySelectHwModule(pHwModule);
}
void   IHwModuleContainer::NotifyHwModuleDisplayNameChange(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifyHwModuleDisplayNameChange(pHwModule);
}

void IHwModuleContainer::AddHwModule(CHwModule* pHwModule)
{
	m_HwModules.push_back(pHwModule);
}

BOOL IHwModuleContainer::ReplaceHwModule(CHwModule* pHwModule, CString sNewItemName, CString sNewItemDisplayName)
{	
	// Don't replace to the same one
	if(sNewItemName == pHwModule->GetName())
	{
		//Unless display name is different (de-serialize issue...)
		if(sNewItemDisplayName == pHwModule->GetDisplayName())
		{
			return TRUE;
		}
	}
	//Create the new item
	CHwModuleInfo newItemInfo(sNewItemName,pHwModule->GetID(),
		pHwModule->GetInterfaceType(),pHwModule->GetParentUnit(),pHwModule->GetOwner());
	CHwModule *pNewHwModule = CHwModuleFactory::GetInstance().Create(&newItemInfo);
	
	if(pNewHwModule == NULL)
		return FALSE;

	//De-serialize issue...
	if(sNewItemDisplayName.IsEmpty() == FALSE)
		pNewHwModule->SetDisplayName(sNewItemDisplayName);

	return ReplaceHwModule(pHwModule,pNewHwModule);
}


BOOL IHwModuleContainer::ReplaceHwModule(CHwModule *pHwModule, CHwModule* pNewHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;

	NotifyReplaceHwModule(pHwModule,pNewHwModule);

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
		{
			delete (*iter);
			m_HwModules.erase(iter);
			break;
		}
	}
	if(iter == endIter)
		return FALSE;

	m_HwModules.push_back(pNewHwModule);
	if(pNewHwModule->Init() == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
	
BOOL IHwModuleContainer::DeSelectHwModule(CHwModule *pHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
		{
			pHwModule->OnDeSelect();
			NotifyDeSelectHwModule(pHwModule);
			break;
		}
	}
	if(iter == endIter)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IHwModuleContainer::SelectHwModule(CHwModule *pHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();
	//First, check if Hw Module is here
	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
		{
			pHwModule->OnSelect();
			NotifySelectHwModule(pHwModule);
			break;
		}
	}
	if(iter == endIter)
		return FALSE;

	return TRUE;
}

BOOL IHwModuleContainer::SetHwModuleDisplayName(CHwModule *pHwModule, CString sDisplayName)
{
	if(IsHwModuleExists(pHwModule) == TRUE)
	{
		pHwModule->SetDisplayName(sDisplayName);
        NotifyHwModuleDisplayNameChange(pHwModule);
		return TRUE;
	}
	return FALSE;
}


BOOL IHwModuleContainer::RemoveHwModule(CHwModule *pHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
		{
			DeSelectHwModule((*iter));
			NotifyRemoveHwModule(*iter);
			delete (*iter);
			m_HwModules.erase(iter);
			break;
		}
	}
	if(iter == endIter)
		return FALSE;

	return TRUE;
}

IHwModuleContainer*	IHwModuleContainer::GetHwModuleContainer(CHwModule* pHwModule)
{
	vector<CHwModule*>::iterator    iter, endIter;
	
	if(IsHwModuleExists(pHwModule) == TRUE)
		return this;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();
	for(; iter != endIter ; iter++) 
	{	
	   	if((*iter) != NULL)
		{
			IHwModuleContainer *pHwModuleContainer,*pTheContainer;
			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(*iter);
			if(pHwModuleContainer != NULL)
			{
				if((pTheContainer = pHwModuleContainer->GetHwModuleContainer(pHwModule)) != NULL)
					return pTheContainer;
			}
		}
	}

	return NULL;
}

BOOL  IHwModuleContainer::IsHwModuleExists(CHwModule* pHwModule)
{
	vector<CHwModule*>::iterator    iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(; iter != endIter ; iter++) 
	{	
	   	if((*iter) != NULL)
		{
			if((*iter) == pHwModule)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

int   IHwModuleContainer::GetHwModuleCount()
{
	return (int)m_HwModules.size();
}

CHwModule*	IHwModuleContainer::GetHwModuleByName(CString sName,BOOL bRecursive)
{
	vector<CHwModule*>::iterator    iter, endIter;
	int							    i = 0;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();


	for(; iter != endIter ; iter++) 
	{	
		CString sID, sDisplayName;
		sID = "0x" + (*iter)->GetID() + " - ";
		sDisplayName = (*iter)->GetDisplayName();
		sDisplayName = sDisplayName.Right(sDisplayName.GetLength() - sID.GetLength());
		if(sDisplayName == sName)
		{
			return (*iter);
		}
	}


	if(bRecursive == TRUE)
	{
		iter    = m_HwModules.begin();
		endIter = m_HwModules.end();

		//Look under sub containers
		for(; iter != endIter ; iter++) 
		{	
			IHwModuleContainer *pHwModuleContainer;
			CHwModule          *pHwModule;

			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(*iter);
			if(pHwModuleContainer != NULL)
			{

				if((pHwModule = pHwModuleContainer->GetHwModuleByName(sName,bRecursive)) != NULL)
				{
					return pHwModule;
				}
			}
		}
	}
	return NULL;
}

CHwModule*  IHwModuleContainer::GetHwModuleByID(CString sID,BOOL bRecursive)
{
	vector<CHwModule*>::iterator    iter, endIter;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(; iter != endIter ; iter++) 
	{	
	   	if((*iter) != NULL)
		{
			if((*iter)->GetID().MakeLower() == sID.MakeLower())
			{
				return (*iter);
			}
		}
	}
	if(bRecursive == TRUE)
	{
		//Look under sub containers
		for(; iter != endIter ; iter++) 
		{	
			IHwModuleContainer *pHwModuleContainer;
			CHwModule          *pHwModule;

			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(*iter);
			if(pHwModuleContainer != NULL)
			{
				if((pHwModule = pHwModuleContainer->GetHwModuleByID(sID,bRecursive)) != NULL)
				{
					return pHwModule;
				}
			}
		}
	}
	return NULL;
}
	
CHwModule*  IHwModuleContainer::GetHwModule(int nIndex)
{
	vector<CHwModule*>::iterator    iter, endIter;
	int							    i = 0;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(; iter != endIter ; iter++) 
	{	
		if(i == nIndex)
		{
			return (*iter);
		}
		i++;
	}
	return NULL;
}




BOOL  IHwModuleContainer::IsHwModuleExists(CString sID)
{
	vector<CHwModule*>::iterator    iter, endIter;
		
	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(; iter != endIter ; iter++) 
	{	
	   	if((*iter) != NULL)
		{
			if((*iter)->GetID().MakeLower()	== sID.MakeLower())
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL  IHwModuleContainer::CreateHwModules(vector<CHwModuleInfo*>& HWMInfo)
{
	vector<CHwModuleInfo*>::iterator iter, endIter;

	iter    = HWMInfo.begin();
	endIter = HWMInfo.end();

	for(; iter != endIter ; iter++)
	{
		if(IsHwModuleExists((*iter)->GetID()) == TRUE)
			continue;

		// Create UnknownModule modules
		CHwModule *pUnknownModule = CHwModuleFactory::GetInstance().Create((*iter));
		if(pUnknownModule == NULL)
		{
			AfxMessageBox("Error while creating Unknown hardware module!",NULL,MB_ICONERROR);
			continue;
		}
		m_HwModules.push_back(pUnknownModule);
		NotifyCreateHwModule(pUnknownModule);
	}
	return TRUE;
}

BOOL  IHwModuleContainer::DetectHwModules()
{
	vector<CHwModule*>::iterator    iter, endIter;
		
	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	for(; iter != endIter ; iter++)
	{
        IHwModuleContainer* pHwModuleContainer;
		pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(*iter);
		if(pHwModuleContainer != NULL)
		{
			if(pHwModuleContainer->DetectHwModules() == FALSE)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

CHwModuleItem*	IHwModuleContainer::GetSubHwModuleItem(list<CHwModuleItem>& ownerList)
{
	vector<CHwModule*>::iterator    iter, endIter;
	list<CHwModuleItem>::iterator   item;

	//Get the first element
	item = ownerList.begin();
	if(item == ownerList.end())
		return NULL;

	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();

	//Search for the item
	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetID() == (*item).GetID()) && ((*iter)->GetName() == (*item).GetName()))
		{
			//Remove the item from list
			ownerList.erase(item);
			if(ownerList.empty() == true)
			{
				return (*iter);
			}
			else
			{
		        IHwModuleContainer* pHwModuleContainer;
				pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(*iter);
				if(pHwModuleContainer != NULL)
				{
					//Pass the list on..
					return pHwModuleContainer->GetSubHwModuleItem(ownerList);
				}
			}

		}
	}
	return NULL;
}



CString  IHwModuleContainer::Serialize()
{
	CMarkup xmlWriter;
	vector<CHwModule*>::iterator    iter, endIter;
		
	iter    = m_HwModules.begin();
	endIter = m_HwModules.end();
	
	xmlWriter.AddElem("HwModules");
	xmlWriter.IntoElem();

	for(; iter != endIter ; iter++) 
	{	
	   	if((*iter) != NULL)
		{
			xmlWriter.AddSubDoc( (*iter)->Serialize() );
		}
	}
	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL  IHwModuleContainer::DeSerialize(CMarkup& xmlReader)
{
	CHwModule* pHwModule;
	CString    sName,sIFType,sDisplayName,sID,sParentUnit;

	if(xmlReader.FindElem("HwModules") == false)
		return FALSE;

	xmlReader.IntoElem();
    while ( xmlReader.FindElem("HwModule") )
	{
		sName        = xmlReader.GetAttrib("Name");
		sIFType      = xmlReader.GetAttrib("IFType");
		sDisplayName = xmlReader.GetAttrib("DisplayName");
		sID          = xmlReader.GetAttrib("ID");
		sParentUnit  = xmlReader.GetAttrib("ParentUnit");

		if((pHwModule = GetHwModuleByID(sID)) == NULL)
		{
			//Notify user of replace failure
			AfxMessageBox("Unable to set tree item " + sName + ", ID = " + sID + " (item could not be found during detection)",NULL,MB_ICONWARNING);
			continue;
		}
		if(pHwModule->GetName() != sName)
		{
			if(ReplaceHwModule(pHwModule,sName,sDisplayName) == FALSE)
			{
				return FALSE;
			}
		}
		// Get the newly replaced module (incase of mux that was created during this replace we
		// need to DeSerialize its sub HwModules as well
		if((pHwModule = GetHwModuleByID(sID)) == NULL)
		{
			return FALSE;
		}
		if(pHwModule->DeSerialize(xmlReader) == FALSE)
		{
			return FALSE;
		}
	}
	xmlReader.OutOfElem();
	return TRUE;
}



