// HWMTreeCtrlView.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "HwModuleTreeView.h"
#include "IHwModuleItemDlg.h"
#include "BasicItemDlg.h"
#include "HwModule.h"
#include "HwModuleCtrl.h"
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlUnitItem.h"
#include "HwModuleFactory.h"
#include "HwModuleDlgFactory.h"
#include "HwModulePane.h"

// CHWMTreeCtrlView


CHwModuleTreeView::CHwModuleTreeView()
{
	int               nHwModuleProtoCount,i;
	vector<CString>*  pHwModuleReplaceItemsVec;

	pHwModuleReplaceItemsVec = new vector<CString>;
	nHwModuleProtoCount = CHwModuleFactory::GetInstance().GetPrototypeCount();
	for(i = 0; i < nHwModuleProtoCount; i++)
	{
		CString sName;
		sName = CHwModuleFactory::GetInstance().GetPrototypeName(i);
		if(sName.Find("Unknown",0) == -1)
		{
			// Don't insert Unknown (X) where X stand for interface (I2C, USB...)
			// Unknown (X) will be inserted accoring to HwModule IF type
			pHwModuleReplaceItemsVec->push_back(sName);
		}
	}
	for(i = 0; i < nHwModuleProtoCount; i++)
	{
		CString sName;
		sName = CHwModuleFactory::GetInstance().GetPrototypeName(i);
		m_HwModuleMenuReplaceItemsMap[sName] = pHwModuleReplaceItemsVec;
	}

	CHwModuleCtrlMenuItem menuItem;

	menuItem.m_nMenuItemImage = IDB_SAVE_FILE;
	menuItem.m_nMenuItemID    = ID_HW_MODULE_CTRL_MENU_SAVE;
	menuItem.m_sMenuItem      = "Save settings...";
	m_HwModuleCtrlMenuItems.push_back(menuItem);

	menuItem.m_nMenuItemImage = IDB_OPEN_FILE;
	menuItem.m_nMenuItemID    = ID_HW_MODULE_CTRL_MENU_LOAD;
	menuItem.m_sMenuItem      = "Load settings...";
	m_HwModuleCtrlMenuItems.push_back(menuItem);
	
	menuItem.m_sMenuItem      = "SEPARATOR";
	m_HwModuleCtrlMenuItems.push_back(menuItem);

	menuItem.m_nMenuItemImage = IDB_REFRESH_HW_MODDULE_TREE;
	menuItem.m_nMenuItemID    = ID_HW_MODULE_CTRL_MENU_REFRESH;
	menuItem.m_sMenuItem      = "Refresh Tree";
	m_HwModuleCtrlMenuItems.push_back(menuItem);

	//menuItem.m_nMenuItemImage = -1;
	//menuItem.m_nMenuItemID    = ID_HW_MODULE_CTRL_MENU_RESET_SUB20;
	//menuItem.m_sMenuItem      = "Reset SUB-20";
	//m_HwModuleCtrlMenuItems.push_back(menuItem);

	//I2C Unit menu
	CHwModuleCtrlMenuItem          unitMenuItem;
	vector<CHwModuleCtrlMenuItem>  *pHwModuleCtrlUnitI2CMenuItems = new vector<CHwModuleCtrlMenuItem>;

	unitMenuItem.m_nMenuItemImage = -1;
	unitMenuItem.m_nMenuItemID    = ID_HW_MODULE_CTRL_UNIT_I2C_SET_FREQ;
	unitMenuItem.m_sMenuItem      = "Set I2C Frequency...";

	pHwModuleCtrlUnitI2CMenuItems->push_back(unitMenuItem);
	m_HwModuleCtrlUnitMenuItems["SUB-20 I2C"] = pHwModuleCtrlUnitI2CMenuItems;

}

