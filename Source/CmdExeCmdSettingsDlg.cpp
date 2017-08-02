// CommandSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdSettingsDlg.h"
#include "MVTerminal.h"
#include "HwModuleModel.h"
#include "TreeDroptarget.h"
#include "CmdExeItemFactory.h"
#include "CmdExePane.h"
#include "CmdExeCmdItem.h"
#include "CmdExeItemDlgFactory.h"
#include "CmdExeItemContainerFactory.h"
#include "HwModuleCtrlUnit.h"
#include "IHwModuleContainer.h"
#include "HwModuleItemCmdExeItemContainer.h"
#include "CmdExeItemContainer.h"
#include "HwModuleItemCmdExeCmdItem.h"
#include "HwModuleDlgFactory.h"

#define  _WM_USER               2600
#define  ID_CE_COPY_ITEM	   (_WM_USER+18)
#define  ID_CE_PASTE_ITEM	   (_WM_USER+19)
#define  ID_CE_DUPLICATE_ITEM  (_WM_USER+20)
#define  ID_CE_REMOVE_ITEM     (_WM_USER+21)

// CCmdExeCmdSettingsDlg dialog

IMPLEMENT_DYNAMIC(CCmdExeCmdSettingsDlg, CDialog)
CCmdExeCmdSettingsDlg::CCmdExeCmdSettingsDlg(CCmdExeCmd* pCmd,CWnd* pParentWnd)
	: CDialog(CCmdExeCmdSettingsDlg::IDD, NULL)
{
	m_pGeneralCmdExeCmdItemContainer = NULL;
	m_CmdItemClipboard.m_bIsEmpty    = TRUE;
	m_pCmd      					 = pCmd;
	m_pParentWnd					 = pParentWnd;
}

CCmdExeCmdSettingsDlg::~CCmdExeCmdSettingsDlg()
{
	if(m_pGeneralCmdExeCmdItemContainer != NULL)
	{
		delete m_pGeneralCmdExeCmdItemContainer;
		m_pGeneralCmdExeCmdItemContainer = NULL;
	}
}

void CCmdExeCmdSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDC_CMDE_CANCEL_BTN, m_cancelBtn);
	DDX_Control(pDX, IDC_CMDE_CMD_LOOPCOUNT_SPIN, m_loopCountSpin);
	DDX_Control(pDX, IDC_CMDE_CMD_LOOPCOUNT_EDIT, m_loopCountEdit);
	DDX_Control(pDX, IDC_SRC_TREE, m_templateCmdItemTreeView);
	DDX_Control(pDX, IDC_CMD_EXE_CMD_SETTING_TREE, m_cmdTreeView);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_CMDE_CANCEL_BTN, OnBnClickedCmdeCancelBtn)
	ON_BN_CLICKED(IDC_CMDE_CMD_STOP_ON_ERR_CHECK, OnBnClickedCmdeCmdStopOnErrCheck)
	ON_MESSAGE(CE_ON_DEST_TREE_ITEM_ADDED,OnDestTreeItemAdded)
	ON_BN_CLICKED(IDC_CE_CMD_EN_CHECK, OnBnClickedCeCmdEnCheck)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_EXE_CMD_SETTING_TREE, OnNMDblclkCmdExeCmdSettingTree)
	ON_NOTIFY(NM_RCLICK, IDC_CMD_EXE_CMD_SETTING_TREE, OnNMRclickCmdExeCmdSettingTree)
END_MESSAGE_MAP()

void CCmdExeCmdSettingsDlg::OnBnClickedCmdeCmdStopOnErrCheck()
{
	m_pCmd->SetStopExecutionOnError(!m_pCmd->GetStopExecutionOnError());
}


