// HWModulesPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "HwModulePane.h"
#include "HwModulePresenter.h"
#include "HwModuleCtrlUnit.h"
#include ".\hwmodulepane.h"
#include "HwModulePropertiesDlg.h"
#include "HwModuleCtrlI2CUnit.h"
#include "HwModuleCtrlUnitI2CSetFreqDlg.h"

#define  ID_HW_MODULE_SHOW_PROPERTIES		 (WM_USER+22)
#define  ID_HW_MODULE_REMOVE_HW_MODULE_ITEM  (WM_USER+23)

// Dynamic hw module type
#define  ID_HW_MODULE_TYPE               (WM_USER+50)


// Compute Device Class: this is used to get the tree contrl root icon
static const GUID GUID_DEVCLASS_COMPUTER = 
{ 0x4D36E966, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18} };

// Copy from HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\DeviceClasses
static const GUID GUID_DEVINTERFACE_LIST[] = 
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

	// GUID_DEVINTERFACE_DISK
	{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

	// GUID_DEVINTERFACE_HID, 
	{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
			 
	// GUID_NDIS_LAN_CLASS
	{ 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
};
// CHwModulesPane

IMPLEMENT_DYNCREATE(CHwModulesPane, CFormView)

CHwModulesPane::CHwModulesPane()
	: CFormView(CHwModulesPane::IDD)
{
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(LoadSettings(m_sAppDataPath + "\\HwModulesPane.xml") == FALSE) 
	{		
		CHwModuleModel::GetInstance().SetDetectHwModulesOnStartup(TRUE);
	}

	m_pHWMModel        = &CHwModuleModel::GetInstance();
    m_pHWMPresenter    = new CHwModulePresenter(m_pHWMModel, &m_HwModuleTreeCtrl);

	//To notify by SendMessage this pane 
	m_HwModuleTreeCtrl.SetParentWindow(this);
}

CHwModulesPane::~CHwModulesPane()
{
	SaveSettings(m_sAppDataPath + "\\HwModulesPane.xml");
	if(m_pHWMPresenter != NULL)
		delete m_pHWMPresenter;
}

void CHwModulesPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HW_MODULE_TREE_CTRL, m_HwModuleTreeCtrl);
}

BEGIN_MESSAGE_MAP(CHwModulesPane, CFormView) 
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_NOTIFY(NM_DBLCLK, IDC_HW_MODULE_TREE_CTRL, OnNMDblclkHwModuleTreeCtrl)
	ON_NOTIFY(NM_RCLICK, IDC_HW_MODULE_TREE_CTRL, OnNMRclickHwModuleTreeCtrl)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HW_MODULE_TREE_CTRL, OnTvnSelchangedHwModuleTreeCtrl)
	ON_BN_CLICKED(IDC_HW_MODULE_PANE_DET_ON_STARTUP, OnBnClickedHwModulePaneDetOnStartup)
END_MESSAGE_MAP()

LRESULT CHwModulesPane::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) 
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
		PDEV_BROADCAST_HANDLE pDevHnd;
		PDEV_BROADCAST_OEM pDevOem;
		PDEV_BROADCAST_PORT pDevPort;
		PDEV_BROADCAST_VOLUME pDevVolume;

		switch( pHdr->dbch_devicetype ) 
		{
			case DBT_DEVTYP_DEVICEINTERFACE:
				pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				UpdateDevice(pDevInf, wParam);
				break;
			case DBT_DEVTYP_HANDLE:
				pDevHnd = (PDEV_BROADCAST_HANDLE)pHdr;
				break;
			case DBT_DEVTYP_OEM:
				pDevOem = (PDEV_BROADCAST_OEM)pHdr;
				break;
			case DBT_DEVTYP_PORT:
				pDevPort = (PDEV_BROADCAST_PORT)pHdr;
				break;
			case DBT_DEVTYP_VOLUME:
				pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;
				break;
		}
	}
	return 0;
}

