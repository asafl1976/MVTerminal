#include "StdAfx.h"
#include "muxhwmodule.h"
#include "HwModuleAccessStartegy.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleFactory.h"
#include "MVTerminal.h"
#include "MuxHwModuleAccessStartegy.h"
#include "HwModuleModel.h"
#include "MuxHwModuleChannel.h"


CMuxHwModule::CMuxHwModule(void)
{
	m_pAccessStrategy  = NULL;
	m_nActiveChannel   = -1;
	m_pUpperMuxChannel = NULL;
}

CMuxHwModule::~CMuxHwModule(void)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		delete (*iter);
		NotifyRemoveHwModule(*iter);
	}
	m_MuxChannels.clear();
}

int CMuxHwModule::GetChannelCount()
{
	return (int)m_MuxChannels.size();
}


void	CMuxHwModule::AddChannel(CMuxHwModuleChannel* pChannel)
{
	m_MuxChannels.push_back(pChannel);
}

CMuxHwModule::CMuxHwModule(CHwModuleInfo* pInfo) : CHwModule(pInfo)
{
	m_pAccessStrategy  = NULL;
	m_pUpperMuxChannel = NULL;
}



BOOL   CMuxHwModule::SelectChannel(int nChannel)
{
	m_nActiveChannel = nChannel;
	if(m_pAccessStrategy->SelectChannel(nChannel) == FALSE)
	{
		m_nActiveChannel = -1;
		return FALSE;
	}
	return TRUE;
}

BOOL   CMuxHwModule::DeSelectChannels()
{
	m_nActiveChannel = -1;
	if(m_pAccessStrategy != NULL)
	{
		return m_pAccessStrategy->DeSelectChannels();
	}
	return FALSE;
}

BOOL CMuxHwModule::Init()
{
	return DetectHwModules();
}   

void   CMuxHwModule::NotifyCreateHwModule(CHwModule* pHwModule)
{
	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(pHwModule,this);
}

BOOL   CMuxHwModule::DetectHwModules()
{
	CMuxHwModuleChannel *pChannel;

	if(m_MuxChannels.size() == 0)//New mux is created 
	{
		for(int nChannel = 0; nChannel < m_nChannelCount ; nChannel++)
		{
			pChannel = new CMuxHwModuleChannel(nChannel,this);
			AddChannel(pChannel);
			NotifyCreateHwModule(pChannel);
			pChannel->DetectHwModules();
		}
	}
	else//Refresh detection
	{
		for(int nChannel = 0; nChannel < m_nChannelCount ; nChannel++)
		{
			
			pChannel = GetChannel(nChannel);
			if(pChannel  != NULL)
			{
				pChannel->DetectHwModules();
			}
		}
	}
	//Close all mux channles
	DeSelectChannels();
	return TRUE;
}

BOOL   CMuxHwModule::DetectHwModules(vector<CHwModuleInfo*>& HwModuleInfo,CHwModuleItem* pOwner)
{
	return TRUE;
}


BOOL   CMuxHwModule::ReplaceHwModule(CString sNewItemName, CHwModule *pHwModule)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	// Check if its one of the mux channels 
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->ReplaceHwModule(pHwModule,sNewItemName) == TRUE)
			return TRUE;
	}
	return FALSE;
}


BOOL   CMuxHwModule::RemoveHwModule(CHwModule *pHwModule)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	// Check if its one of the mux channels 
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
		{
			delete (*iter);
			m_MuxChannels.erase(iter);
			NotifyRemoveHwModule(pHwModule);
			return TRUE;
		}
	}

	// If not the one of the channels...
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->RemoveHwModule(pHwModule) == TRUE)
			return TRUE;
	}
	return FALSE;
}



BOOL   CMuxHwModule::IsHwModuleExists(CString sID)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->IsHwModuleExists(sID) == TRUE)
			return TRUE;
	}

	return FALSE;
}

BOOL   CMuxHwModule::SelectHwModule(CHwModule *pHwModule)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->SelectHwModule(pHwModule) == TRUE)
			return TRUE;
	}
	return FALSE;
}

IHwModuleContainer*	    CMuxHwModule::GetHwModuleContainer(CHwModule* pHwModule)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	// Check if its one of the mux channels 
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) == pHwModule)
			return this;

		IHwModuleContainer *pTheContainer;
		if((pTheContainer = (*iter)->GetHwModuleContainer(pHwModule)) != NULL)
			return pTheContainer;
	}
	return FALSE;
}

CMuxHwModuleChannel* CMuxHwModule::GetChannel(int nNumber)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	// Check if its one of the mux channels 
	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->GetChannelNumber() == nNumber)
			return (*iter);
	}
	return FALSE;
}

BOOL   CMuxHwModule::DeSelectHwModule(CHwModule *pHwModule)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		if((*iter)->DeSelectHwModule(pHwModule) == TRUE)
			return TRUE;
	}

	return FALSE;

}

CString  CMuxHwModule::Serialize()
{
	CMarkup							       xmlWriter;
	vector<CMuxHwModuleChannel*>::iterator iter, endIter; 

	xmlWriter.AddElem( "HwModule");
	xmlWriter.AddAttrib( "Name",GetName());
	xmlWriter.AddAttrib( "IFType",GetInterfaceTypeName());
	xmlWriter.AddAttrib( "DisplayName",GetDisplayName());
	xmlWriter.AddAttrib( "ID",GetID());
	xmlWriter.AddAttrib( "ParentUnit",m_pParentUnit->GetName());

	xmlWriter.IntoElem();

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();
	for(;iter != endIter; iter++)
	{
		xmlWriter.AddSubDoc( (*iter)->Serialize() );
	}
	xmlWriter.OutOfElem();

	return xmlWriter.GetDoc();
}

BOOL  CMuxHwModule::DeSerialize(CMarkup& xmlReader)
{
	CString				 sNumber;
	int					 nNumber;
	CMuxHwModuleChannel *pChannel;

	xmlReader.IntoElem();
    while ( xmlReader.FindElem("MuxHwModuleChannel") )
	{
		sNumber = xmlReader.GetAttrib("Channel");
		sscanf(	(LPCSTR)sNumber,"%d",&nNumber);

		if((pChannel = GetChannel(nNumber)) == NULL)
		{
			return FALSE;
		}
		if(pChannel->DeSerialize(xmlReader) == FALSE)
		{
			return FALSE;
		}
	}
	xmlReader.OutOfElem();
	return TRUE;
}


int   CMuxHwModule::GetHwModuleCount()
{
	return (int)m_MuxChannels.size();
}

CHwModule*  CMuxHwModule::GetHwModule(int nIndex)
{
	return GetChannel(nIndex);
}


CHwModuleItem*   CMuxHwModule::GetSubHwModuleItem(list<CHwModuleItem>& ownerList) 
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter;
	list<CHwModuleItem>::iterator          item;

	//Get the first element
	item = ownerList.begin();
	if(item == ownerList.end())
		return NULL;

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();

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

CHwModule*	CMuxHwModule::GetHwModuleByName(CString sName,BOOL bRecursive)
{
	vector<CMuxHwModuleChannel*>::iterator iter, endIter;
	int							           i = 0;
	CHwModule							  *pHwModule = NULL;

	iter    = m_MuxChannels.begin();
	endIter = m_MuxChannels.end();

	for(; iter != endIter ; iter++) 
	{
        if((pHwModule = (*iter)->GetHwModuleByName(sName,bRecursive)) != NULL)
		{
			return pHwModule;
		}
	}
	return NULL;
}