void  CCmdExeCmdSettingsDlg::AlignWindow()
{

    CRect rectFrame, rectDlg;
    CWnd* pMainWnd = AfxGetMainWnd();
    if(pMainWnd != NULL)
    {
        pMainWnd->GetClientRect(rectFrame);
        pMainWnd->ClientToScreen(rectFrame);
        GetWindowRect(rectDlg);

        int nXPos = rectFrame.left + (rectFrame.Width() / 2) 
                                   - (rectDlg.Width() / 2);
        int nYPos = rectFrame.top + (rectFrame.Height() / 3) 
                                  - (rectDlg.Height() / 3);
        
	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

BOOL CCmdExeCmdSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	AlignWindow();

	CreateImageList();

	m_loopCountSpin.SetBuddy(&m_loopCountEdit);
	m_loopCountSpin.SetRange(0,10000);

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_browseBtn.SetTheme(xtpButtonThemeOffice2000);

	//Init command parameters
	CheckDlgButton(IDC_CMDE_CMD_STOP_ON_ERR_CHECK, m_pCmd->GetStopExecutionOnError());
	CheckDlgButton(IDC_CE_CMD_EN_CHECK, m_pCmd->GetEnable());
	SetDlgItemText(IDC_CMDE_CMD_NAME_EDIT, m_pCmd->GetDisplayName());
	CString sLoopCount;
	sLoopCount.Format("%d", m_pCmd->GetLoopCount());
	SetDlgItemText(IDC_CMDE_CMD_LOOPCOUNT_EDIT,sLoopCount);

	m_cmdTreeView.RegisterDropTarget();
	m_cmdTreeView.SetIsDroppable(TRUE);
	m_cmdTreeView.SetCanMoveItems(TRUE);

	m_templateCmdItemTreeView.RegisterDropTarget();
	m_templateCmdItemTreeView.SetIsDroppable(FALSE);
	m_templateCmdItemTreeView.SetCanMoveItems(FALSE);

	InitCmdItemContainerList();
	InitTemplateCmdItemTreeView();
	InitCmdTreeView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdExeCmdSettingsDlg::CreateImageList()
{
	//CImageList
	
	m_cImageList.Create(16,16, ILC_COLOR32 , 1, 0);
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_TITLE_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_CE_CMD_BMP);
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_CE_CMD_STATE_BMP);
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	m_cmdTreeView.SetImageList(&m_cImageList, TVSIL_NORMAL);
	m_templateCmdItemTreeView.SetImageList(&m_cImageList, TVSIL_NORMAL);	

}


void CCmdExeCmdSettingsDlg::InitCmdTreeView()
{
	HTREEITEM  tiCmd, tiCmdItem;
	CString    sName;

	//Add the command
	tiCmd = m_cmdTreeView.InsertItem(m_pCmd->GetDisplayName(),1,1,TVI_ROOT,TVI_LAST);
	m_cmdTreeView.SetItemData(tiCmd,(DWORD_PTR)m_pCmd);

	//Add command items
	int i, nCount = m_pCmd->GetCmdItemCount();
	for(i = 0; i < nCount; i++)
	{
		CCmdExeItem *pCmdItem;
		pCmdItem = m_pCmd->GetCmdItem(i);
		tiCmdItem = m_cmdTreeView.AddItem(tiCmd,pCmdItem->GetDisplayName() , TVI_LAST, 2,2,0);
		m_cmdTreeView.SetItemData(tiCmdItem,(DWORD_PTR)pCmdItem);
	}

	m_cmdTreeView.SetItemBold(tiCmd);
	m_cmdTreeView.Expand(tiCmd,TVE_EXPAND);
	m_cmdTreeView.SetRootItem(tiCmd);
	m_cmdTreeView.SetRootName(m_pCmd->GetDisplayName());
	m_cmdTreeView.ResetNonDragableItems();
	m_cmdTreeView.AddNonDragableItem(m_pCmd->GetDisplayName());
}

void CCmdExeCmdSettingsDlg::InitCmdItemContainerList()
{
	// Create General cmd items
	m_pGeneralCmdExeCmdItemContainer = CCmdExeItemContainerFactory::GetInstance().Create("General");

}

