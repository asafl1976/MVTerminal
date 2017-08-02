#pragma once
#include "afxwin.h"
#include "FontStatic.h"


// CPlatformInfoUpdateDlg dialog

class CPlatformInfoUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlatformInfoUpdateDlg)

public:
	CPlatformInfoUpdateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlatformInfoUpdateDlg();

// Dialog Data
	enum { IDD = IDD_PLATFORM_INFO_UPDATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> m_filesVec;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CListBox m_fileList;
	CFontStatic m_newFilesMsgStatic;

};
