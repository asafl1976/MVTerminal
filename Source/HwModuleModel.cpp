#include "StdAfx.h"
#include "HwModuleModel.h"
#include "LPT2I2CHWModuleCtrl.h"
#include "SUB20HWModuleCtrl.h"
#include "IHwModuleContainer.h"
#include "HwModuleCtrlUnit.h"
#include "IHWModuleModel.h"
#include "libsub.h"
#include "MVTerminal.h"

CHwModuleModel::CHwModuleModel(void)
{
	m_bInitialized = FALSE;
}

CHwModuleModel::~CHwModuleModel(void)
{
	ReleaseHwModuleCtrls();
}


void   CHwModuleModel::ReleaseHwModuleCtrls()
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;
	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();
	for(;iter != endIter;iter++)
	{
		if((*iter) != NULL)
		{
			delete (*iter);
		}
	}
	m_HwModuleCtrls.clear();
}

void   CHwModuleModel::ReleaseHwModules()
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if((*iter) != NULL)
		{
			(*iter)->ReleaseHwModules();
		}
	}
}

int    CHwModuleModel::GetHwModuleCtrlCount()
{
	return (int)m_HwModuleCtrls.size();
}

CHwModuleCtrl*  CHwModuleModel::GetHwModuleCtrl(int i)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;
	int									  nCount = 0;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if(nCount == i)
			return (*iter);
		nCount++;
	}
	return NULL;
}


BOOL  CHwModuleModel::FindSub20Devices(vector<sub_handle>& sub20handles)
{
	sub_device dev = NULL;
	sub_handle handle;

	while((dev = sub_find_devices(dev)) != NULL)
	{
		handle = sub_open (dev );
		if(handle != NULL)
		{
			sub20handles.push_back(handle);
		}
	}
	return TRUE;
}

CLPT2I2CHwModuleCtrl*   CHwModuleModel::CreateLPT2I2CModuleCtrl()
{
	CLPT2I2CHwModuleCtrl *pHwModuleCtrl;

	///// Add non detectable LPT-2-I2C //////////////
	pHwModuleCtrl = new CLPT2I2CHwModuleCtrl();
    if(pHwModuleCtrl->Init() == TRUE)
	{
		m_HwModuleCtrls.push_back(pHwModuleCtrl);
		return pHwModuleCtrl;
	}
	delete pHwModuleCtrl;
	return NULL;
}



CString CHwModuleModel::GetSub20UniqueID(sub_handle handle)
{
	CString sUniqueID;
    if(handle != NULL)
	{
		char buf[8];
		if(sub_get_serial_number(handle,buf,8) >= 0)
		{
			sUniqueID = buf;
		}
	}
	return sUniqueID; 
}

CSUB20HwModuleCtrl*   CHwModuleModel::CreateSub20UsbHwModuleCtrl(sub_handle handle,CString sUniqueID)
{
	CSUB20HwModuleCtrl	 *pHwModuleCtrl;

    pHwModuleCtrl = new CSUB20HwModuleCtrl(handle,sUniqueID);
	if(pHwModuleCtrl->Init() == TRUE)
	{
		m_HwModuleCtrls.push_back(pHwModuleCtrl);
		return pHwModuleCtrl;
	}
	delete pHwModuleCtrl;
	return NULL;
}

void   CHwModuleModel::CreateSub20UsbHwModuleCtrls(vector<sub_handle>&  sub20handles)
{
	vector<sub_handle>::iterator  iter, endIter;	

	iter    = sub20handles.begin();
	endIter = sub20handles.end();
	for(; iter != endIter; iter++)
	{
		CString sUniqueID = GetSub20UniqueID(*iter);
		
		if(IsUsbDeviceExist(USB_ID_SUB_20,sUniqueID) == TRUE)
			continue;

		if(CreateSub20UsbHwModuleCtrl(*iter,sUniqueID) == NULL)
		{
			AfxMessageBox("Failed to create SUB-20 device with ID = " + sUniqueID);
		}
	}
}


