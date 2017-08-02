#pragma once

#include "MarvellDeviceHwModuleBasicDlg.h"
#include "IHwModuleItemDlg.h"

// CODPGHwModuleRegAccessModelessDlg dialog

class CMarvellDeviceHwModuleModelessDlg : public CMarvellDeviceHwModuleBasicDlg, public IHwModuleItemDlg
{
protected:
	static	int				 m_nWinPos;
	CMarvellDeviceHwModule*  m_pMarvellDeviceModule;
public:
	CMarvellDeviceHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMarvellDeviceHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_MARVELL_DEVICE_HWMODULE_DLG };
protected:


	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void		    AlignWindow();
	virtual BOOL			OnInitDialog();
	virtual void			OnBnClickedOk();
	afx_msg void			OnBnClickedSummitDlgSettingsBtn();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();
	
	DECLARE_MESSAGE_MAP()
};
