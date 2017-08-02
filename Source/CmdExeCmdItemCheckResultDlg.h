#pragma once

#include "afxwin.h"
#include "resource.h"
#include "CmdExeCmdItemCheckResult.h"
#include "BasicItemDlg.h"
#include "ICmdExeCmdItemDlg.h"


class CCmdExeCmdItemCheckResultDlg : public CBasicItemDlg, public ICmdExeCmdItemDlg 
{
	CComboBox					    m_targetWinCombo;
	CXTPButton					    m_okBtn;
	CXTPButton						m_cancelBtn;
	CXTPButton						m_cmpFileBrowseBtn;
	CCmdExeCmdItemCheckResult      *m_pItem;

	CCmdExeCmdItemCheckResultParams m_params;

public:
	CCmdExeCmdItemCheckResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdExeCmdItemCheckResultDlg();

// Dialog Data
	enum { IDD = IDD_CE_CHECK_RESULT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void                     OnBnClickedOk();
	afx_msg void                     OnBnClickedCeCompareStrRadio();
	afx_msg void                     OnBnClickedCompareTextFileRadio();
	afx_msg void                     OnBnClickedCeCheckResultBrowseBtn();
	afx_msg void                     OnBnClickedCeCheckResFailIfFoundCheck();
	afx_msg void			         OnBnClickedCeCheckResultLogBrowseBtn();
	virtual BOOL                     OnInitDialog();

	CCmdExeCmdItemCheckResultParams  GetParams() {return m_params;}
	void                             SetParams(CCmdExeCmdItemCheckResultParams params) {m_params = params;}

public:	
	virtual CBasicItemDlg*           Clone();
	virtual void					 SetCmdItem(CCmdExeCmdItem* pItem);

};
