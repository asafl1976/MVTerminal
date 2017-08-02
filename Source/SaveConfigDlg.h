#pragma once
#include "afxwin.h"


// CSaveConfigDlg dialog

class CSaveConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveConfigDlg)

public:
	CSaveConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveConfigDlg();

// Dialog Data
	enum { IDD = IDD_SAVE_CONFIGURATION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CXTPButton m_browseBtn;
	CXTPButton m_saveBtn;
	CXTPButton m_cancelBtn;
	int		  m_nChoice;

	BOOL	m_bScriptFiles;
	BOOL	m_bTFTPFiles;
	BOOL	m_bCommandExeFiles;
	BOOL	m_bHwModulesSettings;
	BOOL	m_bSessions;

	BOOL	m_bScriptFilesCheckEn;
	BOOL	m_bTFTPFilesCheckEn;
	BOOL	m_bCommandExeFilesCheckEn;
	BOOL	m_bSessionsCheckEn;
	BOOL	m_bHardwareModulesCheckEn;
	CString m_sAppDataPath;
	CString m_sCloseBtnText;

	CString	m_sPath;

	BOOL GetScriptFilesEn()     {return m_bScriptFiles;}
	BOOL GetTFTPFilesEn()       {return m_bTFTPFiles;}
	BOOL GetCommandExeFilesEn() {return m_bCommandExeFiles;}
	BOOL GetSessionsEn()        {return m_bSessions;}
	CString GetPath()           {return m_sPath;}
	void    SetPath(CString sPath)           {m_sPath = sPath;}
	void    SetCloseBtnText(CString sCloseBtnText) {m_sCloseBtnText = sCloseBtnText;}

	void		 EnableSaveScriptCheck(BOOL bEnable);
	void		 EnableSaveTFTPCheck(BOOL bEnable);
	void		 EnableSaveCommandExeCheck(BOOL bEnable);
	void		 EnableSaveSessionsCheck(BOOL bEnable);
	void		 EnableSaveHardwareModulesCheck(BOOL bEnable);
	int  		 GetChoice() {return m_nChoice;	}

	afx_msg void OnBnClickedScBrowseBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedScSaveScriptsCheck();
	afx_msg void OnBnClickedScSaveTftpFilesCheck();
	afx_msg void OnBnClickedScSaveCommandExeFilesCheck();
	afx_msg void OnBnClickedScCurrWinSessionCheck();
	virtual BOOL OnInitDialog();
	CComboBox m_lruCombo;
	afx_msg void OnCbnEditchangeScConfigFilePathCombo();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSaveBtn();
	afx_msg void OnClose();
	afx_msg void OnBnClickedScSaveHwmoduelsSettingsCheck();
};
