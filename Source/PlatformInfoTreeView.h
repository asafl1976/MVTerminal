#pragma once
#include "c:\program files\codejock software\mfc\xtreme toolkitpro v13.2.1\source\controls\xttreectrlview.h"
#import  "C:\Projects\C# Projects\PlatformInfoLib\PlatformInfoLib\bin\release\PlatformInfoLib.tlb" named_guids raw_interfaces_only
using namespace PlatformInfoLib;

class CPlatformInfoTreeView : public CXTTreeCtrl
{
	CImageList    m_cImageList;

	virtual void  FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem);
	virtual void  AddSubItems(IPlatformInfoItem *pPlatformInfoItem, HTREEITEM tiParent);
public:
	CPlatformInfoTreeView(void);
	~CPlatformInfoTreeView(void);
	virtual void CreateImageList();

	virtual void AddPlatformInfoCategory(IPlatformInfoItem* pPlatformInfoCategory);

};
