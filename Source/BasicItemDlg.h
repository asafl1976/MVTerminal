#pragma once

#define WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED			      (WM_USER+100)
#define WM_HW_MODULE_ITEM_MODELESS_DLG_UNIT_ITEM_DIS_NAME_CHANGE  (WM_USER+101)
#define WM_HW_MODULE_MODELESS_DLG_DIS_NAME_CHANGE                 (WM_USER+102)

class CBasicItemDlg: public CDialog
{
public:
	CBasicItemDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CBasicItemDlg(void);

	virtual void			AlignWindow();
	virtual CBasicItemDlg * Clone() = 0;
};