void   CHwModuleModel::CreateUsbHwModuleCtrl()
{
	vector<sub_handle>			  sub20handles;
	vector<sub_device>::iterator  iter, endIter;

	///// SUB-20 //////////////
	FindSub20Devices(sub20handles);
	CreateSub20UsbHwModuleCtrls(sub20handles);

}

void   CHwModuleModel::Init(BOOL bDetHwModule)
{
	if(m_bInitialized == TRUE)
		return;

	m_bInitialized = TRUE;

#if 0 //Remove support for LPT-2-I2C
	CreateLPT2I2CModuleCtrl(); 
#endif

	CreateUsbHwModuleCtrl();

	if(bDetHwModule == TRUE)
	{
		DetectHwModules();
	}
}

CHwModule*  CHwModuleModel::GetHwModule(CString sHwModuleCtrlName, int nHwModuleCtrlID, CString sName)
{
	CString sID;

	sID.Format("%x",nHwModuleCtrlID);
	CHwModuleCtrl *pHwModuleCtrl = GetHwModuleCtrl(sHwModuleCtrlName,sID);
	if(pHwModuleCtrl == NULL)
	{
		return NULL;
	}
	CHwModule *pHwModule;
	if((pHwModule = pHwModuleCtrl->GetHwModule(sName)) != NULL)
	{	
		return pHwModule;
	}
    return NULL;
}
CHwModuleCtrlUnitItem*  CHwModuleModel::GetHwModuleCtrlUnitItem(CString sHwModuleCtrlName, int nHwModuleCtrlID,CString sUnitName , CString sName)
{
	CString sID;

	sID.Format("%x",nHwModuleCtrlID);
	CHwModuleCtrl *pHwModuleCtrl = GetHwModuleCtrl(sHwModuleCtrlName,sID);
	CHwModuleCtrlUnitItem *pHwModuleCtrlUnitItem;

	if((pHwModuleCtrlUnitItem = pHwModuleCtrl->GetHwModuleCtrlUnitItem(sUnitName,sName)) != NULL)
	{
		return pHwModuleCtrlUnitItem;
	}

    return NULL;
}

void   CHwModuleModel::GetHwModuleCtrlList(vector<CHwModuleCtrl*>& HWCtrlList)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		HWCtrlList.push_back((*iter));
	}
}

CHwModuleCtrl* CHwModuleModel::GetHwModuleCtrl(CString sName, CString sID)
{
	CHwModuleCtrl				    *pCtrl = NULL;
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();
	for(;iter != endIter;iter++)
	{
		if((*iter)->GetName() == sName)
		{
			//If passed ID is not 0xffffffff, check for ID match
			if(sID.MakeLower() != "ffffffff")
			{
				if(sID == (*iter)->GetUniqueID())
				{
					return (*iter);
				}
			}
			//Else, return the last ctrl that matches the name
			else
			{
				pCtrl = (*iter);
			}
		}
	}

	return pCtrl;
}


BOOL   CHwModuleModel::DetectHwModules()
{
	vector<CHwModuleInfo*>			 HWModulesInfo;
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if((*iter) != NULL)
		{
			if((*iter)->DetectHwModules() == FALSE)
			{
				AfxMessageBox((*iter)->GetName() + " error while detecting HW modules!\n",NULL,MB_ICONERROR); 
			}
		}
	}
	return TRUE;
}

CHwModuleModel& CHwModuleModel::GetInstance()
{
    /* Allocate system object once on the heap */
    static CHwModuleModel *pObj = new CHwModuleModel();
    return *pObj;
}

IHwModuleContainer* CHwModuleModel::GetHwModuleContainer(CHwModule *pHwModule)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		IHwModuleContainer *pTheContainer;
		if((pTheContainer = (*iter)->GetHwModuleContainer(pHwModule)) != NULL)
		{
			return pTheContainer;
		}
	}
	return NULL;
}

