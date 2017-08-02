// DragDropTreeCtrl.cpp : implementation file
// For OLE Drag and Drop between tree controls
// Designed and developed by Vinayak Tadas
// vinayakt@aditi.com
// 


#include "stdafx.h"
#include "DragDropTreeCtrl.h"
#include ".\dragdroptreectrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl

CDragDropTreeCtrl::CDragDropTreeCtrl()
{
	m_bIsDropable = TRUE;
	m_bCanMoveItems = FALSE;
	AllowSelfDropMessage(FALSE);
}

CDragDropTreeCtrl::~CDragDropTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDragDropTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDragDropTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl message handlers
/*******************************************************************
AddItem()
	Helper Function for adding an item to the tree control
********************************************************************/

HTREEITEM CDragDropTreeCtrl::AddItem( 
	HTREEITEM hParent, 
	CString csItemName, 
	HTREEITEM hInsAfter, 
	int iSelImage , 
	int iNonSelImage, 
	long lParam
)
{
	HTREEITEM hItem;
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvIns;

	tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	int nLength = csItemName.GetLength() ;
	tvItem.pszText = csItemName.GetBuffer(nLength);
	tvItem.cchTextMax = nLength;
	tvItem.lParam = lParam;

	tvItem.iImage = iNonSelImage;
	tvItem.iSelectedImage = iSelImage;
	
	tvIns.item = tvItem;
	tvIns.hInsertAfter = hInsAfter;
	tvIns.hParent = hParent;

	hItem = InsertItem(&tvIns);
	
	return (hItem);
}


int CDragDropTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	// Register Tree control as a drop target	
	//m_CTreeDropTarget.Register(this);
	RegisterDropTarget();
	return 0;
}

/*******************************************************************
OnBegindrag()
	Called when drag operation begins
********************************************************************/
void CDragDropTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;
	// Highlight selected item
	SelectItem(hTSelItem);
	Select(hTSelItem, TVGN_DROPHILITE);

	if(IsItemDragable(GetItemText(hTSelItem)) == FALSE)
	{
		*pResult = 0;
		return;
	}
	
	COleDataSource *poleSourceObj = new COleDataSource ;
	CTreeDropTarget::m_shWndTreeCtrl = m_hWnd;
	// Begin Drag operation
	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();
	// Remove the highlighting
	SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
	// If user is moving item by pressing Shift, delete selected item
	if ( dropeffect == DROPEFFECT_MOVE)
		DeleteItem(hTSelItem); 
	delete poleSourceObj;
	*pResult = 0;
}

BOOL CDragDropTreeCtrl::IsItemDragable(CString sItemName)
{
	vector<CString>::iterator iter, endIter;
	iter    = m_nonDragableItemVec.begin();
	endIter = m_nonDragableItemVec.end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == sItemName)
			return FALSE;
	}
	return TRUE;
}
void CDragDropTreeCtrl::RemoveNonDragableItem(CString sItemName)
{
	vector<CString>::iterator iter, endIter;
	iter    = m_nonDragableItemVec.begin();
	endIter = m_nonDragableItemVec.end();
	for(; iter != endIter; iter++)
	{
		if((*iter) == sItemName)
		{
			m_nonDragableItemVec.erase(iter);
			break;
		}
	}
}
void CDragDropTreeCtrl::ResetNonDragableItems()
{
	m_nonDragableItemVec.clear();
}

void CDragDropTreeCtrl::AddNonDragableItem(CString sItemName)
{
	m_nonDragableItemVec.push_back(sItemName);
}


void CDragDropTreeCtrl::SetRootItem(HTREEITEM hRootItem)
{
	m_hRootItem = hRootItem;
}
void CDragDropTreeCtrl::SetRootName(CString sName)
{
	m_sRootName = sName;
}
void CDragDropTreeCtrl::SetIsDroppable(BOOL bIsDropable)
{
	m_bIsDropable = bIsDropable;
}

