#include "StdAfx.h"
#include ".\muxhwmodulechannel.h"
#include "MuxHwModule.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleFactory.h"
#include "HwModuleModel.h"

CMuxHwModuleChannel::CMuxHwModuleChannel(int nChannel, CMuxHwModule *pMux)
{
	CString sChannel;

	m_sName        = "Mux Channel";
	m_nChannel     = nChannel;
	m_pMux	       = pMux;
	m_pOwner	   = pMux;
	m_sID.Format("%d",m_nChannel);
	//Set display name
	m_sDisplayName.Format("Channel#%d",m_nChannel);
}

CMuxHwModuleChannel::~CMuxHwModuleChannel(void)
{
}

void   CMuxHwModuleChannel::NotifyCreateHwModule(CHwModule* pHwModule)
{

	CHwModuleModel::GetInstance().NotifyCreateHwModuleItem(pHwModule,this);
}

BOOL  CMuxHwModuleChannel::DetectHwModules(vector<CHwModuleInfo*>& HwModuleInfo,CHwModuleItem* pOwner)
{
	CHwModuleCtrlUnit			     *pParentUnit;

	if((pParentUnit = m_pMux->GetParentUnit()) == NULL)
		return FALSE;

	IHwModuleContainer *pHwModuleContainer;
	pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pParentUnit);
	if(pHwModuleContainer != NULL)
	{
		//Let the parent unit detect...
		return pHwModuleContainer->DetectHwModules(HwModuleInfo,pOwner);
	}
	return FALSE;
}


BOOL  CMuxHwModuleChannel::DetectHwModules()
{
	vector<CHwModuleInfo*>   HwModuleInfo;

	// First, select this channel (and all upper channels)
	if(Select() == FALSE)
		return FALSE;

	if( DetectHwModules(HwModuleInfo,this) == FALSE )
	{
		return FALSE;
	}

	return CreateHwModules(HwModuleInfo);
}

BOOL CMuxHwModuleChannel::UpperChannelIsHwModuleExists(CString sID)
{
	CMuxHwModuleChannel*    pUpperChannel;

	if(IsHwModuleExists(sID) == TRUE)
		return TRUE;

	if((pUpperChannel = m_pMux->GetUpperMuxChannel()) != NULL)
	{
		if(pUpperChannel->UpperChannelIsHwModuleExists(sID) == TRUE)
			return TRUE;
	}
	else
	{
		//if not connected to upper channel --> connected to unit
		CHwModuleCtrlUnit  *pParentUnit;

		if((pParentUnit = m_pMux->GetParentUnit()) == NULL)
			return FALSE;

		IHwModuleContainer *pHwModuleContainer;
		pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pParentUnit);
		if(pHwModuleContainer != NULL)
		{
			return pHwModuleContainer->IsHwModuleExists(sID);
		}
	}
	return FALSE;
}

BOOL  CMuxHwModuleChannel::CreateHwModules(vector<CHwModuleInfo*>& HWMInfo)
{
	vector<CHwModuleInfo*>::iterator iter, endIter;

	iter    = HWMInfo.begin();
	endIter = HWMInfo.end();

	for(; iter != endIter ; iter++)
	{
		if(UpperChannelIsHwModuleExists((*iter)->GetID()) == TRUE)
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

BOOL CMuxHwModuleChannel::DeSelectHwModule(CHwModule *pHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;
	
	// First, select this channel (and all upper channels)
	if(Select() == FALSE)
		return FALSE;

	//Check if HwModule is under this mux channel
	if(IsHwModuleExists(pHwModule->GetID()) == TRUE)
	{
		//Hw HwModule is under this mux channel --> de-select it
		pHwModule->OnDeSelect();
		m_pMux->DeSelectChannels();
		NotifyDeSelectHwModule(pHwModule);
		return TRUE;
	}

	return FALSE;
}


BOOL CMuxHwModuleChannel::SelectHwModule(CHwModule *pHwModule)
{
	vector<CHwModule*>::iterator iter, endIter;
	
	// First, select this channel (and all upper channels)
	if(Select() == FALSE)
		return FALSE;

	//Check if HwModule is under this mux channel
	if(IsHwModuleExists(pHwModule->GetID()) == TRUE)
	{
		pHwModule->OnSelect();
		NotifySelectHwModule(pHwModule);
		return TRUE;
	}
	return FALSE;
}



BOOL CMuxHwModuleChannel::Select()
{
	CMuxHwModuleChannel*    pUpperChannel;
	if((pUpperChannel = m_pMux->GetUpperMuxChannel()) != NULL)
	{
		pUpperChannel->Select();
	}
	//Select this channel
	return m_pMux->SelectChannel(m_nChannel);
}

CString  CMuxHwModuleChannel::Serialize()
{
	CString  sCh;
	CMarkup	 xmlWriter;

	sCh.Format("%d",m_nChannel);
	xmlWriter.AddElem( "MuxHwModuleChannel");
	xmlWriter.AddAttrib( "Channel",sCh);
	xmlWriter.IntoElem();

	xmlWriter.AddSubDoc( IHwModuleContainer::Serialize() );

	xmlWriter.OutOfElem();
	
	return xmlWriter.GetDoc();
}

BOOL	CMuxHwModuleChannel::DeSerialize(CMarkup& xmlReader)
{
	xmlReader.IntoElem();
	if(IHwModuleContainer::DeSerialize(xmlReader) == FALSE)
		return FALSE;
	xmlReader.OutOfElem();
	return TRUE;
}