void CHwModulesPane::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
	ASSERT(lstrlen(pDevInf->dbcc_name) > 4);
	CString szDevId = pDevInf->dbcc_name+4;
	int idx = szDevId.ReverseFind(_T('#'));
	ASSERT( -1 != idx );
	szDevId.Truncate(idx);
	szDevId.Replace(_T('#'), _T('\\'));
	szDevId.MakeUpper();
	CString szClass;
	idx = szDevId.Find(_T('\\'));
	ASSERT(-1 != idx );
	szClass = szDevId.Left(idx);
	CString szLog;
	CString szTmp;
	if ( DBT_DEVICEARRIVAL == wParam ) 
	{
		m_HwModuleTreeCtrl.AddUsbDeviceMsg(szDevId);
	} 
	else 
	{
		m_HwModuleTreeCtrl.RemoveUsbDeviceMsg(szDevId);
	}
}
// CHwModulesPane diagnostics

#ifdef _DEBUG
void CHwModulesPane::AssertValid() const
{
	CFormView::AssertValid();
}

void CHwModulesPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


void CHwModulesPane::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	CheckDlgButton(IDC_HW_MODULE_PANE_DET_ON_STARTUP,CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup());
	
	/////////////////////// Register USB device notification ///////////////
	HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) 
	{
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if( !hDevNotify ) 
		{
			AfxMessageBox("Can't register USB device notification!");
		}
	}

	m_HwModuleTreeCtrl.CreateImageList();
}


void CHwModulesPane::OnNMDblclkHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM  item = GetMouseClickedItem();

	 if(item != NULL)
	 {
         CHwModuleItem* pHwModuleItem = (CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(item);
		 if(pHwModuleItem != NULL)
		 {
			 m_HwModuleTreeCtrl.ShowHwModuleItemDlg(pHwModuleItem);
	     }
	 }
	 //Set to 1 -> don't exapnd or collapse
	*pResult = 1;
}