CHwModuleTreeView::~CHwModuleTreeView()
{
	map<CString,vector<CString>*>::iterator iter, endIter;

	iter    = m_HwModuleMenuReplaceItemsMap.begin();
	endIter = m_HwModuleMenuReplaceItemsMap.begin();
	for(; iter != endIter; iter++)
	{
		if((iter)->second != NULL)
		{
			delete (iter)->second;
		}
	}
}

BEGIN_MESSAGE_MAP(CHwModuleTreeView, CXTTreeCtrl)
	
	ON_MESSAGE(WM_HW_MODULE_MODELESS_DLG_DIS_NAME_CHANGE, OnHwModuleModelessDlgDisplayNameChange)
	ON_MESSAGE(WM_HW_MODULE_ITEM_MODELESS_DLG_UNIT_ITEM_DIS_NAME_CHANGE, OnHwModuleModelessDlgUnitItemDisplayNameChange)
	ON_MESSAGE(WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED, OnHwModuleModelessDlgDestroyed)
	ON_MESSAGE(WM_HW_MODULE_NOTIFY_SELECT, OnHwModuleSelect)
	ON_MESSAGE(WM_HW_MODULE_NOTIFY_DESELECT, OnHwModuleDeSelect)
END_MESSAGE_MAP()


void  CHwModuleTreeView::CreateImageList()
{
	m_cImageList.Create(16,16, ILC_COLOR32 , 1, 0);
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_HW_MODULE_CTRL_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_HW_MODULE_CTRL_UNIT_BMP2); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_HW_MODULE_BMP2); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_HW_MODULE_MUX_CHAN_BMP1); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_HW_MODULE_ADC_VOL_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_HW_MODULE_ADC_CUR_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	SetImageList(&m_cImageList, TVSIL_NORMAL);
}

vector<CHwModuleCtrlMenuItem>*  CHwModuleTreeView::GetHwModuleCtrlMenuItems(CHwModuleCtrl *pObj)
{
	//All ctrl have same menu
	return &m_HwModuleCtrlMenuItems;
}

BOOL CHwModuleTreeView::OnHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem* pItem) 
{
	CString   sName;
	HTREEITEM tItem = GetTreeItem(pItem);

	if(tItem == NULL)
	{
		AfxMessageBox("Error while setting tree item (HwModuleCtrlUnitItem) name!",NULL,MB_ICONERROR);
		return FALSE;
	
	}

	sName = pItem->GetDisplayName();
	SetItemText(tItem,sName);

	return TRUE;
}
LRESULT CHwModuleTreeView::OnHwModuleModelessDlgDisplayNameChange(WPARAM a, LPARAM b) 
{
	CHwModule *pHwModule = (CHwModule*)(a);
	CString    sName     = *(CString*)(b);

	if(NotifySetHwModuleDisplayName(pHwModule,sName) == TRUE)
		return 1;
	return 0;
}

LRESULT CHwModuleTreeView::OnHwModuleModelessDlgUnitItemDisplayNameChange(WPARAM a, LPARAM b) 
{
	CHwModuleCtrlUnitItem *pItem = (CHwModuleCtrlUnitItem*)(a);
	CString                sName = *(CString*)(b);

	if(NotifySetHwModuleCtrlUnitItemDisplayName(pItem,sName) == TRUE)
		return 1;
	return 0;
}

LRESULT CHwModuleTreeView::OnHwModuleModelessDlgDestroyed(WPARAM a, LPARAM b) 
{
	CHwModuleItem *pHwModuleItem = (CHwModuleItem*)(a);
	if(pHwModuleItem != NULL)
	{
		map<CHwModuleItem*,CBasicItemDlg*>::iterator iter, endIter;

		iter    = m_HwModuleDlgMap.begin();
		endIter = m_HwModuleDlgMap.end();
		for(; iter != endIter; iter++)
		{
			if((iter)->first == pHwModuleItem)
			{
				m_HwModuleDlgMap.erase(iter);
				break;
			}
		}
	}
	return 0;
}

