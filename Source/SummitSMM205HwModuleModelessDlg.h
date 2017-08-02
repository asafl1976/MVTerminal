#pragma once
#include "SummitSMM205HwModuleBasicDlg.h"
#include "resource.h"
#include "IHwModuleItemDlg.h"

// CSummitSMM205HwModuleModelessDlg dialog

class CSummitSMM205HwModuleModelessDlg : public CSummitSMM205HwModuleBasicDlg, public IHwModuleItemDlg
{
	//For cascading modeless windows
	static	int				m_nWinPos;
	CSummitSMM205HwModule* 	m_pSummitHwModule;

public:
	CSummitSMM205HwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSummitSMM205HwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_SUMMIT_SMM205_DLG };

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