void CHwModulesPane::HandleMenuSelection(UINT nMenuIndex)
{
    HTREEITEM          item = NULL;
	CHwModule*         pHwModule;
	CHwModuleCtrl*     pHwModuleCtrl;
	CHwModuleCtrlUnit* pHwModuleCtrlUnit;
	CString		       sNewHwModuleName,sHwModuleCtrlMenuItem;

	item = m_tiSelectedItem;
	switch(nMenuIndex)
	{	
	case ID_HW_MODULE_REMOVE_HW_MODULE_ITEM:
		if(item != NULL)
		{
			if((pHwModule = (CHwModule*)m_HwModuleTreeCtrl.GetItemData(item)) != NULL)
			{
				if(MessageBox("Remove hardware module '" + pHwModule->GetDisplayName() + "'?",NULL,MB_YESNO|MB_ICONQUESTION) == IDYES)
				{
					if(m_HwModuleTreeCtrl.RemoveHwModule(pHwModule) == FALSE)
					{
						AfxMessageBox("Error while removing hw module!",NULL,MB_ICONERROR);
					}
				}
			}
		}
		return;
	case ID_HW_MODULE_SHOW_PROPERTIES:
		//Hw Module
		pHwModule = dynamic_cast<CHwModule*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(item));
		if(pHwModule != NULL)
		{
			CHwModulePropertiesDlg dlg;
			dlg.SetDisplayName(pHwModule->GetDisplayName());
			dlg.SetName(pHwModule->GetName());
			dlg.SetInterfaceName(pHwModule->GetInterfaceTypeName());
			dlg.SetID(pHwModule->GetID());
			if(dlg.DoModal() == IDOK)
			{
				CString sDisName = dlg.GetDisplayName();
				if(sDisName.IsEmpty() == FALSE)
				{
					if(m_HwModuleTreeCtrl.SetHwModuleDisplayName(pHwModule,sDisName) == FALSE)
					{
						AfxMessageBox("Failed to modify display name!",NULL,MB_ICONERROR);
					}
				}
			}
			return;
		}
	
		break;
	default:
		if(item != NULL)
		{
			//Hw Module
			pHwModule = dynamic_cast<CHwModule*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(item));
			if(pHwModule != NULL)
			{
				sNewHwModuleName = GetMenuItem(nMenuIndex);
				if(sNewHwModuleName.IsEmpty() == TRUE)
					return;
				if(sNewHwModuleName == pHwModule->GetName())
				{
					return;
				}
				m_HwModuleTreeCtrl.ReplaceHwModule(pHwModule,sNewHwModuleName);
				return;
			}
			//Hw Module Ctrl
			pHwModuleCtrl = dynamic_cast<CHwModuleCtrl*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(item));
			if(pHwModuleCtrl != NULL)
			{
				switch(nMenuIndex)
				{
				case ID_HW_MODULE_CTRL_MENU_SAVE:
					OnMenuClickedHwModulePaneSaveConTableBtn(pHwModuleCtrl);
					break;
				case ID_HW_MODULE_CTRL_MENU_LOAD:
					OnMenuClickedHwModulePaneLoadConTableBtn(pHwModuleCtrl);
					break;
				case ID_HW_MODULE_CTRL_MENU_REFRESH:
					OnMenuClickedHwModulePaneRefreshTreeBtn(pHwModuleCtrl);
					break;
				case ID_HW_MODULE_CTRL_MENU_RESET_SUB20:
					OnMenuClickedHwModulePaneResetSub20Btn(pHwModuleCtrl);
					break;
					
				default:
					break;
				}
				
			}
			//Hw Module Ctrl unit
			pHwModuleCtrlUnit = dynamic_cast<CHwModuleCtrlUnit*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(item));
			if(pHwModuleCtrlUnit != NULL)
			{
				switch(nMenuIndex)
				{
				case ID_HW_MODULE_CTRL_UNIT_I2C_SET_FREQ:
					OnMenuClickedHwModulePaneSetI2CFreq(pHwModuleCtrlUnit);
					break;
				default:
					break;
				}
				
			}
		}
	}
}
void CHwModulesPane::OnNMRclickHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	 HTREEITEM				   hitem;
	 CPoint					   point;
	 CNewMenu				   itemMenu;
	 int				       i,mMenuItem = 0,nCheck = 0;
 	 CBitmap			       bmpImage;

	 hitem = m_HwModuleTreeCtrl.GetSelectedItem();
	 if(hitem == NULL)
		 return;
	 m_tiSelectedItem = hitem;
	 //If Hw module was clicked
	 CHwModule* pHwModule = dynamic_cast<CHwModule*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(hitem));
	 if(pHwModule != NULL)
	 {
	    vector<CString>*		   pRepItemList;
	    vector<CString>::iterator iter, endIter;

		pRepItemList = m_HwModuleTreeCtrl.GetHwModuleMenuReplaceItems(pHwModule);
		if(pRepItemList == NULL)
			return; 
		GetCursorPos(&point);   
		itemMenu.CreatePopupMenu();
		iter    = pRepItemList->begin();
		endIter = pRepItemList->end();
		i = 0;
		for(;iter != endIter; iter++)
		{
			nCheck = 0;
			if((*iter) == pHwModule->GetName())
			{
				nCheck = MF_CHECKED;
			}
			itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION|nCheck, ID_HW_MODULE_TYPE + i ,  (*iter));
			m_menuMap[ID_HW_MODULE_TYPE + i] = (*iter);
			i++;
		}
		itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
		bmpImage.LoadBitmap(IDB_REMOVE_BMP); 
		itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, ID_HW_MODULE_REMOVE_HW_MODULE_ITEM ,  "Remove ",&bmpImage);
		itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
		bmpImage.DeleteObject();

		bmpImage.LoadBitmap(IDB_PROPERTIES_BMP); 
		itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, ID_HW_MODULE_SHOW_PROPERTIES ,  "Properties",&bmpImage);
 		itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
		bmpImage.DeleteObject();
	 }
 	 //If Hw module ctrl was clicked
	 CHwModuleCtrl* pHwModuleCtrl = dynamic_cast<CHwModuleCtrl*>((CHwModuleItem*)m_HwModuleTreeCtrl.GetItemData(hitem));
	 if(pHwModuleCtrl != NULL)
	 {	 
		vector<CHwModuleCtrlMenuItem>*			 pCtrlItemList;
		vector<CHwModuleCtrlMenuItem>::iterator  iter, endIter;

		pCtrlItemList = m_HwModuleTreeCtrl.GetHwModuleCtrlMenuItems(pHwModuleCtrl);
		if(pCtrlItemList == NULL)
			return; 
		GetCursorPos(&point);   
		itemMenu.CreatePopupMenu();
		iter    = pCtrlItemList->begin();
		endIter = pCtrlItemList->end();
		i = 0;
		mMenuItem = 0;
		for(;iter != endIter; iter++)
		{
			if((*iter).m_sMenuItem == "SEPARATOR")
			{
				itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
				continue;
			}
			bmpImage.LoadBitmap((*iter).m_nMenuItemImage); 
			itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, (*iter).m_nMenuItemID ,  (*iter).m_sMenuItem,&bmpImage);
			bmpImage.DeleteObject();
			i++;
		}
		//itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
		//bmpImage.LoadBitmap(IDB_PROPERTIES_BMP); 
		//itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, ID_HW_MODULE_SHOW_PROPERTIES ,  "Properties",&bmpImage);
 		itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
		bmpImage.DeleteObject();
	 }
 	 //If Hw module ctrl unit was clicked
	 CHwModuleCtrlUnit* pHwModuleCtrlUnit = dynamic_cast<CHwModuleCtrlUnit*>((CHwModuleCtrlUnit*)m_HwModuleTreeCtrl.GetItemData(hitem));
	 if(pHwModuleCtrlUnit != NULL)
	 {

 	    vector<CHwModuleCtrlMenuItem>*	          pUnitItemList;
	    vector<CHwModuleCtrlMenuItem>::iterator   iter, endIter;

		pUnitItemList = m_HwModuleTreeCtrl.GetHwModuleCtrlUnitMenuItems(pHwModuleCtrlUnit);
		if(pUnitItemList == NULL)
			return; 

		GetCursorPos(&point);   
		itemMenu.CreatePopupMenu();
		iter    = pUnitItemList->begin();
		endIter = pUnitItemList->end();
		i = 0;
		mMenuItem = 0;
		for(;iter != endIter; iter++)
		{
			if((*iter).m_sMenuItem == "SEPARATOR")
			{
				itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
				continue;
			}
			bmpImage.LoadBitmap((*iter).m_nMenuItemImage); 
			itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, (*iter).m_nMenuItemID ,  (*iter).m_sMenuItem,&bmpImage);
			bmpImage.DeleteObject();
			i++;
		}
 		itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
		bmpImage.DeleteObject();
	 }
	*pResult = 0;
}