HTREEITEM	CCmdExeCmdSettingsDlg::AddCmdExeCmdItemContainer(CCmdExeItemContainer* pContainer, HTREEITEM tiParent)
{
	HTREEITEM				     tiItem,tiContainer = NULL;
	vector<CString>				 cmdItemNameList;
	vector<CString>::iterator    iter,endIter;
	CString					     sName;

    if(pContainer != NULL)
	{
		m_templateCmdItemTreeView.AddNonDragableItem(pContainer->GetDisplayName());

		tiContainer = m_templateCmdItemTreeView.AddItem(tiParent, pContainer->GetDisplayName(), NULL, 0,0,0); 
		m_templateCmdItemTreeView.SetItemBold(tiContainer);

		pContainer->GetCmdItemNameList(cmdItemNameList);
		iter    = cmdItemNameList.begin();
		endIter = cmdItemNameList.end();
		for(; iter != endIter; iter++)
		{
			sName = (*iter);
			CCmdExeItem *pItem = pContainer->CreateCmdItem(sName);
			if(pItem != NULL)
			{
				tiItem = m_templateCmdItemTreeView.AddItem(tiContainer,sName , TVI_LAST, 2,2,0);
				m_templateCmdItemTreeView.SetItemData(tiItem,(DWORD_PTR)pItem);
			}
		}
	}
	m_templateCmdItemTreeView.Expand(tiContainer,TVE_EXPAND);
	return tiContainer;
}

HTREEITEM CCmdExeCmdSettingsDlg::CreateHwModuleItemContainer(CHwModuleItem* pItem, HTREEITEM hParent)
{
	CHwModuleItemCmdExeItemContainer *pHwModuleCmdItemContainer;
	HTREEITEM						  tHwModuleItemCont = NULL;

	pHwModuleCmdItemContainer = (CHwModuleItemCmdExeItemContainer*)CCmdExeItemContainerFactory::GetInstance().Create(pItem->GetName());
	if(pHwModuleCmdItemContainer != NULL)
	{
		pHwModuleCmdItemContainer->SetHwModuleItem(pItem);
		pHwModuleCmdItemContainer->SetDisplayName(pItem->GetDisplayName());
		tHwModuleItemCont = AddCmdExeCmdItemContainer(pHwModuleCmdItemContainer,hParent);
	}

	return tHwModuleItemCont;
}

HTREEITEM CCmdExeCmdSettingsDlg::AddHwModuleContainerCmdItemContainer(CHwModuleItem* pItem, HTREEITEM hParent)
{
	IHwModuleContainer* pHwModuleContainer,*pSubHwModuleContainer;
	HTREEITEM           hHwModuleCont = NULL;
	CHwModule		   *pHwModule;

	pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pItem);
	if(pHwModuleContainer != NULL)
	{
		hHwModuleCont = CreateHwModuleItemContainer(pItem,hParent);

		if(hHwModuleCont == NULL)
			return NULL;

		int nHwModuleCount = 0, nHwModule;
		nHwModuleCount = pHwModuleContainer->GetHwModuleCount();

		for(nHwModule = 0; nHwModule < nHwModuleCount; nHwModule++)
		{
			pHwModule = pHwModuleContainer->GetHwModule(nHwModule);

			pSubHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pHwModule);
			if(pSubHwModuleContainer != NULL)
			{
				AddHwModuleContainerCmdItemContainer(pHwModule,hHwModuleCont);
			}
			else
			{
				CreateHwModuleItemContainer(pHwModule,hHwModuleCont);
			}
		}
	}

	return hHwModuleCont;
}


