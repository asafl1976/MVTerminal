// TFTPSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "TFTPSettingsDlg.h"
#include ".\tftpsettingsdlg.h"


// CTFTPSettingsDlg dialog

IMPLEMENT_DYNAMIC(CTFTPSettingsDlg, CDialog)
CTFTPSettingsDlg::CTFTPSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTFTPSettingsDlg::IDD, pParent)
{
	// Set installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(ImportSettingsFromFile() == FALSE) 
	{		
		m_bConnectOnAppStartup = TRUE;
		m_bWarnTFTPFailure     = TRUE;
	}

}

CTFTPSettingsDlg::~CTFTPSettingsDlg()
{
	ExportSettingsToFile();
}

void CTFTPSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTFTPSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_CON_AUTO_CHECK, OnBnClickedConAutoCheck)
	ON_BN_CLICKED(IDC_TFTP_FAIL_CHECK, OnBnClickedTftpFailCheck)
END_MESSAGE_MAP()


// CTFTPSettingsDlg message handlers

void CTFTPSettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
BOOL CTFTPSettingsDlg::ImportSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;

	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsTFTPServer.ini", 
		              CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine == "[START]")
	{	
		// Zero all members
		m_bConnectOnAppStartup = FALSE;
		m_bWarnTFTPFailure     = FALSE;
				
		//m_bConnectOnAppStartup = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bConnectOnAppStartup = TRUE;
		
		//m_bWarnTFTPFailure = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bWarnTFTPFailure = TRUE;

		//[END]
		cfgFile.ReadString(sLine);
		if(sLine != "[END]")
		{
			cfgFile.Close();
			cfgFile.Remove(m_sAppDataPath +  "\\appGenSettingsTFTPServer.ini");
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
BOOL CTFTPSettingsDlg::ExportSettingsToFile()
{
	CStdioFile  cfgFile;

	// Load settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsTFTPServer.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}

	CString sLine;

	/*
	[START]
	m_bConnectOnAppStartup=TRUE
	m_bWarnTFTPFailure=TRUE
	[END]
	*/
	sLine = "[START]";
	cfgFile.WriteString(sLine + "\n");

	if(m_bConnectOnAppStartup == TRUE)
	{
		sLine = "m_bConnectOnAppStartup=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bConnectOnAppStartup=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}	

	if(m_bWarnTFTPFailure == TRUE)
	{
		sLine = "m_bWarnTFTPFailure=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bWarnTFTPFailure=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}
	cfgFile.WriteString("[END]\n");
	cfgFile.Close();
	return TRUE; 
}


BOOL CTFTPSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_CON_AUTO_CHECK,m_bConnectOnAppStartup);
	CheckDlgButton(IDC_TFTP_FAIL_CHECK,m_bWarnTFTPFailure);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTFTPSettingsDlg::OnBnClickedConAutoCheck()
{
	m_bConnectOnAppStartup = !m_bConnectOnAppStartup;
	CheckDlgButton(IDC_CON_AUTO_CHECK, m_bConnectOnAppStartup);
}
void CTFTPSettingsDlg::OnOk()
{ 

}


void CTFTPSettingsDlg::OnBnClickedTftpFailCheck()
{
	m_bWarnTFTPFailure = !m_bWarnTFTPFailure;
}