LRESULT CHwModuleTreeView::OnHwModuleSelect(WPARAM a, LPARAM b) 
{
	CHwModule *pModule = dynamic_cast<CHwModule*>((CHwModuleItem*)a);

	if(pModule != NULL)
	{
		if(SelectHwModule(pModule) == FALSE)
		{
			AfxMessageBox("Failed to select hw module!",NULL,MB_ICONERROR);
		}
	}
	RedrawWindow();
	return 0;
}
LRESULT CHwModuleTreeView::OnHwModuleDeSelect(WPARAM a, LPARAM b) 
{
	CHwModule *pModule = dynamic_cast<CHwModule*>((CHwModuleItem*)a);
	if(pModule != NULL)
	{
		if(DeSelectHwModule(pModule) == FALSE)
		{
			AfxMessageBox("Failed to de-select hw module!",NULL,MB_ICONERROR);
		}

	}
	RedrawWindow();
	return 0;
}

CBasicItemDlg* CHwModuleTreeView::GetHwModuleItemDlg(CHwModuleItem *pObj)
{
	map<CHwModuleItem*,CBasicItemDlg*>::iterator iter, endIter;

	iter    = m_HwModuleDlgMap.begin();
	endIter = m_HwModuleDlgMap.end();
	for(; iter != endIter; iter++)
	{
		if((iter)->first == pObj)
		{
			return (iter)->second;
		}
	}
	return NULL;
}

void	 CHwModuleTreeView::ShowHwModuleItemDlg(CHwModuleItem *pObj)
{
	CWnd		      *pMainWnd;
	CBasicItemDlg     *pBasicDlg = NULL;
	IHwModuleItemDlg  *pHwModuleItemDlg;
	
	pBasicDlg = GetHwModuleItemDlg(pObj);
	if(pBasicDlg == NULL)
	{		
		pMainWnd = AfxGetApp()->m_pMainWnd;
    	pBasicDlg = CHwModuleItemDlgFactory::GetInstance().Create(pObj);

		if(pBasicDlg == NULL)
			return;

		pHwModuleItemDlg = dynamic_cast<IHwModuleItemDlg*>(pBasicDlg);
		if(pHwModuleItemDlg == NULL)
		{
			delete pBasicDlg;
			return;
		}
		if(pHwModuleItemDlg->Create(pMainWnd) == FALSE) 
		{
			delete pBasicDlg;
			return;
		} 
		m_HwModuleDlgMap[pObj] = pBasicDlg;
		pBasicDlg->SetOwner(this);		

		pBasicDlg->ShowWindow(SW_SHOW);
		pBasicDlg->SetForegroundWindow();

	}	
    else /* already open */
    {
		pBasicDlg->ShowWindow(SW_SHOW);
		if(pBasicDlg->IsIconic())   // in case the minimize button is enabled
		{
			pBasicDlg->ShowWindow(SW_RESTORE);
		}
		pBasicDlg->SetForegroundWindow();
	}
}



BOOL CHwModuleTreeView::InitHWMTreeCtrl()
{
	return TRUE;
}

BOOL CHwModuleTreeView::IsStringMenuExists(CString sStrMenu, vector<CString>* pVec)
{
	vector<CString>::iterator iter, endIter;

	iter    = pVec->begin();
	endIter = pVec->end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == sStrMenu)
			return TRUE;
	}
	return FALSE;
}
vector<CHwModuleCtrlMenuItem>* CHwModuleTreeView::GetHwModuleCtrlUnitMenuItems(CHwModuleCtrlUnit *pObj)
{
	map<CString,vector<CHwModuleCtrlMenuItem>*>::iterator iter, endIter;

	iter    = m_HwModuleCtrlUnitMenuItems.begin();
	endIter = m_HwModuleCtrlUnitMenuItems.end();
	for(; iter != endIter; iter++)
	{
		if((iter)->first == pObj->GetName())
		{
			return (iter)->second;
		}
	}
	return NULL;
}
vector<CString>* CHwModuleTreeView::GetHwModuleMenuReplaceItems(CHwModule *pObj)
{
	map<CString,vector<CString>*>::iterator iter, endIter;

	iter    = m_HwModuleMenuReplaceItemsMap.begin();
	endIter = m_HwModuleMenuReplaceItemsMap.end();
	for(; iter != endIter; iter++)
	{
		if((iter)->first == pObj->GetName())
		{
			CString sStrMenu;
			//Add unknown (X) type
			sStrMenu = "Unknown (" + pObj->GetInterfaceTypeName() + ")";
			if(IsStringMenuExists(sStrMenu,(iter)->second) == FALSE)
			{
				((iter)->second)->push_back(sStrMenu);
			}
			return (iter)->second;
		}
	}
	return NULL;
}

