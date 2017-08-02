#pragma once

#include "ODPGHwModuleRegAccessBasicDlg.h"
#include "IHwModuleItemDlg.h"

// CODPGHwModuleRegAccessModelessDlg dialog

class CODPGHwModuleRegAccessModelessDlg : public CODPGHwModuleRegAccessBasicDlg, public IHwModuleItemDlg
{
protected:
	CODPGHwModule*          m_pODPGModule;
public:
	CODPGHwModuleRegAccessModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CODPGHwModuleRegAccessModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_ODPG_CTRL_READWRITE_REG_DLG };
protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void			OnOK();
	virtual BOOL			OnInitDialog();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();
	
	DECLARE_MESSAGE_MAP()
};