void CCmdExeCmdSettingsDlg::InitTemplateCmdItemTreeView()
{
	m_templateCmdItemTreeView.ResetNonDragableItems();

	// Add general container name
	AddCmdExeCmdItemContainer(m_pGeneralCmdExeCmdItemContainer,TVI_ROOT);

	//Add HwModules containers 
	HTREEITEM						  hParent, hHwModuleCtrl;
	vector<CHwModuleCtrl*>            hwModuleCtrlList;
	vector<CHwModuleCtrl*>::iterator  iter, endIter;

	CHwModuleModel::GetInstance().GetHwModuleCtrlList(hwModuleCtrlList);

	iter    = hwModuleCtrlList.begin();
	endIter = hwModuleCtrlList.end();

	hParent = m_templateCmdItemTreeView.AddItem(TVI_ROOT, "Hardware Modules", NULL, 0,0,0); 
	m_templateCmdItemTreeView.AddNonDragableItem("Hardware Modules");
	m_templateCmdItemTreeView.SetItemBold(hParent);

	for(;iter != endIter; iter++)
	{
		// Hw Module Ctrl
		hHwModuleCtrl = CreateHwModuleItemContainer((*iter),hParent);
		if(hHwModuleCtrl == NULL)
			continue;

		int				    nUnit, nUnitCount = (*iter)->GetUnitCount();
		CHwModuleCtrlUnit*  pUnit;
		IHwModuleContainer* pHwModuleContainer;

		for(nUnit = 0; nUnit < nUnitCount ; nUnit++)
		{
			pUnit = (*iter)->GetUnit(nUnit);

			pHwModuleContainer = dynamic_cast<IHwModuleContainer*>(pUnit);
			if(pHwModuleContainer != NULL)
			{
				//Recursivelly add all hw modules
				AddHwModuleContainerCmdItemContainer(pUnit,hHwModuleCtrl);
			}
			else
			{
				// Hw Module Ctrl unit
				CreateHwModuleItemContainer(pUnit,hHwModuleCtrl);
			}
		}
	}
	m_templateCmdItemTreeView.Expand(hParent,TVE_EXPAND);
}

void CCmdExeCmdSettingsDlg::OnBnClickedCmdeCancelBtn()
{
	OnCancel();
}

HTREEITEM CCmdExeCmdSettingsDlg::GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult)
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_cmdTreeView.ScreenToClient(&point);
	 item = m_cmdTreeView.HitTest(point, &uFlags);
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
	 *pResult = 0;
	 return NULL;
}

void CCmdExeCmdSettingsDlg::HandleMenuSelection(UINT nMenuIndex)
{
	HTREEITEM item = NULL;

	if((item = m_cmdTreeView.GetSelectedItem()) == NULL)
	{
		return;
	}
	switch(nMenuIndex)
	{
	case ID_CE_COPY_ITEM:
		CopyCmdItem(item);
    	break;
	case ID_CE_PASTE_ITEM:
		PasteCmdItem(item);
		break;
	case ID_CE_DUPLICATE_ITEM:
		CopyCmdItem(item);
		PasteCmdItem(item);
		break;
	case ID_CE_REMOVE_ITEM:
		DeleteTreeItem(item);
		break;
	default:
		break;
	}
   	m_cmdTreeView.RedrawWindow();
}

void CCmdExeCmdSettingsDlg::DeleteTreeItem(HTREEITEM item)
{	
	CCmdExeItem      *pCmdItem; 

	if((pCmdItem = (CCmdExeItem*)m_cmdTreeView.GetItemData(item)) == NULL)
	{
		MessageBox("Error while deleting item!",NULL,MB_ICONERROR);
		return;
	}
	//No check for return value since item may not exist in cmd list (new command)
	m_pCmd->DeleteItem(pCmdItem);
	m_cmdTreeView.DeleteItem(item);
}


void CCmdExeCmdSettingsDlg::PasteCmdItem(HTREEITEM hInsAfterItem)
{
	HTREEITEM         hItem;
	TV_ITEM		      tvItem;
	TV_INSERTSTRUCT	  tvIns;	
	CCmdExeItem      *pCmdItem; 

	tvItem = m_CmdItemClipboard.m_tvItem;
	
	tvIns.item		   = tvItem;
	tvIns.hInsertAfter = hInsAfterItem;
	tvIns.hParent	   = m_cmdTreeView.GetRootItem();
	hItem = m_cmdTreeView.InsertItem(&tvIns);

	pCmdItem = m_CmdItemClipboard.m_pCmdItem->Clone();
	m_cmdTreeView.SetItemData(hItem,(DWORD_PTR)pCmdItem);
}

