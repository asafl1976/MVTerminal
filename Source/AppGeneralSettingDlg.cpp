// AppGeneralSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "AppGeneralSettingDlg.h"
#include ".\appgeneralsettingdlg.h"


// CAppGeneralSettingDlg dialog

IMPLEMENT_DYNAMIC(CAppGeneralSettingDlg, CDialog)
CAppGeneralSettingDlg::CAppGeneralSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppGeneralSettingDlg::IDD, pParent)
{

	// Set installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(ImportSettingsFromFile() == FALSE) 
	{		
		m_bPromptToSaveSettings = TRUE;
		m_bCheckForUpdates  = TRUE;
		m_bEnableIPMMessagesOnStartup = FALSE;
	}


}

CAppGeneralSettingDlg::~CAppGeneralSettingDlg()
{
	ExportSettingsToFile();
}

void CAppGeneralSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CAppGeneralSettingDlg::OnOk()
{

}


BEGIN_MESSAGE_MAP(CAppGeneralSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_PROMPT_TO_SAVE_ENTIRE_SETTINGS_CHECK, OnBnClickedPromptToSaveEntireSettingsCheck)
	ON_BN_CLICKED(IDC_CHECK_FOR_NEW_UPDATES, OnBnClickedCheckForNewUpdates)
	ON_BN_CLICKED(IDC_ENABLE_IPM_MSG_ON_STARTUP, &CAppGeneralSettingDlg::OnBnClickedEnableIpmMsgOnStartup)
END_MESSAGE_MAP()


// CAppGeneralSettingDlg message handlers



void CAppGeneralSettingDlg::OnBnClickedPromptToSaveEntireSettingsCheck()
{
	m_bPromptToSaveSettings = !m_bPromptToSaveSettings;
}


BOOL CAppGeneralSettingDlg::ImportSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;

	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsGeneral.ini", 
		              CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine == "[START]")
	{	
		// Zero all members
		m_bPromptToSaveSettings = FALSE;
		m_bCheckForUpdates      = FALSE;
		m_bEnableIPMMessagesOnStartup = FALSE;

		//m_bPromptToSaveSettings = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bPromptToSaveSettings = TRUE;

		//m_bCheckForUpdates = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bCheckForUpdates = TRUE;
		
		//m_bEnableIPMMessagesOnStartup = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bEnableIPMMessagesOnStartup = TRUE;
		
		//[END]
		cfgFile.ReadString(sLine);
		if(sLine != "[END]")
		{
			cfgFile.Close();
			return FALSE;
		}
		cfgFile.Close();
		return TRUE;
	}
	else
	{
		cfgFile.Close();
		return FALSE;
	}
}
BOOL CAppGeneralSettingDlg::ExportSettingsToFile()
{
	CStdioFile  cfgFile;

	// Load settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsGeneral.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}


	CString sLine;


	/*
	[START]
	m_bPromptToSaveSettings=
	m_bPromptToSaveSession=
	m_bCheckForUpdates=
	m_bEnableIPMMessagesOnStartup=
	[END]
	*/
	sLine = "[START]";
	cfgFile.WriteString(sLine + "\n");

	if(m_bPromptToSaveSettings == TRUE)
	{
		sLine = "m_bPromptToSaveSettings=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bPromptToSaveSettings=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}
	
	if(m_bCheckForUpdates == TRUE)
	{
		sLine = "m_bCheckForUpdates=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bCheckForUpdates=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}
	
	if(m_bEnableIPMMessagesOnStartup == TRUE)
	{
		sLine = "m_bEnableIPMMessagesOnStartup=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bEnableIPMMessagesOnStartup=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}	

	cfgFile.WriteString("[END]\n");

	cfgFile.Close();
	return TRUE; 
}

BOOL CAppGeneralSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_PROMPT_TO_SAVE_ENTIRE_SETTINGS_CHECK, m_bPromptToSaveSettings);
	CheckDlgButton(IDC_CHECK_FOR_NEW_UPDATES, m_bCheckForUpdates);
	CheckDlgButton(IDC_ENABLE_IPM_MSG_ON_STARTUP, m_bEnableIPMMessagesOnStartup);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CAppGeneralSettingDlg::OnBnClickedCheckForNewUpdates()
{
	m_bCheckForUpdates = !m_bCheckForUpdates;
}

void CAppGeneralSettingDlg::OnBnClickedEnableIpmMsgOnStartup()
{
	m_bEnableIPMMessagesOnStartup = !m_bEnableIPMMessagesOnStartup;
}