void CDragDropTreeCtrl::SetCanMoveItems(BOOL bCanMoveItems)
{
	m_bCanMoveItems = bCanMoveItems; 
}
void CDragDropTreeCtrl::AllowSelfDropMessage(BOOL bAllow)
{
	m_CTreeDropTarget.m_bAllowSelfDropMessage = bAllow;
}


// CopyBranch           - Copies all items in a branch to a new location
// Returns              - The new branch node
// htiBranch            - The node that starts the branch
// htiNewParent - Handle of the parent for new branch
// htiAfter             - Item after which the new branch should be created
HTREEITEM CDragDropTreeCtrl::CopyBranch(CTreeCtrl *pSourceTree, HTREEITEM htiSrcBranch, HTREEITEM htiNewParent, 
                                         HTREEITEM htiAfter,BOOL bCopyBranchRoot)
{
        HTREEITEM hChild,hNewItem;

		if(bCopyBranchRoot == TRUE)
			hNewItem = CopyItem(pSourceTree, htiSrcBranch, htiNewParent,htiAfter);
		else
			hNewItem = htiNewParent;

        hChild = pSourceTree->GetChildItem(htiSrcBranch);
        while( hChild != NULL)
        {
         // recursively transfer all the items
            CopyBranch(pSourceTree,hChild, hNewItem);  
            hChild = pSourceTree->GetNextSiblingItem( hChild );
        }
        return hNewItem;
}

// CopyItem             - Copies an item to a new location
// Returns              - Handle of the new item
// hItem                - Item to be copied
// htiNewParent         - Handle of the parent for new item
// htiAfter             - Item after which the new item should be created
HTREEITEM CDragDropTreeCtrl::CopyItem(CTreeCtrl *pSourceTree, HTREEITEM hSrcItem, HTREEITEM htiNewParent, 
                                        HTREEITEM htiAfter)
{
        TV_INSERTSTRUCT         tvstruct;
        HTREEITEM               hNewItem;
        CString                 sText;

        // get information of the source item
        tvstruct.item.hItem = hSrcItem;
        tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | 
                                TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        pSourceTree->GetItem(&tvstruct.item);  
        sText = pSourceTree->GetItemText( hSrcItem );
        
        tvstruct.item.cchTextMax = sText.GetLength();
        tvstruct.item.pszText = sText.LockBuffer();

        // Insert the item at proper location
        tvstruct.hParent = htiNewParent;
        tvstruct.hInsertAfter = htiAfter;
        tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
        hNewItem = InsertItem(&tvstruct);
        sText.ReleaseBuffer();

        // Now copy item data and item state.
        SetItemData( hNewItem, pSourceTree->GetItemData( hSrcItem ));
        SetItemState( hNewItem, pSourceTree->GetItemState( hSrcItem, TVIS_STATEIMAGEMASK ), 
                                                        TVIS_STATEIMAGEMASK );

        // Call virtual function to allow further processing in derived class
        OnItemCopied( hSrcItem, hNewItem );

        return hNewItem;
}
void CDragDropTreeCtrl::DeleteItemChilds(HTREEITEM hItem)
{
    HTREEITEM hChildItem;

    while( (hChildItem = GetChildItem(hItem)) != NULL)
    {
		DeleteItem(hChildItem);
    }
}

void CDragDropTreeCtrl::OnItemCopied(HTREEITEM hItem, HTREEITEM hNewItem )
{
        // Virtual function 
}
void CDragDropTreeCtrl::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	TV_ITEM tvi;
	UINT32  nBold = 0;  

	tvi.mask = TVIF_STATE | TVIF_HANDLE;
	tvi.hItem = hItem;

	if(bBold == TRUE)
		nBold = TVIS_BOLD;

	tvi.state = nBold;
	tvi.stateMask = TVIS_BOLD;

	SetItem(&tvi);
}

void CDragDropTreeCtrl::RemoveItemCheckBox(HTREEITEM hItem)
{
	SetItemState( hItem, 0, TVIS_STATEIMAGEMASK );
}


