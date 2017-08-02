// AppAppearanceSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "AppAppearanceSettingsDlg.h"
#include ".\appappearancesettingsdlg.h"


// CAppAppearanceSettingsDlg dialog

IMPLEMENT_DYNAMIC(CAppAppearanceSettingsDlg, CDialog)
CAppAppearanceSettingsDlg::CAppAppearanceSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppAppearanceSettingsDlg::IDD, pParent)
{
	//Set default theme
	SetTheme((LPCTSTR)"Smooth");
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	ImportSettingsFromFile();
}

CAppAppearanceSettingsDlg::~CAppAppearanceSettingsDlg()
{
	ExportSettingsToFile();
}

void CAppAppearanceSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THEME_LIST, m_themeList);
}


BEGIN_MESSAGE_MAP(CAppAppearanceSettingsDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_THEME_LIST, OnLbnSelchangeThemeList)
END_MESSAGE_MAP()


// CAppAppearanceSettingsDlg message handlers

void CAppAppearanceSettingsDlg::OnLbnSelchangeThemeList()
{
	int		  nIndex = m_themeList.GetCurSel();
	CString   sThemeName;

	if(nIndex == LB_ERR)
		return;
	
	m_themeList.GetText(nIndex,sThemeName);	
	SetTheme(sThemeName);
}

BOOL CAppAppearanceSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_themeList.AddString((LPCTSTR)"Smooth");
	m_themeList.AddString((LPCTSTR)"Flat");	
	m_themeList.AddString((LPCTSTR)"Flat 1");
	m_themeList.AddString((LPCTSTR)"Flat 2");
	m_themeList.AddString((LPCTSTR)"Native");

	SetTheme(m_sAppTheme);
	m_themeList.SelectString(-1,GetTheme());


	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAppAppearanceSettingsDlg::OnOk()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	if(pMainFrame == NULL)
		return;

	CXTPPaintManager::SetTheme(m_nAppTheme);
	pMainFrame->m_paneManager.SetTheme(m_nPanesTheme);
	pMainFrame->m_paneManager.GetPaintManager()->GetTabPaintManager()->SetOneNoteColors(TRUE);
	pMainFrame->m_paneManager.GetPaintManager()->GetTabPaintManager()->SetAppearance(m_nTabAppearanceStyle);

	pMainFrame->m_MTIClientWnd.GetPaintManager()->SetAppearance(m_nTabAppearanceStyle);


	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	
	pMainFrame->RecalcLayout(FALSE);
	pMainFrame->GetCommandBars()->RedrawCommandBars();
	pMainFrame->RedrawWindow();
}

void CAppAppearanceSettingsDlg::SetTheme(CString sTheme)
{
	if(sTheme == "Smooth")
	{
		m_nAppTheme	  = xtpThemeOffice2003;
		m_nPanesTheme = xtpPaneThemeOffice2003;
		m_nTabAppearanceStyle = xtpTabAppearanceVisualStudio2005;
	}
	if(sTheme == "Flat")
	{
		m_nAppTheme	  = xtpThemeOfficeXP;
    	m_nPanesTheme = xtpPaneThemeNativeWinXP;
		m_nTabAppearanceStyle = xtpTabAppearancePropertyPageFlat;

	}
	if(sTheme == "Native")
	{
		m_nAppTheme   = xtpThemeOffice2000;
    	m_nPanesTheme = xtpPaneThemeDefault;
		m_nTabAppearanceStyle = xtpTabAppearancePropertyPage;
	}
	if(sTheme == "Flat 1")
	{
		m_nAppTheme   = xtpThemeNativeWinXP;
    	m_nPanesTheme = xtpPaneThemeNativeWinXP;
		m_nTabAppearanceStyle = xtpTabAppearanceVisualStudio;
	}	
	if(sTheme == "Flat 2")
	{
		m_nAppTheme   = xtpThemeWhidbey;
    	m_nPanesTheme = xtpPaneThemeWhidbey;
		m_nTabAppearanceStyle = xtpTabAppearancePropertyPageSelected;
	}

	m_sAppTheme = sTheme;
}

BOOL CAppAppearanceSettingsDlg::ImportSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;
	int				nIndex;

	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsAppearance.ini", 
		              CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine == "[START]")
	{	
		// Zero all members
		m_sAppTheme  = "Smooth";
		//m_nAppTheme = "xxx"
		cfgFile.ReadString(sLine);
		if((nIndex = sLine.Find("=",0)) != -1)
		{
			SetTheme(sLine.Right(sLine.GetLength() - (nIndex + 1)));
		}
		else
		{
			SetTheme("Smooth");
		}

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

BOOL CAppAppearanceSettingsDlg::ExportSettingsToFile()
{
	CStdioFile  cfgFile;

	// Save settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\appGenSettingsAppearance.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		AfxMessageBox("Failed to open file "+ m_sAppDataPath +  "\\appGenSettingsAppearance.ini. Unable to save configuration!",NULL,MB_ICONERROR);
		return FALSE;
	}

	CString sLine;

	/*
	[START]
	m_nAppTheme = "xxx"
	[END]
	*/
	sLine = "[START]";
	cfgFile.WriteString(sLine + "\n");
	cfgFile.WriteString("m_sAppTheme=");
	cfgFile.WriteString(m_sAppTheme+"\n");
	cfgFile.WriteString("[END]\n");
	cfgFile.Close();
	return TRUE; 
}


