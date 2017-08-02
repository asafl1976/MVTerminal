#include "StdAfx.h"
#include ".\cmdexetreeview.h"
#include "resource.h"
#include "CmdExeCmd.h"

CCmdExeTreeView::CCmdExeTreeView(void)
{
}

CCmdExeTreeView::~CCmdExeTreeView(void)
{
}

void CCmdExeTreeView::CreateImageList()
{
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

	SetImageList(&m_cImageList, TVSIL_NORMAL);	
}

void CCmdExeTreeView::DeleteItemChilds(HTREEITEM tItem)
{
	HTREEITEM tChild;

	CTreeCtrl t;

	while((tChild = GetChildItem(tItem)) != NULL)
	{
		DeleteItem(tChild);
	}
}

HTREEITEM CCmdExeTreeView::GetTreeItem(CCmdExeItem* pCmdExeItem)
{
	HTREEITEM		tRoot,tTheItem = NULL;

	tRoot = GetRootItem();
	FindTreeItem((DWORD_PTR)pCmdExeItem,tTheItem,tRoot);
	
	if(tTheItem != NULL)
		return tTheItem;

	while((tRoot = GetNextSiblingItem(tRoot) )!= NULL)
	{
		FindTreeItem((DWORD_PTR)pCmdExeItem,tTheItem,tRoot);
		if(tTheItem != NULL)
			return tTheItem;
	}

	return NULL;

}
void CCmdExeTreeView::FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem)
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

BOOL CCmdExeTreeView::UpdateCommand(HTREEITEM tItem, CCmdExeCmd *pCommand)
{
	HTREEITEM tChildItem;
    CString   sDisplayName;
	
	sDisplayName = pCommand->GetDisplayName();
	SetItemText(tItem,sDisplayName);
	SetItemData(tItem,(DWORD_PTR)pCommand);
	SetItemBold(tItem);
	SetCheck(tItem,pCommand->GetEnable());

	int		     nItem, nItemCount = pCommand->GetCmdItemCount();
	CCmdExeItem *pItem;

	for(nItem = 0; nItem < nItemCount; nItem++)
	{
		if((pItem = pCommand->GetCmdItem(nItem)) != NULL)
		{
			tChildItem = InsertItem(pItem->GetDisplayName(),2,2,tItem);
			SetCheck(tChildItem,pItem->GetEnable());
			SetItemData(tChildItem,(DWORD_PTR)pItem);
		}
	}
	return TRUE;
}

//Triggered by presenter
BOOL CCmdExeTreeView::OnUpdateCommand(CCmdExeCmd *pCommand)
{	
	HTREEITEM tCmdItem = NULL;
    CString   sDisplayName;
	
	tCmdItem = GetTreeItem(pCommand);
	if(tCmdItem == NULL)
	{
		AfxMessageBox("OnUpdateCommand: Can't find item " + pCommand->GetDisplayName());
		return FALSE;
	}
	DeleteItemChilds(tCmdItem);
	UpdateCommand(tCmdItem,pCommand);
	return TRUE;
}

//Triggered by presenter
BOOL CCmdExeTreeView::OnAddCommand(CCmdExeCmd *pCommand)
{	
	HTREEITEM tCmdItem;
    CString   sDisplayName;
	
	sDisplayName = pCommand->GetDisplayName();
	tCmdItem = InsertItem(sDisplayName,1,1,TVI_ROOT);
	return UpdateCommand(tCmdItem,pCommand);
}

BOOL CCmdExeTreeView::OnRemoveCommand(CCmdExeCmd *pCommand)
{
	HTREEITEM tCmdItem = NULL;
	
	tCmdItem = GetTreeItem(pCommand);
	if(tCmdItem == NULL)
	{
		AfxMessageBox("OnRemoveCommand: Can't find item");
		return FALSE;
	}
	DeleteItem(tCmdItem);
	return TRUE;
}

//Triggered by user
BOOL CCmdExeTreeView::LoadCommands(CMarkup& xmlReader)
{
	return NotifyLoadCommands(xmlReader);
}

BOOL CCmdExeTreeView::SaveCommands(CMarkup& xmlWriter)
{
	return NotifySaveCommands(xmlWriter);
}

BOOL CCmdExeTreeView::RemoveAllCommands()
{
	return NotifyRemoveAllCommands();
}
BOOL CCmdExeTreeView::RemoveCommand(CCmdExeCmd *pCommand)
{
	return NotifyRemoveCommand(pCommand);
}

BOOL CCmdExeTreeView::AddCommand(CCmdExeCmd *pCommand)
{
	return NotifyAddCommand(pCommand);
}

BOOL CCmdExeTreeView::UpdateCommand(CCmdExeCmd *pCommand)
{
	return NotifyUpdateCommand(pCommand);
}

int  CCmdExeTreeView::GetCommandCount()
{
	HTREEITEM tiCmd;
	int		  nCount = 0;

	tiCmd = GetRootItem();
	if(tiCmd != NULL)
	{
		nCount++;
		while((tiCmd = GetNextSiblingItem(tiCmd)) != NULL)
		{
			nCount++;
		}
	}
	return nCount;
}

