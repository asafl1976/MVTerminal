#pragma once
#include "afxwin.h"
#include "BasicItemDlg.h"
#include "ODPGHwModule.h"
#include "ODPGHwModuleRegAccessBasicDlg.h"
#include "resource.h"

// CODPGHwModuleRegAccessBasicDlg dialog

class CODPGHwModuleRegAccessBasicDlg : public CBasicItemDlg
{
protected:

	CXTPButton							m_okBtn;
	CXTPButton							m_cancelBtn;
	CCEODPGRWRegTransCmdItemParams		m_params;

public:
	CODPGHwModuleRegAccessBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CODPGHwModuleRegAccessBasicDlg();

	CCEODPGRWRegTransCmdItemParams      GetParams() {return m_params;}
	void                                SetParams(CCEODPGRWRegTransCmdItemParams params) {m_params = params;}

// Dialog Data
	enum { IDD = IDD_ODPG_CTRL_READWRITE_REG_DLG };

protected:
	virtual void 						DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL 						OnInitDialog();
	afx_msg void 						OnBnClickedCancel();
	afx_msg void 						OnBnClickedOdpgCtrlRegReadRadio();
	afx_msg void 						OnBnClickedOdpgCtrlRegWriteRadio();
	DECLARE_MESSAGE_MAP()
};
