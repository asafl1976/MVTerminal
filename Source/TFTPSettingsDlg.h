#pragma once


// CTFTPSettingsDlg dialog

class CTFTPSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTFTPSettingsDlg)

	CString		m_sAppDataPath;
	BOOL		m_bConnectOnAppStartup;
	BOOL		m_bWarnTFTPFailure;
	BOOL       	ExportSettingsToFile();
	BOOL		ImportSettingsFromFile();


public:
	CTFTPSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTFTPSettingsDlg();
	BOOL		 GetAutoConnect() {return m_bConnectOnAppStartup;}
	BOOL		 GetWarnTFTPFailure() {return m_bWarnTFTPFailure;}

// Dialog Data
	enum { IDD = IDD_APP_TFTP_SERVER_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedConAutoCheck();
	void		 OnOk();

	afx_msg void OnBnClickedTftpFailCheck();
};
