#pragma once
#include "afxwin.h"

// CAppGeneralSettingDlg dialog

class CAppGeneralSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CAppGeneralSettingDlg)

	BOOL	 m_bPromptToSaveSettings;
	BOOL	 m_bCheckForUpdates;
	BOOL	 m_bEnableIPMMessagesOnStartup;
	CString	 m_sAppDataPath;

public:
	CAppGeneralSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAppGeneralSettingDlg();
	afx_msg void OnOk();	
	BOOL     GetPromptToSaveSettings() {return m_bPromptToSaveSettings;}	
	BOOL     GetCheckForUpdates()  {return m_bCheckForUpdates;}	
	BOOL     GetEnableIPMMessagesOnStartup()  {return m_bEnableIPMMessagesOnStartup;}	
	BOOL	 ImportSettingsFromFile();
	BOOL	 ExportSettingsToFile();



// Dialog Data
	enum { IDD = IDD_APP_GENERAL_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPromptToSaveScriptsCheck();
	afx_msg void OnBnClickedPromptToSaveEntireSettingsCheck();
	afx_msg void OnBnClickedPromptToSaveSessionCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckForNewUpdates();
	afx_msg void OnBnClickedEnableIpmMsgOnStartup();
};
