#include "StdAfx.h"
#include ".\SysProfilerTreeView.h"
#include "resource.h"

CSysProfilerTreeView::CSysProfilerTreeView(void)
{
	m_pSelEvent = NULL;
}

CSysProfilerTreeView::~CSysProfilerTreeView(void)
{
}

void CSysProfilerTreeView::CreateImageList()
{
	m_cImageList.Create(16,16, ILC_COLOR24 , 1, 0);
	CBitmap bmpImage;

	bmpImage.LoadBitmap(IDB_TITLE_BMP);
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_GRAPH_ICON); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_GRAPH_ICON_SEL); 
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	SetImageList(&m_cImageList, TVSIL_NORMAL);	
}

HTREEITEM CSysProfilerTreeView::GetTreeItem(CSysProfilerEvent *pSysProfilerEvent)
{
	HTREEITEM		tRoot,tTheItem = NULL;

	tRoot = GetRootItem();
	FindTreeItem((DWORD_PTR)pSysProfilerEvent,tTheItem,tRoot);
	
	if(tTheItem != NULL)
		return tTheItem;

	while((tRoot = GetNextSiblingItem(tRoot) )!= NULL)
	{
		FindTreeItem((DWORD_PTR)pSysProfilerEvent,tTheItem,tRoot);
		if(tTheItem != NULL)
			return tTheItem;
	}

	return NULL;

}


void CSysProfilerTreeView::FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem)
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

void CSysProfilerTreeView::SetSelectedEvent(CSysProfilerEvent *pSysProfilerEvent)
{
	m_pSelEvent = pSysProfilerEvent;
}

CSysProfilerEvent* CSysProfilerTreeView::GetSelectedEvent()
{
	return m_pSelEvent;
}

void CSysProfilerTreeView::RemoveAllEvents()
{
	HTREEITEM ti;

	ti = GetRootItem();
	if(ti != NULL)
	{
		DeleteItem(GetRootItem());

		return;

		while((ti = GetNextSiblingItem(ti)) != NULL)
		{
			DeleteItem(ti);
		}
	}
	DeleteItem(GetRootItem());

}

void CSysProfilerTreeView::ExpandTree(HTREEITEM tiStartItem)
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


void	CSysProfilerTreeView::ExpandTree()
{
	HTREEITEM		tRoot;

	tRoot = GetRootItem();
	ExpandTree(tRoot);
	while((tRoot = GetNextSiblingItem(tRoot) )!= NULL)
	{
		ExpandTree(tRoot);
	}
}


BOOL CSysProfilerTreeView::AddEvent(CSysProfilerEvent *pSysProfilerEvent)
{
	HTREEITEM tCmdItem;
    CString   sName;
	
	HTREEITEM	tRoot,tTheItem = NULL;

	tRoot = GetRootItem();

	if(tRoot == NULL)
	{
		//create root item
		tRoot = InsertItem("Events",0,0,TVI_ROOT);
		SetItemImage(tRoot,0,0);
	}

	sName = pSysProfilerEvent->GetName();
	tCmdItem = InsertItem(sName,1,2,tRoot);
	SetItemData(tCmdItem,(DWORD_PTR)pSysProfilerEvent);
	ExpandTree();
	return TRUE;
}

int  CSysProfilerTreeView::GetEventCount()
{
	HTREEITEM ti;
	int		  nCount = 0;

	ti = GetRootItem();
	if(ti != NULL)
	{
		nCount++;
		while((ti = GetNextSiblingItem(ti)) != NULL)
		{
			nCount++;
		}
	}
	return nCount;
}

void  CSysProfilerTreeView::HandleEventCheck(HTREEITEM ti)
{
	HTREEITEM tiTemp;

	SelectItem(ti);
	tiTemp = GetRootItem();
	if(tiTemp != NULL)
	{
		if(ti != tiTemp)
		{
			if(GetCheck(tiTemp) == TRUE)
			{
				SetCheck(tiTemp,FALSE);
			}
		}

		while((tiTemp = GetNextSiblingItem(tiTemp)) != NULL)
		{
			if(ti != tiTemp)
			{
				if(GetCheck(tiTemp) == TRUE)
				{
					SetCheck(tiTemp,FALSE);
				}
			}
		}
	}
}