void CCmdExeCmdSettingsDlg::CopyCmdItem(HTREEITEM hItem)
{
	CCmdExeItem  *pCmdItem; 

	m_CmdItemClipboard.m_bIsEmpty          = FALSE;
	m_CmdItemClipboard.m_tvItem.hItem      = hItem;
	m_CmdItemClipboard.m_tvItem.mask       = TVIF_TEXT | TVIF_HANDLE | TVIF_IMAGE |TVIF_PARAM |TVIF_STATE |TVIF_SELECTEDIMAGE |TVIF_CHILDREN;
	m_CmdItemClipboard.m_tvItem.pszText    = m_CmdItemClipboard.m_szText;
    m_CmdItemClipboard.m_tvItem.cchTextMax = 1024;
	if(m_cmdTreeView.GetItem(&m_CmdItemClipboard.m_tvItem) == FALSE)
	{
		MessageBox("Failed to copy item!",NULL,MB_ICONERROR);
		return;
	}

	if((pCmdItem = (CCmdExeItem*)m_cmdTreeView.GetItemData(hItem)) == NULL)
	{
		MessageBox("Error while copying item!",NULL,MB_ICONERROR);
		return;
	}
	m_CmdItemClipboard.m_pCmdItem = pCmdItem;
}

BOOL CCmdExeCmdSettingsDlg::ShowCmdItemDlg(CCmdExeCmdItem *pCmdItem)
{
	CBasicItemDlg			   *pDlg;

	pDlg = CCmdExeItemDlgFactory::GetInstance().Create(pCmdItem);
	if(pDlg != NULL)
	{
		if(pDlg->DoModal() != IDOK)
		{
			return FALSE;
		}
	}
	//If pDlg is null, item do not need a dialog (like board reset)
	return TRUE;
}

LRESULT CCmdExeCmdSettingsDlg::OnDestTreeItemAdded(WPARAM a, LPARAM b) 
{
	HTREEITEM	     tiAddedItem = (HTREEITEM)a;
	CCmdExeCmdItem  *pCmdItem; 

	pCmdItem = (CCmdExeCmdItem*)m_cmdTreeView.GetItemData(tiAddedItem);
	if(pCmdItem != NULL)
	{
		CCmdExeCmdItem *pNewCmdItem;
		// Clone CCmdExeCmdItem since the dragged one is a prototype
		pNewCmdItem = (CCmdExeCmdItem *)pCmdItem->Clone();
		if(ShowCmdItemDlg(pNewCmdItem) == TRUE)
		{
			// Override prototype pointer copy with new one 
			m_cmdTreeView.SetItemData(tiAddedItem,(DWORD_PTR)pNewCmdItem);
			m_cmdTreeView.SetItemText(tiAddedItem,pNewCmdItem->GetDisplayName());
			//Add the item to m_pCmd at OnOk since user might move item position
		}
		else
		{
			delete pNewCmdItem;
			m_cmdTreeView.DeleteItem(tiAddedItem);
		}
	}

	return 0;
}

