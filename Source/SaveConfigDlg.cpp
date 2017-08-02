// SaveConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SaveConfigDlg.h"
#include ".\saveconfigdlg.h"


// CSaveConfigDlg dialog

IMPLEMENT_DYNAMIC(CSaveConfigDlg, CDialog)
CSaveConfigDlg::CSaveConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveConfigDlg::IDD, pParent)
{
	m_bScriptFiles = TRUE;
	m_bTFTPFiles = TRUE;
	m_bCommandExeFiles = TRUE;
	m_bSessions = TRUE;
	m_bHwModulesSettings = TRUE;
	m_sPath = "";

	m_bScriptFilesCheckEn = TRUE;
	m_bTFTPFilesCheckEn = TRUE;
	m_bCommandExeFilesCheckEn = TRUE;
	m_bSessionsCheckEn = TRUE;
	m_bHardwareModulesCheckEn = TRUE;
	m_sCloseBtnText = "Close";

	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
}

CSaveConfigDlg::~CSaveConfigDlg()
{

}

void CSaveConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SC_BROWSE_BTN, m_browseBtn);
	DDX_Control(pDX, IDC_SAVE_BTN, m_saveBtn);
	DDX_Control(pDX, IDC_CANCEL_BTN, m_cancelBtn);
	DDX_Control(pDX, IDC_SC_CONFIG_FILE_PATH_COMBO, m_lruCombo);
}


BEGIN_MESSAGE_MAP(CSaveConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_SC_BROWSE_BTN, OnBnClickedScBrowseBtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SC_SAVE_SCRIPTS_CHECK, OnBnClickedScSaveScriptsCheck)
	ON_BN_CLICKED(IDC_SC_SAVE_TFTP_FILES_CHECK, OnBnClickedScSaveTftpFilesCheck)
	ON_BN_CLICKED(IDC_SC_SAVE_COMMAND_EXE_FILES_CHECK, OnBnClickedScSaveCommandExeFilesCheck)
	ON_BN_CLICKED(IDC_SC_CURR_WIN_SESSION_CHECK, OnBnClickedScCurrWinSessionCheck)
	ON_CBN_EDITCHANGE(IDC_SC_CONFIG_FILE_PATH_COMBO, OnCbnEditchangeScConfigFilePathCombo)
	ON_BN_CLICKED(IDC_CANCEL_BTN, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SAVE_BTN, OnBnClickedSaveBtn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SC_SAVE_HWMODUELS_SETTINGS_CHECK, OnBnClickedScSaveHwmoduelsSettingsCheck)
END_MESSAGE_MAP()


// CSaveConfigDlg message handlers

void CSaveConfigDlg::OnBnClickedScBrowseBtn()
{
	char szFilters[128] = "MVTerminal Configuration Files (*.mvtc)|*.mvtc|All Files (*.*)|*.*||";
 	CFileDialog		dlg(FALSE,"mvtc","",OFN_OVERWRITEPROMPT,szFilters,this);
	int			    nCount,i;
	CString			sItem;
	
	if(dlg.DoModal() == IDOK)
	{
		m_sPath = dlg.GetPathName();
		SetDlgItemText(IDC_SC_CONFIG_FILE_PATH_COMBO,m_sPath);

		nCount = m_lruCombo.GetCount();
		for(i = 0; i < nCount ;i++)
		{
			m_lruCombo.GetLBText(i,sItem);

			if(sItem == m_sPath)
			{
				break;
			}
		}
		if(i == nCount)
			m_lruCombo.AddString(m_sPath);
	}
}

void CSaveConfigDlg::EnableSaveScriptCheck(BOOL bEnable)
{
	m_bScriptFilesCheckEn = bEnable;

}
void CSaveConfigDlg::EnableSaveTFTPCheck(BOOL bEnable)
{
	m_bTFTPFilesCheckEn = bEnable;

}
void CSaveConfigDlg::EnableSaveCommandExeCheck(BOOL bEnable)
{
	m_bCommandExeFilesCheckEn = bEnable;

}
void CSaveConfigDlg::EnableSaveSessionsCheck(BOOL bEnable)
{
	m_bSessionsCheckEn = bEnable;
}

void CSaveConfigDlg::EnableSaveHardwareModulesCheck(BOOL bEnable)
{
	m_bHardwareModulesCheckEn = bEnable;
}