BOOL  CHwModuleModel::ReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName)
{
	IHwModuleContainer *pHwModuleContainer;

	pHwModuleContainer = GetHwModuleContainer(pHwModule);
	if(pHwModuleContainer != NULL)
	{
		return pHwModuleContainer->ReplaceHwModule(pHwModule,sNewHwModuleName);
	}
	return FALSE;
}


BOOL   CHwModuleModel::DeSelectHwModule(CHwModule *pHwModule)
{
	IHwModuleContainer *pHwModuleContainer;

	pHwModuleContainer = GetHwModuleContainer(pHwModule);
	if(pHwModuleContainer != NULL)
	{
		return pHwModuleContainer->DeSelectHwModule(pHwModule);
	}
	return FALSE;
}

BOOL   CHwModuleModel::SelectHwModule(CHwModule *pHwModule)
{
	IHwModuleContainer *pHwModuleContainer;

	pHwModuleContainer = GetHwModuleContainer(pHwModule);
	if(pHwModuleContainer != NULL)
	{
		return pHwModuleContainer->SelectHwModule(pHwModule);
	}
	return FALSE;
}

BOOL   CHwModuleModel::RemoveHwModuleCtrl(CHwModuleCtrl *pHwModuleCtrl)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if((*iter) == pHwModuleCtrl)
		{
			delete (*iter);
			m_HwModuleCtrls.erase(iter);
			break;
		}
	}
	return TRUE;
}

BOOL   CHwModuleModel::RemoveHwModule(CHwModule *pHwModule)
{
	IHwModuleContainer *pHwModuleContainer;

	pHwModuleContainer = GetHwModuleContainer(pHwModule);
	if(pHwModuleContainer != NULL)
	{
		return pHwModuleContainer->RemoveHwModule(pHwModule);
	}
	return FALSE;
}

BOOL   CHwModuleModel::IsUsbDeviceExist(CString sVendorID,CString sUniqueID)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetVendorID() == sVendorID) && ((*iter)->GetUniqueID() == sUniqueID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

CHwModuleCtrl*   CHwModuleModel::GetHwModuleCtrl(CString sName,CString sVendorID,CString sUniqueID)
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetName() == sName) && ((*iter)->GetVendorID() == sVendorID) && ((*iter)->GetUniqueID() == sUniqueID))
		{
			return (*iter);
		}
	}
	return NULL;

}

BOOL   CHwModuleModel::AddSub20UsbDevice()
{
	vector<sub_handle>	  sub20handles;
	CHwModuleCtrl		 *pHwModuleCtrl;

	FindSub20Devices(sub20handles);
	if(sub20handles.size() == 0)
	{
		AfxMessageBox("USB SUB-20 device was detected but could not be found using SUB-20 driver scan function!");
		return FALSE;
	}
	CString sUniqueID = GetSub20UniqueID(*sub20handles.begin());
	if(IsUsbDeviceExist(USB_ID_SUB_20,sUniqueID) == TRUE)
	{
		AfxMessageBox("SUB-20 (" + sUniqueID + ") Already exists!");
		return FALSE;
	}
	//Create thr first new found device
	pHwModuleCtrl = CreateSub20UsbHwModuleCtrl(*sub20handles.begin(),sUniqueID);
	if(pHwModuleCtrl->DetectHwModules() == FALSE)
	{
		AfxMessageBox("Failed to detect hw modules!");
		return FALSE;
	}

	//Patch - Enable board power ctrl
	CMVTerminalApp *pApp = (CMVTerminalApp*)AfxGetApp();
	if((pApp->IsBoardResetStartegySet() == FALSE) || 
	   (pApp->IsBoardPowerStartegySet() == FALSE))
	{
		if(((CMVTerminalApp*)AfxGetApp())->SetBoardPowerCtrlHwModuleItem(pHwModuleCtrl->GetUnit("SUB-20 GPIO")) == FALSE)
		{
			AfxMessageBox("Warning - SUB-20 created successfully but cannot be used to control board power!");
		}
	}
	return TRUE;
}




