#pragma once
#include "sub20hwmodulectrlgpiounitbasicdlg.h"
#include "IHwModuleItemDlg.h"

class CSUB20HwModuleCtrlGPIOUnitModelessDlg : public CSUB20HwModuleCtrlGPIOUnitBasicDlg, public IHwModuleItemDlg
{
	//DECLARE_DYNAMIC(CSUB20HwModuleCtrlGPIOUnitModelessDlg)
public:
	// Dialog Data
	enum { IDD = IDD_SUB20_GPIO_UNIT_DLG };

	CSUB20HwModuleCtrlGPIOUnitModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSUB20HwModuleCtrlGPIOUnitModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

protected:
	
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL			OnInitDialog();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	afx_msg void            OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnLvnItemActivateSub20GpioUnitListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
};
