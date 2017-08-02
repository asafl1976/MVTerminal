// MVTerminal.h : main header file for the MVTerminal application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MVTerminalView.h"
#include <Winsock2.h>
#include "AppSettingsDlg.h"
#include "SaveConfigDlg.h"
#include <vector>
using namespace std;



// CMVTerminalApp:
// See MVTerminal.cpp for the implementation of this class
//

class CMVTerminalApp : public CWinApp
{	
	CString							m_sHostIPAddress;
	CString							m_sInstallPath;
	CString							m_sAppDataPath;
	CAppSettingsDlg					m_appSettingsDlg;
	CStringArray					m_ipAddressArray;
	bool							m_bEnableNewSessionBtn;
	CCommandLineInfo				m_cmdInfo;
	CSaveConfigDlg			        m_saveCfgDlg;
	int							    m_nRC;
	CString							m_sAppUpdateVersion;
	CString							m_sAppUpdateExePath;
	HMODULE				            m_hIPMDLLModule;
	BOOL						    m_bCloseUponUpdate;
	BOOL						    m_bPowerCtrlPowerEn;
	BOOL						    m_bPowerCtrlResetEn;
	int						        m_nPowerCtrlResetCheck;
	int						        m_nPowerCtrlPowerCheck;

private:
	virtual BOOL		GetXmlPlatformInfoFiles(CString sPath, vector<CString> & xmlFilesVec);
public:
	CMVTerminalApp();
	virtual ~CMVTerminalApp();

// Overrides 
public:
	void                SetSaveConfigDlgCloseBtnText(CString sText);
	CString			    GetCurrentWorkingDir();
	void			    SetCurrentWorkingDir(CString sDir);
	virtual  void       OnDeleteObject();

	BOOL				CheckForApplicationUpdates();
	BOOL				CheckForPlatformInfoNewFileUpdates(vector<CString> & remoteFilesToCopyVec);
	BOOL				ShowNewUpdateMessage();
	void                OnCheckForUpdates();

	virtual BOOL		InitInstance();
	virtual BOOL		IsBoardPowerStartegySet();
	virtual BOOL        IsBoardResetStartegySet();
	virtual VOID		ResetBoardPowerCtrlHwModuleItem();
	virtual BOOL		SetBoardPowerCtrlHwModuleItem(CHwModuleItem *pItem);
	virtual BOOL        InitBoardPowerCtrlManager();
	virtual void		InitHwModulesModel(BOOL bDetHwModulesOnStartup);
	virtual void		EnableNewSessionBtn(bool bEnable){m_bEnableNewSessionBtn = bEnable;}
	int				    EnumerateSerialPorts(vector<CString> &comPortVec);
	int					GetComPortCount();
	CMVTerminalView    *GetActiveView();
	void                SelectLocalHostIPAddress();
	BOOL                InitLocalHostIPAddressList() ;
	CString             GetLocalHostIPAddress() ;
	CString             GetInstallationPath();
	CString             GetAppDataPath();
	CString				GetRegistryEntryValue(CString sEntry);
	CMainFrame*         GetMainFrame() {return reinterpret_cast<CMainFrame *>(m_pMainWnd);}
	CString				GetDefaultScriptsFolder() {return m_appSettingsDlg.GetScriptSettingsTab().GetDefaultScriptPath();}
	BOOL				GetLoadDefaultScript() {return m_appSettingsDlg.GetScriptSettingsTab().GetLoadDefaultScript();}
	BOOL				GetTFTPServerAutoConnect() {return m_appSettingsDlg.GetTFTPSettingsTab().GetAutoConnect();}
	BOOL				GetWarnTFTPFailure() {return m_appSettingsDlg.GetTFTPSettingsTab().GetWarnTFTPFailure();}
	CAppSettingsDlg&    GetAppSettingsDlg() {return m_appSettingsDlg;}
	
	void				EnablePowerCtrlReset(BOOL bEnable);
	void				EnablePowerCtrlPower(BOOL bEnable);
	void 				SetPowerCtrlPowerCheck(int nCheck);
	void 				SetPowerCtrlResetCheck(int nCheck);

	void			    OnLoadConfiguration();
	void			    OnSaveConfiguration();
	void				OnRestoreConfiguration();

	void				CreateRestoreFile();
	int 				SaveConfiguration();

	BOOL			    LoadConfiguration(CString sFile);
	BOOL				SaveConfiguration(CString sFile);

	BOOL				CreateNewSession(CViewPropertiesDlg*  pViewPropertiesDlg, BOOL bOpenViewPropertiesDlg = TRUE);
	int  				OpenSaveConfigurationDlg();

	CString				Serialize();
	BOOL			    DeSerialize(CMarkup & xmlReader);

	BOOL				GetConfigurationDataAvailability();


// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPowerCtrlPowerUpdate(CCmdUI* pCmdUI);
	afx_msg void OnPowerCtrlResetUpdate(CCmdUI* pCmdUI);
	afx_msg void OnPowerCtrlPower();
	afx_msg void OnPowerCtrlReset();

	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNewUpdate(CCmdUI* pCmdUI);
	
	afx_msg void OnAppSettings();
	afx_msg void OnExportScripts();
	afx_msg void OnImportScripts();

	afx_msg void OnSendTextFile();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	virtual void OnFileSave();
	virtual int ExitInstance();
	afx_msg void OnAppExit();
};

extern CMVTerminalApp theApp;

