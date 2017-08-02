#pragma once


// CHwModulePropertiesDlg dialog

class CHwModulePropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CHwModulePropertiesDlg)

	CString m_sDisplayName;
	CString m_sName;
	CString m_sInterfaceName;
	CString m_sID;
public:
	CHwModulePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHwModulePropertiesDlg();

// Dialog Data
	enum { IDD = IDD_HW_MODULE_PROPERTIES_DLG };

	virtual CString GetDisplayName() {return m_sDisplayName;}
	virtual void SetDisplayName(CString sDisplayName) {m_sDisplayName = sDisplayName;}
	virtual void SetName(CString sName) {m_sName = sName;}
	virtual void SetInterfaceName(CString sInterfaceName) {m_sInterfaceName = sInterfaceName;}
	virtual void SetID(CString sID) {m_sID = sID;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