BOOL   CHwModuleModel::AddUsbDevice(CString sDevID)
{
	CString sVendorID,sUniqueID;

	sVendorID = sDevID.Left( sDevID.ReverseFind('\\') + 1);
	sUniqueID = sDevID.Right(sDevID.GetLength() - sDevID.ReverseFind('\\') - 1);

	if(sVendorID == USB_ID_SUB_20)
	{
		return AddSub20UsbDevice();
	}
	
	return TRUE;
}

BOOL   CHwModuleModel::RemoveUsbDevice(CString sDevID)
{
	CString sVendorID,sUniqueID;

	sVendorID = sDevID.Left( sDevID.ReverseFind('\\') + 1);
	sUniqueID = sDevID.Right(sDevID.GetLength() - sDevID.ReverseFind('\\') - 1);

	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if(((*iter)->GetVendorID() == sVendorID) && ((*iter)->GetUniqueID() == sUniqueID))
		{
			delete (*iter);
			m_HwModuleCtrls.erase(iter);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL   CHwModuleModel::IsHwModuleCtrlExist(CHwModuleCtrl *pCtrl) 
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if((*iter) == pCtrl)
		{
			return TRUE;
		}
	}
	return FALSE;
}

CHwModuleItem*   CHwModuleModel::GetHwModuleItem(list<CHwModuleItem>& ownerList) 
{
	vector<CHwModuleCtrl*>::iterator iter,endIter;
	list<CHwModuleItem>::iterator    item;

	//Get the first element
	item = ownerList.begin();
	if(item == ownerList.end())
		return NULL;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

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


CHwModuleCtrlUnit*   CHwModuleModel::GetHwModuleCtrlUnitItemUnit(CHwModuleCtrlUnitItem *pItem) 
{
	CHwModuleCtrlUnit               *pUnit;
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();

	for(;iter != endIter;iter++)
	{
		if((pUnit = (*iter)->GetHwModuleCtrlUnitItemUnit(pItem)) != NULL)
		{
			return pUnit;
		}
	}
	return NULL;
}

BOOL  CHwModuleModel::SetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName)
{
	CHwModuleCtrlUnit *pUnit;

	if((pUnit = GetHwModuleCtrlUnitItemUnit(pItem)) == NULL)
		return FALSE;
	return pUnit->SetItemDisplayName(pItem,sDisplayName);
}

BOOL   CHwModuleModel::RefreshHwModuleCtrl(CHwModuleCtrl *pCtrl) 
{
	if(IsHwModuleCtrlExist(pCtrl) == TRUE)
	{
		return pCtrl->DetectHwModules();
	}
	return FALSE;
}

BOOL   CHwModuleModel::ResetHwModuleCtrl(CHwModuleCtrl *pCtrl) 
{
	if(IsHwModuleCtrlExist(pCtrl) == TRUE)
	{
		return pCtrl->Reset();
	}
	return FALSE;
}

BOOL   CHwModuleModel::SetHwModuleDisplayName(CHwModule* pHwModule, CString sDisplayName)
{
	IHwModuleContainer *pHwModuleContainer;

	pHwModuleContainer = GetHwModuleContainer(pHwModule);
	if(pHwModuleContainer != NULL)
	{
		return pHwModuleContainer->SetHwModuleDisplayName(pHwModule,sDisplayName);
	}
	return FALSE;
}

