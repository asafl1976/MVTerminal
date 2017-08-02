
// PlatformInfoPane.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "PlatformInfoPane.h"
#include "PlatformInfoSettingsDlg.h"
#include "PlatformInfoManager.h"
#include "FileOperations.h"

// CPlatformInfoPane

#define ID_PLATFORM_INFO_RELOAD_ITEM 100

IMPLEMENT_DYNCREATE(CPlatformInfoPane, CFormView)

CPlatformInfoPane::CPlatformInfoPane() : CFormView(CPlatformInfoPane::IDD)
{
	m_bInitialized = FALSE;

}

CPlatformInfoPane::~CPlatformInfoPane()
{
}

void CPlatformInfoPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLATFORM_INFO_TREE, m_platformTree);
	//DDX_Control(pDX, IDC_PLATFORM_INFO_BIN_FILES_TREE, m_binFilesTree);
}

BEGIN_MESSAGE_MAP(CPlatformInfoPane, CFormView)
	ON_NOTIFY(NM_DBLCLK, IDC_PLATFORM_INFO_TREE, OnNMDblclkPlatformInfoTree)
	ON_NOTIFY(NM_RCLICK, IDC_PLATFORM_INFO_TREE, &CPlatformInfoPane::OnNMRClickPlatformInfoTree)
	ON_BN_CLICKED(IDC_RESTORE_PLATFORM_INFO_DEFAULTS_BTN, &CPlatformInfoPane::OnBnClickedRestorePlatformInfoDefaultsBtn)
END_MESSAGE_MAP()


// CPlatformInfoPane diagnostics

#ifdef _DEBUG
void CPlatformInfoPane::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlatformInfoPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CPlatformInfoPane::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	if(m_bInitialized == FALSE)
	{
		LoadTree();
	}
}

void CPlatformInfoPane::LoadTree() 
{
	m_platformTree.CreateImageList();
	CString sPlatformInfoCategoriesPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath() + "\\PlatformInfo";
	
	CPlatformInfoManager::GetInstance().Init(sPlatformInfoCategoriesPath);
	
	m_platformTree.DeleteAllItems();
	int categoryCount = CPlatformInfoManager::GetInstance().GetCategoryCount();
	for(int category = 0; category < categoryCount; category++)
	{
		IPlatformInfoItem* pItem = CPlatformInfoManager::GetInstance().GetCategoryAtIndex(category);
		m_platformTree.AddPlatformInfoCategory(pItem);
	}
	m_bInitialized = TRUE;
}

HTREEITEM CPlatformInfoPane::GetMouseClickedItem()
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_platformTree.ScreenToClient(&point);
	 item = m_platformTree.HitTest(point, &uFlags);
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

// CPlatformInfoPane message handlers

void CPlatformInfoPane::RestoreDefaultsClicked()
{
	LoadTree();
}


void CPlatformInfoPane::OnNMDblclkPlatformInfoTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM  item = GetMouseClickedItem();
	if(item != NULL)
	{
		 IPlatformInfoItem	*pPlatformInfoItem = (IPlatformInfoItem*)m_platformTree.GetItemData(item);
			
		 if(pPlatformInfoItem != NULL)
		 {
			 CPlatformInfoSettingsDlg dlg;
			 dlg.m_delegate = this;
			 dlg.m_pPlatformInfo = pPlatformInfoItem;
		  	 dlg.DoModal();
	     }
	 }
	 //Set to 1 -> don't expand or collapse
	*pResult = 1;
}

void CPlatformInfoPane::HandleMenuSelection(UINT nMenuIndex)
{
    HTREEITEM  item = m_tiSelectedItem;
	switch(nMenuIndex)
	{	
	case ID_PLATFORM_INFO_RELOAD_ITEM:
		if(item != NULL)
		{ 
			m_bInitialized = FALSE;
			CPlatformInfoManager::GetInstance().Reset();
			LoadTree();
		}
		break;  
	default:
		break;
	}

}

BOOL CPlatformInfoPane::PreTranslateMessage(MSG* pMsg)
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

void CPlatformInfoPane::OnNMRClickPlatformInfoTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM  item = GetMouseClickedItem();
	if(item != NULL)
	{
		 CString itemDisplayName = m_platformTree.GetItemText(item);
		 if(CPlatformInfoManager::GetInstance().GetCategoryWithDisplayName(itemDisplayName) != NULL)
		 {
			CPoint		point;
			CNewMenu	itemMenu;

			GetCursorPos(&point);   
			itemMenu.CreatePopupMenu();

			itemMenu.InsertMenu(0,MF_BYPOSITION, ID_PLATFORM_INFO_RELOAD_ITEM ,  "Reload ",NULL);
 			itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
			m_tiSelectedItem = item;
		 }
	}

	*pResult = 0;
}

void CPlatformInfoPane::OnBnClickedRestorePlatformInfoDefaultsBtn()
{
	CStdioFile  file;

	int res = AfxMessageBox("Restore all XML Platform info files to original state (factory defaults)?",MB_YESNO|MB_ICONWARNING);
	if(res == IDNO)
		return;

	CString sSourcePath = "\\\\marvell\\israel\\groups\\eecv\\Utilities\\LabTools\\MVTerminal\\PlatformInfo";
	CString sPlatformInfoCategoriesPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
					
	CFileOperation fo;      
	fo.SetOverwriteMode(true); 

	if (!fo.Copy(sSourcePath, sPlatformInfoCategoriesPath)) // do Copy
	{
		fo.ShowError(); // if copy fails show error message
	}		
	CPlatformInfoManager::GetInstance().ReloadData();
	LoadTree();
}
