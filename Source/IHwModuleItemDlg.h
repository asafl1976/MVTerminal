#pragma once

class CHwModuleItem;

#define WM_HW_MODULE_NOTIFY_SELECT	        (WM_USER+112)
#define WM_HW_MODULE_NOTIFY_DESELECT	    (WM_USER+113)

class IHwModuleItemDlg
{
public:
	IHwModuleItemDlg(void);
	virtual ~IHwModuleItemDlg(void);

	virtual void SetHwModuleItem(CHwModuleItem *pItem) = 0;
	virtual BOOL Create(CWnd* pParentWnd)              = 0;
};