//All ctrls
BOOL   CHwModuleModel::SaveHwModuleModelSettings(CString sFile)
{
	CStdioFile  file;
	CMarkup		xmlWriter;
	CString		xmlData;

	if(file.Open(sFile,CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox("Failed to open file " + sFile + "!",NULL,MB_ICONERROR);
		return FALSE;
	}

	xmlWriter.AddSubDoc( Serialize() );
	file.WriteString( xmlWriter.GetSubDoc() );
	return TRUE;
}

//Single ctrl
BOOL   CHwModuleModel::SaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	CStdioFile  file;
	CMarkup		xmlWriter;
	CString		xmlData;

	if(file.Open(sFile,CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox("Failed to open file " + sFile + "!",NULL,MB_ICONERROR);
		return FALSE;
	}

	xmlWriter.AddElem("HwModuleModelSettings");//root
	xmlWriter.IntoElem();

	xmlWriter.AddSubDoc( pCtrl->Serialize() );

	xmlWriter.OutOfElem();
	xmlData = xmlWriter.GetDoc();
	file.WriteString(xmlData);
	file.Close();
	return TRUE;
}

//All ctrls
CString	CHwModuleModel::Serialize()
{	
	CMarkup							 xmlWriter;
	vector<CHwModuleCtrl*>::iterator iter,endIter;

	xmlWriter.AddElem("HwModuleModelSettings");//root
	xmlWriter.IntoElem();

	iter    = m_HwModuleCtrls.begin();
	endIter = m_HwModuleCtrls.end();
	for(;iter != endIter;iter++)
	{
		if(xmlWriter.AddSubDoc( (*iter)->Serialize() ) == false)
			return FALSE;
	}
	xmlWriter.OutOfElem();

	return xmlWriter.GetDoc();
}


//All ctrls
BOOL   CHwModuleModel::LoadHwModuleModelSettings(CString sFile)
{
	CMarkup         xmlReader;
	CString			sName,sID,sUniqueID,sVendorID;

	//Open and load the file
	xmlReader.Load(sFile);

	return DeSerialize(xmlReader);
}

//Single ctrl
BOOL   CHwModuleModel::LoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	CMarkup         xmlReader;
	CString			sName,sID,sUniqueID,sVendorID;
	CHwModuleCtrl  *pHwModuleCtrl;

	//First, release current Ctrl and re-detect
	pCtrl->ReleaseHwModules();
	DetectHwModules();

	//Open and load the file
	xmlReader.Load(sFile);

	if(xmlReader.FindElem("HwModuleModelSettings") == false)
		return FALSE;

	xmlReader.IntoElem();
	while ( xmlReader.FindElem("HwModuleCtrl") )
	{
		sName     = xmlReader.GetAttrib("Name");
		sID       = xmlReader.GetAttrib("ID");
		sVendorID = xmlReader.GetAttrib("VendorID");
		sUniqueID = xmlReader.GetAttrib("UniqueID");
		if((pHwModuleCtrl = GetHwModuleCtrl(sName,sVendorID,sUniqueID)) == NULL)
		{
			continue;
		}
		if( (pCtrl->GetName()     == pHwModuleCtrl->GetName())     &&
			(pCtrl->GetID()       == pHwModuleCtrl->GetID())       &&
			(pCtrl->GetVendorID() == pHwModuleCtrl->GetVendorID()) &&
			(pCtrl->GetUniqueID() == pHwModuleCtrl->GetUniqueID()))
		{
			if(pHwModuleCtrl->DeSerialize(xmlReader) == FALSE)
			{
				return FALSE;
			}
			xmlReader.OutOfElem();
			return TRUE;
		}
	}
	xmlReader.OutOfElem();
	return FALSE;
}

BOOL   CHwModuleModel::DeSerialize(CMarkup & xmlReader)
{
	CString			sName,sID,sUniqueID,sVendorID;
	CHwModuleCtrl  *pHwModuleCtrl;

	if(xmlReader.FindElem("HwModuleModelSettings") == false)
		return FALSE;

	//First, release all hw modules and re-detect
	ReleaseHwModules();
	//DetectHwModules();
	xmlReader.IntoElem();
	while ( xmlReader.FindElem("HwModuleCtrl") )
	{
		sName     = xmlReader.GetAttrib("Name");
		sID       = xmlReader.GetAttrib("ID");
		sVendorID = xmlReader.GetAttrib("VendorID");
		sUniqueID = xmlReader.GetAttrib("UniqueID");
		if((pHwModuleCtrl = GetHwModuleCtrl(sName,sVendorID,sUniqueID)) == NULL)
		{
			return FALSE;
		}
		if(pHwModuleCtrl->DeSerialize(xmlReader) == FALSE)
		{
			return FALSE;
		}

	}
	xmlReader.OutOfElem();
	return TRUE;
}