BOOL CCmdExeCmdSettingsDlg::PreTranslateMessage(MSG* pMsg)
{
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CCmdExeCmdSettingsDlg::OnBnClickedCeCmdEnCheck()
{
	
}

void CCmdExeCmdSettingsDlg::OnBnClickedOk()
{
	CString			  sLoopCount;
    HTREEITEM         hChild,tiRoot;
	int				  nLoopCount = 1;

	//Get loop count
	GetDlgItemText(IDC_CMDE_CMD_LOOPCOUNT_EDIT,sLoopCount);
	if(sLoopCount.IsEmpty() == FALSE)
	{
		sscanf(sLoopCount,"%d",&nLoopCount);
	}

	//Get name
	CString  sName;
	GetDlgItemText(IDC_CMDE_CMD_NAME_EDIT,sName);
	if(sName.IsEmpty() == TRUE)
	{
		MessageBox("Invalid command name!",NULL,MB_ICONERROR);
		return;
	}

	//Set command 
	m_pCmd->SetLoopCount(nLoopCount);
	m_pCmd->SetDisplayName(sName);
	
	//Removes item from vector without delete of each item
	m_pCmd->RemoveItems(); 

	//Collect new items from tree
	tiRoot = m_cmdTreeView.GetRootItem();
	hChild = m_cmdTreeView.GetChildItem(tiRoot);
	if(hChild != NULL)
	{
		CCmdExeCmdItem *pItem;
		pItem = (CCmdExeCmdItem*)m_cmdTreeView.GetItemData(hChild);
		if(pItem == NULL)
		{
			MessageBox("Error while adding items!",NULL,MB_ICONERROR);
			return;
		}
		m_pCmd->AddCmdItem(pItem);
		while((hChild = m_cmdTreeView.GetNextSiblingItem(hChild)) != NULL)
		{
			pItem = (CCmdExeCmdItem*)m_cmdTreeView.GetItemData(hChild);
			if(pItem == NULL)
			{
				MessageBox("Error while adding items!",NULL,MB_ICONERROR);
				return;
			}
			m_pCmd->AddCmdItem(pItem);
		}
	}
	OnOK();
}


void CCmdExeCmdSettingsDlg::OnNMDblclkCmdExeCmdSettingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CCmdExeCmdItem   *pCmdItem; 
	HTREEITEM		  tiSel = GetMouseClickedItem(pNMHDR,pResult);

	if(tiSel == NULL)
		return;

 	m_cmdTreeView.SelectItem(tiSel);
	pCmdItem = dynamic_cast<CCmdExeCmdItem*>((CCmdExeItem*)m_cmdTreeView.GetItemData(tiSel));

	if(pCmdItem == NULL)
		return;

	if(ShowCmdItemDlg(pCmdItem) == TRUE)
	{
		// Override prototype pointer copy with new one 
		m_cmdTreeView.SetItemText(tiSel,pCmdItem->GetDisplayName());
	}
	
	*pResult = 0;
}

void CCmdExeCmdSettingsDlg::OnNMRclickCmdExeCmdSettingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;
	 CPoint				  point;
	 CNewMenu			  itemMenu;
	 int				  mMenuItem =0;
 	 CBitmap			  bmpImage;
	 CCmdExeCmdItem      *pCmdItem; 

	 if((hItem = GetMouseClickedItem(pNMHDR,pResult)) == NULL)
		 return;

     m_cmdTreeView.SelectItem(hItem);

 	 pCmdItem = dynamic_cast<CCmdExeCmdItem*>((CCmdExeItem*)m_cmdTreeView.GetItemData(hItem));
	 if(pCmdItem == NULL)
	  	 return;

     GetCursorPos(&point);   
	 itemMenu.CreatePopupMenu();

	 bmpImage.LoadBitmap(IDB_MENU_COPY_BITMAP); 
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_COPY_ITEM ,  "Copy",&bmpImage);
	 bmpImage.DeleteObject();

 	 bmpImage.LoadBitmap(IDB_MENU_PASTE_BITMAP); 
	 if(m_CmdItemClipboard.m_bIsEmpty == TRUE)
		 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_GRAYED, ID_CE_PASTE_ITEM ,  "Paste",&bmpImage);
	 else
		 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_PASTE_ITEM ,  "Paste",&bmpImage);
	 bmpImage.DeleteObject();

	 bmpImage.LoadBitmap(IDB_MENU_DUPLICATE_BITMAP); 
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_CE_DUPLICATE_ITEM ,  "Duplicate",&bmpImage);
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION|MF_SEPARATOR);
 	 bmpImage.DeleteObject();

	 bmpImage.LoadBitmap(IDB_REMOVE_BMP); 
	 itemMenu.InsertMenu(mMenuItem++,MF_BYPOSITION, ID_CE_REMOVE_ITEM ,  "Remove",&bmpImage);
	 bmpImage.DeleteObject();

	 itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

	//*pResult = 0;
}
