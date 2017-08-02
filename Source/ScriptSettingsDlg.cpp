// ScriptSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ScriptSettingsDlg.h"


// CScriptSettingsDlg dialog

IMPLEMENT_DYNAMIC(CScriptSettingsDlg, CDialog)
CScriptSettingsDlg::CScriptSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptSettingsDlg::IDD, pParent)
{
	// Set installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(ImportSettingsFromFile() == FALSE) 
	{		
		m_bLoadDefaultScripts = TRUE;
		m_sDefaultScriptPath  = m_sAppDataPath + "\\Default Scripts";
	}
}


CScriptSettingsDlg::~CScriptSettingsDlg()
{
	ExportSettingsToFile();
}

void CScriptSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSE_BTN, m_browseBtn);
	DDX_Control(pDX, IDC_DEFAULT_SCRIPTS_PATH_EDIT, m_defaultScriptPathedit);
}


BEGIN_MESSAGE_MAP(CScriptSettingsDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_BTN, OnBnClickedBrowseBtn)
	ON_BN_CLICKED(IDC_LOAD_DEFAULT_SCRIPTS_CHECK, OnBnClickedLoadDefaultScriptsCheck)
END_MESSAGE_MAP()


// CScriptSettingsDlg message handlers

void CScriptSettingsDlg::OnOk()
{
	GetDlgItemText(IDC_DEFAULT_SCRIPTS_PATH_EDIT, m_sDefaultScriptPath);

	// Fix path that ends with '\'
	if(m_sDefaultScriptPath.GetAt(m_sDefaultScriptPath.GetLength() - 1) == '\\')
		m_sDefaultScriptPath = m_sDefaultScriptPath.Left(m_sDefaultScriptPath.GetLength() - 1);	
}

void CScriptSettingsDlg::OnBnClickedBrowseBtn()
{
	CPathDialog dlg(_T("Select Folder"), _T("Select folder to load scripts from"), _T("C:\\"));

	if (dlg.DoModal()==IDOK)
	{
		m_sDefaultScriptPath = dlg.GetPathName();
		SetDlgItemText(IDC_DEFAULT_SCRIPTS_PATH_EDIT, m_sDefaultScriptPath);
	}

}

void CScriptSettingsDlg::OnBnClickedLoadDefaultScriptsCheck()
{
	m_bLoadDefaultScripts = !m_bLoadDefaultScripts;
	CheckDlgButton(IDC_LOAD_DEFAULT_SCRIPTS_CHECK, m_bLoadDefaultScripts);

	m_defaultScriptPathedit.EnableWindow(m_bLoadDefaultScripts);
	m_browseBtn.EnableWindow(m_bLoadDefaultScripts);
}


BOOL CScriptSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_browseBtn.SetTheme(xtpButtonThemeOffice2000);
	
	CheckDlgButton(IDC_LOAD_DEFAULT_SCRIPTS_CHECK, m_bLoadDefaultScripts);
	SetDlgItemText(IDC_DEFAULT_SCRIPTS_PATH_EDIT, m_sDefaultScriptPath);

	//En/Dis according to m_bLoadDefaultScripts state
	m_defaultScriptPathedit.EnableWindow(m_bLoadDefaultScripts);
	m_browseBtn.EnableWindow(m_bLoadDefaultScripts);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CScriptSettingsDlg::ImportSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;

	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsScripts.ini", 
		              CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine == "[START]")
	{	
		// Zero all members
		m_bLoadDefaultScripts = FALSE;
				
		//m_bLoadDefaultScripts = XXX
		cfgFile.ReadString(sLine);
		if(sLine.Find("=TRUE",0) != -1)
			m_bLoadDefaultScripts = TRUE;

		cfgFile.ReadString(sLine);
		//m_sDefaultScriptPath 
		sLine = sLine.Right(sLine.GetLength() - sLine.ReverseFind('=') - 1);
        m_sDefaultScriptPath = sLine;

		//[END]
		cfgFile.ReadString(sLine);
		if(sLine != "[END]")
		{
			cfgFile.Close();
			return FALSE;
		}

		m_defaultScriptPathedit.EnableWindow(m_bLoadDefaultScripts);
		m_browseBtn.EnableWindow(m_bLoadDefaultScripts);
		cfgFile.Close();
		return TRUE;
	}
	else
	{
		cfgFile.Close();
		return FALSE;
	}
}
BOOL CScriptSettingsDlg::ExportSettingsToFile()
{
	CStdioFile  cfgFile;

	// Load settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsScripts.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}

	CString sLine;

	/*
	[START]
	m_bLoadDefaultScripts=TRUE
	m_sDefaultScriptPath=
	[END]
	*/
	sLine = "[START]";
	cfgFile.WriteString(sLine + "\n");

	if(m_bLoadDefaultScripts == TRUE)
	{
		sLine = "m_bLoadDefaultScripts=TRUE";
		cfgFile.WriteString(sLine + "\n");
	}
	else
	{
		sLine = "m_bLoadDefaultScripts=FALSE";
		cfgFile.WriteString(sLine + "\n");
	}
	
	sLine = "m_sDefaultScriptPath=" + m_sDefaultScriptPath;
	cfgFile.WriteString(sLine + "\n");

	cfgFile.WriteString("[END]\n");

	cfgFile.Close();
	return TRUE; 
}