void CSaveConfigDlg::OnBnClickedOk()
{
	CStdioFile  file;
	CString     sLine,sItem;
	int			i;

	GetDlgItemText(IDC_SC_CONFIG_FILE_PATH_COMBO,m_sPath);

	int		nCount = m_lruCombo.GetCount();
	for(i = 0; i < nCount ;i++)
	{
		m_lruCombo.GetLBText(i,sItem);
		if(sItem == m_sPath)
		{
			break;
		}
	}
	if(i == nCount)
	{
		m_lruCombo.AddString(m_sPath);
	}

	if( file.Open(m_sAppDataPath + "\\mvtSaveConfigLru.ini", CFile::modeCreate |CFile::modeWrite | CFile::typeText )  == TRUE) 
	{
		int i,nCount = m_lruCombo.GetCount();

		i = ((nCount-1) >= 15)? 15:(nCount-1);
		for(; i >= 0 ;i--)
		{
			m_lruCombo.GetLBText(i,sItem);
			file.WriteString(sItem + "\n");
		}
	}
	file.Close();

	OnOK();
}

void CSaveConfigDlg::OnBnClickedScSaveScriptsCheck()
{
	m_bScriptFiles = !m_bScriptFiles; 
}

void CSaveConfigDlg::OnBnClickedScSaveTftpFilesCheck()
{
	m_bTFTPFiles = !m_bTFTPFiles; 
}

void CSaveConfigDlg::OnBnClickedScSaveCommandExeFilesCheck()
{
	m_bCommandExeFiles = !m_bCommandExeFiles; 
}

void CSaveConfigDlg::OnBnClickedScCurrWinSessionCheck()
{
	m_bSessions = !m_bSessions; 
}

void CSaveConfigDlg::OnBnClickedScSaveHwmoduelsSettingsCheck()
{
	m_bHwModulesSettings = !m_bHwModulesSettings;
}

BOOL CSaveConfigDlg::OnInitDialog()
{
	CStdioFile  file;
	CString     sLine;

	CDialog::OnInitDialog();

	CheckDlgButton(IDC_SC_SAVE_SCRIPTS_CHECK,m_bScriptFiles);
	CheckDlgButton(IDC_SC_SAVE_TFTP_FILES_CHECK,m_bTFTPFiles);
	CheckDlgButton(IDC_SC_SAVE_COMMAND_EXE_FILES_CHECK,m_bCommandExeFiles);
	CheckDlgButton(IDC_SC_CURR_WIN_SESSION_CHECK,m_bSessions);
	CheckDlgButton(IDC_SC_SAVE_HWMODUELS_SETTINGS_CHECK,m_bHwModulesSettings);

	if(m_bScriptFilesCheckEn == FALSE)
	{
		GetDlgItem(IDC_SC_SAVE_SCRIPTS_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_SC_SAVE_SCRIPTS_CHECK,false);
	}
	if(m_bTFTPFilesCheckEn == FALSE)
	{
		GetDlgItem(IDC_SC_SAVE_TFTP_FILES_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_SC_SAVE_TFTP_FILES_CHECK,false);
	}
	if(m_bCommandExeFilesCheckEn == FALSE)
	{
		GetDlgItem(IDC_SC_SAVE_COMMAND_EXE_FILES_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_SC_SAVE_COMMAND_EXE_FILES_CHECK,false);
	}
	if(m_bSessionsCheckEn == FALSE)
	{
		GetDlgItem(IDC_SC_CURR_WIN_SESSION_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_SC_CURR_WIN_SESSION_CHECK,false);
	}
	if(m_bHardwareModulesCheckEn == FALSE)
	{
		GetDlgItem(IDC_SC_SAVE_HWMODUELS_SETTINGS_CHECK)->EnableWindow(FALSE);
		CheckDlgButton(IDC_SC_SAVE_HWMODUELS_SETTINGS_CHECK,false);
	}

	// Load LRU
	if( file.Open(m_sAppDataPath + "\\mvtSaveConfigLru.ini", CFile::modeRead | CFile::typeText )  == TRUE) 
	{
		while(file.ReadString(sLine) != FALSE)
		{
			if(sLine.IsEmpty() == FALSE)
				m_lruCombo.AddString(sLine);
		}
	}
	file.Close();

	if(m_lruCombo.GetCount() == 0)
		m_lruCombo.AddString(m_sPath + "mvtConfiguration.mvtc");

	CString sItem;
	m_lruCombo.GetLBText(0,sItem);
	SetDlgItemText(IDC_SC_CONFIG_FILE_PATH_COMBO,sItem);


	m_saveBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_browseBtn.SetTheme(xtpButtonThemeOffice2000);

	GetDlgItem(IDC_CANCEL_BTN)->SetWindowText(m_sCloseBtnText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveConfigDlg::OnCbnEditchangeScConfigFilePathCombo()
{
	
}

void CSaveConfigDlg::OnBnClickedCancel()
{
	m_nChoice = IDCANCEL;
	OnCancel();

}

void CSaveConfigDlg::OnBnClickedSaveBtn()
{
	OnBnClickedOk();
	m_nChoice = IDYES;
}

void CSaveConfigDlg::OnClose()
{
	m_nChoice = IDCLOSE; 
	CDialog::OnClose();
}


