// DragDropTreeCtrl.h
// For OLE Drag and Drop between tree controls
// Designed and developed by Vinayak Tadas
// vinayakt@aditi.com
// 

#if !defined(AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_)
#define AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragDropTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDragDropTreeCtrl window
#include "TreeDropTarget.h"
#include <vector>
using namespace std;

#define TVM_TREEITEM_ADDED (WM_USER+20)

// *****************************************************************
// CDragDropTreeCtrl control
// *****************************************************************

class CDragDropTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CString	  m_sRootName;
	HTREEITEM m_hRootItem;
	BOOL	  m_bIsDropable;
	BOOL	  m_bCanMoveItems;
	vector<CString>  m_nonDragableItemVec;

	CDragDropTreeCtrl();

// Attributes
public:
	// Adds an item to the tree control
	HTREEITEM AddItem( HTREEITEM hParent, CString csItemName, 
	HTREEITEM hInsAfter, int iSelImage , int iNonSelImage,	long lParam);
	virtual void SetItemBold(HTREEITEM tItem, BOOL bBold = TRUE);
	// You can set the tree with checkboxs and than selectivly remove checkboxs with this function
	virtual void RemoveItemCheckBox(HTREEITEM tItem);

	LRESULT OnItemAdded(WPARAM hItem, LPARAM x);
	void CDragDropTreeCtrl::RegisterDropTarget() 
	{
		// Register Tree control as a drop target	
		m_CTreeDropTarget.Register(this);
	}
// Operations
public:
	void SetRootName(CString RootName);
	void SetRootItem(HTREEITEM hRootItem);
	void SetIsDroppable(BOOL bIsDropable);
	void SetCanMoveItems(BOOL bCanMoveItems);
	void ResetNonDragableItems();
	void RemoveNonDragableItem(CString sItemName);
	void AddNonDragableItem(CString sItemName);
	BOOL IsItemDragable(CString sItemName);
	void AllowSelfDropMessage(BOOL bAllow);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragDropTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragDropTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CTreeDropTarget m_CTreeDropTarget;
	virtual void		OnItemCopied(HTREEITEM hItem, HTREEITEM hNewItem );
	virtual HTREEITEM	CopyItem(CTreeCtrl *pSourceTree, HTREEITEM htiSrchItem, HTREEITEM htiNewParent,
											HTREEITEM htiAfter = TVI_LAST );

public:
	virtual void		DeleteItemChilds(HTREEITEM hItem);
	virtual HTREEITEM	CopyBranch(CTreeCtrl *pSourceTree, HTREEITEM htiSrcBranch, HTREEITEM htiNewParent, 
									HTREEITEM htiAfter = TVI_LAST,BOOL bCopyBranchRoot = TRUE);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROPTREECTRL_H__29F8894F_897C_11D3_A59E_00A02411D21E__INCLUDED_)