CHwModuleItem* CHwModuleTreeView::GetHwModuleItem(HTREEITEM tItem)
{
	return (CHwModuleItem*)GetItemData(tItem);
}


void CHwModuleTreeView::OnAddHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj)
{
	HTREEITEM tItem, tParentItem = TVI_ROOT;
	CString   sName;
	int		  nImage = 4;

	if(pParentObj != NULL)
	{
		tParentItem = GetTreeItem(pParentObj);
		if(tParentItem == NULL)
		{
			AfxMessageBox("Error while adding tree item (HwModuleCtrlUnitItem)!",NULL,MB_ICONERROR);
			return;
		}
	}

	sName = pObj->GetDisplayName();
	if(sName.Find("Voltage#") != -1)
	{
		nImage = 4;
	}
	if(sName.Find("Current#") != -1)
	{
		nImage = 5;
	}
	tItem = InsertItem(sName,nImage,nImage,tParentItem);
	SetItemData(tItem,(DWORD_PTR)pObj);
	
	ExpandTree();
	RedrawWindow();
}
void CHwModuleTreeView::OnAddHwModuleItem(CHwModule *pObj, CHwModuleItem *pParentObj)
{
	HTREEITEM tItem, tParentItem = TVI_ROOT;
	CString   sName;
	int		  nImage = 2;

	if(pParentObj != NULL)
	{
		tParentItem = GetTreeItem(pParentObj);
		if(tParentItem == NULL)
		{
			AfxMessageBox("Error while adding tree item (CHwModule)!",NULL,MB_ICONERROR);
			return;
		}
	}

	sName = pObj->GetDisplayName();
	if(pObj->GetName() == "Mux Channel")
	{
		nImage = 3;
	}

	tItem = InsertItem(sName,nImage,nImage,tParentItem);
	SetItemData(tItem,(DWORD_PTR)pObj);
	
	ExpandTree();
	RedrawWindow();
}

BOOL CHwModuleTreeView::OnHwModuleDisplayNameChange(CHwModule *pObj)
{
	HTREEITEM tItem;

	tItem = GetTreeItem(pObj);
	if(tItem == NULL)
		return FALSE;
	SetItemText(tItem,pObj->GetDisplayName());
	return TRUE;
}

void CHwModuleTreeView::OnAddHwModuleItem(CHwModuleCtrl *pObj, CHwModuleItem *pParentObj)
{
	HTREEITEM tiItem, tiParentItem = TVI_ROOT;
	CString   sName;

	if(pParentObj != NULL)
	{
		tiParentItem = GetTreeItem(pParentObj);
		if(tiParentItem == NULL)
		{
			AfxMessageBox("Error while adding tree item (HwModuleCtrl)!",NULL,MB_ICONERROR);
			return;
		}
	}

	sName = pObj->GetName(); 
	
	if(pObj->GetUniqueID().IsEmpty() == FALSE)
		sName +=  " (" + pObj->GetUniqueID() + ")"; 

	tiItem = InsertItem(sName,0,0,tiParentItem);
	SetItemBold(tiItem);
	SetItemData(tiItem,(DWORD_PTR)pObj);
	RedrawWindow();
}

