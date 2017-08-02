#include "StdAfx.h"
#include "PlatformInfoTreeView.h"
#include "resource.h"
#include "PlatformInfoManager.h"

CPlatformInfoTreeView::CPlatformInfoTreeView(void)
{
}

CPlatformInfoTreeView::~CPlatformInfoTreeView(void)
{
}

void CPlatformInfoTreeView::CreateImageList()
{
	m_cImageList.Create(16,16, ILC_COLOR32 , 1, 0);
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_TITLE_BMP); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_CE_CMD_BMP);
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	SetImageList(&m_cImageList, TVSIL_NORMAL);	
}

void CPlatformInfoTreeView::FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem)
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

void CPlatformInfoTreeView::AddSubItems(IPlatformInfoItem *pPlatformInfoItem, HTREEITEM tiParent)
{
	HTREEITEM tiItem, tiParentItem = tiParent;
	long nItem = 0,nSubItemCount = 0; 
		
	pPlatformInfoItem->GetSubItemCount(&nSubItemCount);

	for(nItem = 0; nItem < nSubItemCount; nItem++)
	{
		IPlatformInfoItem* pSubItem;
		pPlatformInfoItem->GetSubItemAtIndex(nItem,&pSubItem);
		string sDisplayName = CPlatformInfoManager::GetInstance().GetItemAttributeValue(pSubItem,"DisplayName");
		if((tiItem = InsertItem(sDisplayName.c_str(),1,1,tiParentItem)) != NULL)
		{
			SetItemBold(tiItem);
			SetItemData(tiItem,(DWORD_PTR)pSubItem);
		}
	}
}


void CPlatformInfoTreeView::AddPlatformInfoCategory(IPlatformInfoItem* pPlatformInfoCategory)
{
	HTREEITEM tiItem, tiParentItem = TVI_ROOT;
	string sDisplayName = CPlatformInfoManager::GetInstance().GetItemAttributeValue(pPlatformInfoCategory,"DisplayName");
	if((tiItem = InsertItem(sDisplayName.c_str() ,0,0,tiParentItem)) != NULL)
	{
		SetItemBold(tiItem);
		SetItemData(tiItem,(DWORD_PTR)pPlatformInfoCategory);
		AddSubItems((IPlatformInfoItem*)pPlatformInfoCategory,tiItem);
	}
	Expand(tiItem,TVE_EXPAND);
}
