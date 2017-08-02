#pragma once

#include "UnknownI2CHwModuleBasicDlg.h"
#include "IHwModuleItemDlg.h"

// CUnknownI2CHwModuleModelessDlg dialog

class CUnknownI2CHwModuleModelessDlg : public CUnknownI2CHwModuleBasicDlg, public IHwModuleItemDlg
{
	//For cascading modeless windows
	static	int				m_nWinPos;
	CUnknownI2CHwModule*   	m_pUnknownI2CHwModule;

public:
	CUnknownI2CHwModuleModelessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnknownI2CHwModuleModelessDlg();

	virtual void			SetHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL			Create(CWnd* pParentWnd);
	virtual CBasicItemDlg * Clone();

// Dialog Data
	enum { IDD = IDD_UNKNOWN_I2C_DEV_CTRL_DLG };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void			AlignWindow();
	virtual BOOL			OnInitDialog();
	virtual void			OnOK();

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();

	DECLARE_MESSAGE_MAP()

};
