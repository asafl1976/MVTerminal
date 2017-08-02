#pragma once
#include "ODPGHwModuleBasicDlg.h"
#include "IHwModuleItemDlg.h"

// CODPGHwModuleModelessDlg dialog

class CODPGHwModuleRegAccessModelessDlg;

class CODPGHwModuleModelessDlg : public CODPGHwModuleBasicDlg, public IHwModuleItemDlg
{
	//For cascading modeless windows
	static	int				             m_nWinPos;
	CODPGHwModuleRegAccessModelessDlg*   m_pRWRegDlg;

public:

	CODPGHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CODPGHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_ODPG_CTRL_DLG };

protected:
	BOOL	     			ActivateODPG();
	BOOL		 			SetODPGParams();
	virtual void 			AlignWindow();
	virtual void 			OnOK();

	virtual void 			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void 			OnBnClickedOdpgCtrlRwRegBtn();
	virtual BOOL			OnInitDialog();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	virtual void			InvalidateDlg() {m_pRWRegDlg = NULL;}
};