void CHwModuleTreeView::OnAddHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj)
{
	HTREEITEM tiItem, tiParentItem = TVI_ROOT;
	CString   sName;

	if(pParentObj != NULL)
	{
		tiParentItem = GetTreeItem(pParentObj);
		if(tiParentItem == NULL)
		{
			AfxMessageBox("Error while adding tree item (HwModule)!",NULL,MB_ICONERROR);
			return;
		}
	}

	sName = pObj->GetDisplayName(); 
	tiItem = InsertItem(sName,1,1,tiParentItem);
	SetItemBold(tiItem);
	SetItemData(tiItem,(DWORD_PTR)pObj);
	RedrawWindow();
}

BOOL CHwModuleTreeView::OnSelectHwModule(CHwModule *pObj)
{
	HTREEITEM tItem;
	tItem = GetTreeItem(pObj);
	if(tItem == NULL)
		return FALSE;
	SetItemBold(tItem);
	RedrawWindow();
	return TRUE;
}

BOOL CHwModuleTreeView::OnDeSelectHwModule(CHwModule *pObj)
{
	HTREEITEM tItem;

	tItem = GetTreeItem(pObj);
	if(tItem == NULL)
		return FALSE;
	SetItemBold(tItem,FALSE);	
	RedrawWindow();
	return TRUE;
}

void	CHwModuleTreeView::ExpandTree()
{
	HTREEITEM		tRoot;

	tRoot = GetRootItem();
	ExpandTree(tRoot);
	while((tRoot = GetNextSiblingItem(tRoot) )!= NULL)
	{
		ExpandTree(tRoot);
	}
}

