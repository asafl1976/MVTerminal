#pragma once


#include "afxcmn.h"
#include "afxwin.h"
#include "ScriptSettingsDlg.h"
#include "AppAppearanceSettingsDlg.h"
#include "TFTPSettingsDlg.h"
#include "AppGeneralSettingDlg.h"
#include "BoardPowerCtrlSettingDlg.h"

// CAppSettingsDlg dialog

class CAppSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAppSettingsDlg)

	CScriptSettingsDlg			 m_scriptSettingsTab;
	CAppAppearanceSettingsDlg    m_appearanceSettingsTab;
	CAppGeneralSettingDlg        m_generalSettingsTab;
	CTFTPSettingsDlg			 m_tftpServerSettingsTab;	
	CBoardPowerCtrlSettingDlg    m_boardPowerCtrlSettingsTab;

	CTabCtrl		              m_tabCtrl;
	CRect						  m_tabItemRect;

public:
	CAppSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAppSettingsDlg();
	
	CScriptSettingsDlg&			GetScriptSettingsTab()        {return m_scriptSettingsTab;}
	CAppAppearanceSettingsDlg&  GetAppearanceSettingsTab()    {return m_appearanceSettingsTab;}
	CTFTPSettingsDlg&           GetTFTPSettingsTab()          {return m_tftpServerSettingsTab;}
	CAppGeneralSettingDlg&      GetGeneralSettingTab()        {return m_generalSettingsTab;}
	CBoardPowerCtrlSettingDlg&  GetBoardPowerCtrlSettingTab() {return m_boardPowerCtrlSettingsTab;}

	void						ShowWindowNumber(int number);
	afx_msg void				OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void				OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);

// Dialog Data
	enum { IDD = IDD_APP_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private: 

public:
	virtual BOOL OnInitDialog();
private:
	CXTPButton m_okBtn;
	CXTPButton m_cancelBtn;
public:
	afx_msg void OnBnClickedOk();
};
