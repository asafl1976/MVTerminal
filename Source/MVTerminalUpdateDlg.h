#pragma once
#include "afxcmn.h"
#include "FontStatic.h"


// CMVTerminalUpdateDlg dialog

class CMVTerminalUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CMVTerminalUpdateDlg)

	CString m_sChangeLog;
	CString m_sNewVersionMsg;
	CXTPButton m_updateBtn;
	CXTPButton m_ignoreBtn;
	CFontStatic m_newVersionMsgStatic;

public:
	CMVTerminalUpdateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMVTerminalUpdateDlg();

// Dialog Data
	enum { IDD = IDD_MVT_UPDATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void SetChangeLog(CString sChangeLog){m_sChangeLog = sChangeLog;}
	virtual void SetNewVersionMsg(CString sNewVersionMsg){m_sNewVersionMsg = sNewVersionMsg;}

	CRichEditCtrl m_changeLogEdit;
};