void CHwModuleTreeView::ExpandTree(HTREEITEM tiStartItem)
{
	// start at root is required
	if ( tiStartItem == NULL ) 
	{
		ExpandTree(GetRootItem());
		return;
	}
	
	if(GetItemState(tiStartItem, TVIS_EXPANDED) != TVIS_EXPANDED)
	{
		Expand(tiStartItem,TVE_EXPAND);
	}
	
	// now recurse on all children of this node
	HTREEITEM hChild = GetNextItem(tiStartItem,TVGN_CHILD);
	for ( ; hChild != NULL ; ) 
	{
		ExpandTree(hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

void CHwModuleTreeView::FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem)
{
	// start at root is required
	if ( tiStartItem == NULL ) 
	{
		FindTreeItem(itemData,tiFoundAt,GetRootItem());
		return;
	}
	
	// check this node for match
	if ( itemData == GetItemData(tiStartItem) )
	{
		tiFoundAt = tiStartItem;
	}
	// now recurse on all children of this node
	HTREEITEM hChild = GetNextItem(tiStartItem,TVGN_CHILD);
	for ( ; hChild != NULL && tiFoundAt == NULL ; ) 
	{
		FindTreeItem(itemData,tiFoundAt,hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

HTREEITEM CHwModuleTreeView::GetTreeItem(CHwModuleItem* pHwModuleItem)
{
	HTREEITEM		tRoot,tTheItem = NULL;

	tRoot = GetRootItem();
	FindTreeItem((DWORD_PTR)pHwModuleItem,tTheItem,tRoot);
	
	if(tTheItem != NULL)
		return tTheItem;

	while((tRoot = GetNextSiblingItem(tRoot) )!= NULL)
	{
		FindTreeItem((DWORD_PTR)pHwModuleItem,tTheItem,tRoot);
		if(tTheItem != NULL)
			return tTheItem;
	}

	return NULL;

}

BOOL CHwModuleTreeView::RemoveHwModuleTreeItem(CHwModuleItem *pObj)
{
	HTREEITEM tItem = NULL;

	tItem = GetTreeItem(pObj);
	if(tItem != NULL)
	{
		DeleteItem(tItem);
		return TRUE;
	}
	return FALSE;
}


BOOL CHwModuleTreeView::DeleteRecursiveTreeItems(CHwModuleCtrl *pObj)
{
	HTREEITEM	hChild, tItem;

	tItem = GetTreeItem(pObj);


	if(tItem == NULL)
		return FALSE;

	//Loop all child items
	while((hChild = GetChildItem(tItem)) != NULL)
	{	
		DeleteRecursiveTreeItems(pObj);
	}
	return RemoveHwModuleTreeItem(pObj);
}

BOOL CHwModuleTreeView::OnRemoveHwModuleCtrl(CHwModuleCtrl *pObj)
{
	//////////////////////////////////////////////////////////////
	// OnRemoveHwModuleCtrl tiggers removal of all CHwModuleCtrl 
	// sub units tree items only!!! without notifying the model 
	// like in the hw module case (because delete of CHwModuleCtrl 
	// is done by the model and the release of each unit is also 
	// done there --> no need to notify it)
	//////////////////////////////////////////////////////////////
	return RemoveHwModuleTreeItem(pObj);

}

BOOL CHwModuleTreeView::SetHwModuleDisplayName(CHwModule *pObj, CString sDisplayName)
{
	return NotifySetHwModuleDisplayName(pObj,sDisplayName);
}

BOOL CHwModuleTreeView::RemoveHwModule(CHwModule *pObj)
{
	HTREEITEM		  hChild, tItem = NULL;
	CHwModuleItem*    pHwModuleItem;


	tItem = GetTreeItem(pObj);
	if(tItem == NULL)
		return FALSE;

	while((hChild = GetChildItem(tItem)) != NULL)
	{	
		pHwModuleItem = GetHwModuleItem(hChild);
		if(pHwModuleItem == NULL)
			return FALSE;
		RemoveHwModule((CHwModule*)pHwModuleItem);
	}
	//Request model to remove the item (the presentor callback will remove it from tree automatically)
	if(NotifyRemoveHwModule(pObj) == FALSE)
		return FALSE;

	return TRUE;
}

BOOL CHwModuleTreeView::OnRemoveHwModule(CHwModule *pObj)
{
	return RemoveHwModuleTreeItem((CHwModuleItem*)pObj);
}
BOOL CHwModuleTreeView::SelectHwModule(CHwModule *pObj)
{
	return NotifySelectHwModule(pObj);
}
BOOL CHwModuleTreeView::DeSelectHwModule(CHwModule *pObj)
{
	return NotifyDeSelectHwModule(pObj);
}

BOOL CHwModuleTreeView::ResetHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	return NotifyResetHwModuleCtrl(pCtrl);
}

BOOL CHwModuleTreeView::RefreshHwModuleCtrl(CHwModuleCtrl *pCtrl)
{
	return NotifyRefreshHwModuleCtrl(pCtrl);
}

BOOL CHwModuleTreeView::SaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	return NotifySaveHwModuleModelSettings(sFile,pCtrl);
}

BOOL CHwModuleTreeView::LoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl)
{
	return NotifyLoadHwModuleModelSettings(sFile,pCtrl);
}


BOOL CHwModuleTreeView::OnReplaceHwModule(CHwModule* pObj,CHwModule *pNewObj)
{
	HTREEITEM tiOldHwModule;
	CString   sName;

	tiOldHwModule = GetTreeItem(pObj);

	if(tiOldHwModule == NULL)
		return FALSE;

	sName = pNewObj->GetDisplayName();
	SetItemText(tiOldHwModule,sName);
	SetItemData(tiOldHwModule,(DWORD_PTR)pNewObj);
	return TRUE;

}

BOOL CHwModuleTreeView::ReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName)
{
	return NotifyReplaceHwModule(pHwModule,sNewHwModuleName);
}

BOOL CHwModuleTreeView::AddUsbDeviceMsg(CString sDevId)
{
	return NotifyAddUsbDeviceMsg(sDevId);
}

BOOL CHwModuleTreeView::RemoveUsbDeviceMsg(CString sDevId)
{
	return NotifyRemoveUsbDeviceMsg(sDevId);
}