HTREEITEM CHwModulesPane::GetMouseClickedItem()
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_HwModuleTreeCtrl.ScreenToClient(&point);
	 item = m_HwModuleTreeCtrl.HitTest(point, &uFlags);
	 if(item)
	 { 
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
	 		break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			return item;
		 default:
			break;
	   	}
	 }	
	 return NULL;
}

CString   CHwModulesPane::GetMenuItem(int nIndex)
{
	map<int,CString>::iterator iter,endIter;
	iter    = m_menuMap.begin();
	endIter = m_menuMap.end();
	for(;iter != endIter;iter++)
	{
		if(iter->first == nIndex)
			return iter->second;
	}
	return CString("");
}


BOOL CHwModulesPane::PreTranslateMessage(MSG* pMsg)
{
    // CXxxx can be a CFormView, Cdialog, or CPropertyPage class.
    //return CFormView::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_COMMAND)
	{		
		//The high-order word specifies the notification code if the message is from a control. 
		//If the message is from an accelerator, this value is 1. If the message is from a menu, 
		//this value is zero. 
		//The low-order word specifies the identifier of the menu item, control, or accelerator.
		if(((((UINT)pMsg->wParam)>>16) & 0xFFFF) == 0)
		{
			HandleMenuSelection((UINT)pMsg->wParam & 0xFFFF);
		}
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}
void CHwModulesPane::OnTvnSelchangedHwModuleTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	

	*pResult = 0;
}

void CHwModulesPane::OnBnClickedHwModulePaneDetOnStartup()
{
	CHwModuleModel::GetInstance().SetDetectHwModulesOnStartup(!CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup());
}


void CHwModulesPane::OnMenuClickedHwModulePaneSetI2CFreq(CHwModuleCtrlUnit *pUnit)
{
	CHwModuleCtrlI2CUnit* pI2CUnit = dynamic_cast<CHwModuleCtrlI2CUnit*>(pUnit);
	if(pI2CUnit == NULL)
		return;

	CHwModuleCtrlUnitI2CSetFreqDlg  dlg;
	dlg.SetHwModuleCtrlI2CUnit(pI2CUnit);
	dlg.DoModal();

}

void CHwModulesPane::OnMenuClickedHwModulePaneLoadConTableBtn(CHwModuleCtrl *pCtrl)
{
	char strFilter[] = { "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||" };
	CFileDialog fileDlg(TRUE, ".xml", "HwModuleSettings.xml", OFN_HIDEREADONLY, strFilter);

	if(fileDlg.DoModal() == IDOK)
	{
		CString	 sFilePath;
		sFilePath = fileDlg.GetPathName();
		if(sFilePath.IsEmpty() == FALSE)
		{
			if(m_HwModuleTreeCtrl.LoadHwModuleModelSettings(sFilePath,pCtrl) == FALSE)
			{
				AfxMessageBox("Error while loading settings!",NULL,MB_ICONERROR);
				return;
			}
		}
	}
}


