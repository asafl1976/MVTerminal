#pragma once
#include "afxwin.h"


// CScriptSettingsDlg dialog

class CScriptSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CScriptSettingsDlg)

	BOOL	 m_bLoadDefaultScripts;
	CString	 m_sDefaultScriptPath;
	CString	 m_sAppDataPath;

	BOOL	 ImportSettingsFromFile();
	BOOL	 ExportSettingsToFile();

public:
	CScriptSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScriptSettingsDlg();
	void         SetDefaultScriptPath(CString sDefaultScriptPath) {m_sDefaultScriptPath = sDefaultScriptPath;}
	CString      GetDefaultScriptPath() {return m_sDefaultScriptPath;}
	BOOL         GetLoadDefaultScript() {return m_bLoadDefaultScripts;}

	
// Dialog Data
	enum { IDD = IDD_APP_SCRIPT_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
private:
	CXTPButton m_browseBtn;
public:
	afx_msg void OnBnClickedBrowseBtn();
private:
	CEdit m_defaultScriptPathedit;
public:
	afx_msg void OnBnClickedLoadDefaultScriptsCheck();
	afx_msg void OnBnClickedPromptToSaveScriptsCheck();
	virtual BOOL OnInitDialog();
};
