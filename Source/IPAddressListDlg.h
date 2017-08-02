#pragma once
#include "afxwin.h"


// CIPAddressListDlg dialog

class CIPAddressListDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPAddressListDlg)
	CStringArray   m_strArray;
	CString        m_sSelection;

public:
	CIPAddressListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIPAddressListDlg();
	virtual void AddIPAddress(CString sIPAddress);   
	virtual CString GetSelection();

// Dialog Data
	enum { IDD = IDD_IP_ADDRESS_LIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListBox m_ipListCtrl;
	virtual BOOL OnInitDialog();
	CXTPButton m_okBtn;
	afx_msg void OnLbnDblclkIpAddressList();
};