void CHwModulesPane::OnMenuClickedHwModulePaneSaveConTableBtn(CHwModuleCtrl *pCtrl)
{
	char strFilter[] = { "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||" };
	CFileDialog fileDlg(FALSE, ".xml", "HwModuleSettings.xml", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);

	if(fileDlg.DoModal() == IDOK)
	{
		CString	 sFilePath;
		sFilePath = fileDlg.GetPathName();
		if(sFilePath.IsEmpty() == FALSE)
		{
			if(m_HwModuleTreeCtrl.SaveHwModuleModelSettings(sFilePath,pCtrl) == FALSE)
			{
				AfxMessageBox("Error while saving connection table!",NULL,MB_ICONERROR);
				return;
			}
		}
	}
}

void CHwModulesPane::OnMenuClickedHwModulePaneResetSub20Btn(CHwModuleCtrl *pCtrl)
{
    if(m_HwModuleTreeCtrl.ResetHwModuleCtrl(pCtrl) == FALSE)
	{
		AfxMessageBox("Error while refershing hardware module tree!",NULL,MB_ICONERROR);
		return;
	}		
}

void CHwModulesPane::OnMenuClickedHwModulePaneRefreshTreeBtn(CHwModuleCtrl *pCtrl)
{
    if(m_HwModuleTreeCtrl.RefreshHwModuleCtrl(pCtrl) == FALSE)
	{
		AfxMessageBox("Error while refershing hardware module tree!",NULL,MB_ICONERROR);
		return;
	}		
}

BOOL CHwModulesPane::DeSerialize(CMarkup& xmlReader)
{	
	CString  sFile;

	if(xmlReader.FindElem("HwModulesPane") == false)
		return FALSE;

	BOOL bDetectHwModulesOnStartup = (xmlReader.GetAttrib("DetectHwModuledOnStartup") == "0")? FALSE:TRUE;
	CHwModuleModel::GetInstance().SetDetectHwModulesOnStartup(bDetectHwModulesOnStartup);

	xmlReader.IntoElem();

	if(CHwModuleModel::GetInstance().DeSerialize(xmlReader) == FALSE)
		return FALSE;

	xmlReader.OutOfElem();
	return TRUE;
}

CString CHwModulesPane::Serialize()
{
	CMarkup		xmlWriter;

	xmlWriter.AddElem("HwModulesPane");
	xmlWriter.AddAttrib("DetectHwModuledOnStartup",CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup());
	xmlWriter.IntoElem();
	
	xmlWriter.AddSubDoc( CHwModuleModel::GetInstance().Serialize() );

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

void	CHwModulesPane::RemoveAllHwModuleCtrls()
{
	CHwModuleModel::GetInstance().ReleaseHwModuleCtrls();
}

int	   CHwModulesPane:: GetHwModuleCtrlsCount()
{
	return CHwModuleModel::GetInstance().GetHwModuleCtrlCount();
}

BOOL CHwModulesPane::LoadSettings(CString sFile)
{	
	CMarkup	 xmlReader;

	if(xmlReader.Load(sFile) == false)
		return FALSE;
	if(xmlReader.FindElem("HwModulesPane") == false)
		return FALSE;
	BOOL bDetectHwModulesOnStartup = (xmlReader.GetAttrib("DetectHwModuledOnStartup") == "0")? FALSE:TRUE;
	CHwModuleModel::GetInstance().SetDetectHwModulesOnStartup(bDetectHwModulesOnStartup);
	return TRUE;
}

BOOL CHwModulesPane::SaveSettings(CString sFile)
{	
	CStdioFile  file;
	CMarkup		xmlWriter;

	if(file.Open(sFile,CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox("Failed to open file " + sFile + "!",NULL,MB_ICONERROR);
		return FALSE;
	}
	xmlWriter.AddElem("HwModulesPane");
	xmlWriter.AddAttrib("DetectHwModuledOnStartup",CHwModuleModel::GetInstance().GetDetectHwModulesOnStartup());
	file.WriteString(xmlWriter.GetDoc());
	file.Close();
	return TRUE;
}


