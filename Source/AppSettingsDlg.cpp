// AppSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "AppSettingsDlg.h"
#include ".\appsettingsdlg.h"


// CAppSettingsDlg dialog

IMPLEMENT_DYNAMIC(CAppSettingsDlg, CDialog)
CAppSettingsDlg::CAppSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppSettingsDlg::IDD, pParent)
{
}

CAppSettingsDlg::~CAppSettingsDlg()
{
}

void CAppSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APP_SETTINGS_TAB, m_tabCtrl);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CAppSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_APP_SETTINGS_TAB, OnSelchangeTab)
END_MESSAGE_MAP()

// CAppSettingsDlg message handlers
// CViewPropertiesDlg message handlers
void CAppSettingsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// When the dialog is shown, display the first window
	if (bShow)
	{
		ShowWindowNumber(0);
	}
}

void CAppSettingsDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the number of the currently selected tab, and show it
	ShowWindowNumber(m_tabCtrl.GetCurFocus());

	// Do something with the "formal parameters" so the compiler is happy in warning level 4
	pNMHDR = NULL;
	pResult = NULL;
}

void CAppSettingsDlg::ShowWindowNumber(int number)
{
	// This example uses four windows
	int windowCount = 5;

	// Validate the parameter
	if ((number >= 0) && (number < windowCount))
	{
		// Create and assign pointers to each window
		CDialog *m_dPointer[5];

		m_dPointer[0] = &m_generalSettingsTab;
		m_dPointer[1] = &m_scriptSettingsTab;
		m_dPointer[2] = &m_tftpServerSettingsTab;
		m_dPointer[3] = &m_appearanceSettingsTab;
		m_dPointer[4] = &m_boardPowerCtrlSettingsTab;

		// Hide every window except for the chosen one
		for (int count = 0; count < windowCount; count++)
		{
			if (count != number)
			{
				m_dPointer[count]->ShowWindow(SW_HIDE);
			}
			else if (count == number)
			{
				// Show the chosen window and set it's location
				m_dPointer[count]->SetWindowPos(&wndTop, m_tabItemRect.left,
					m_tabItemRect.top, m_tabItemRect.right,
					m_tabItemRect.bottom, SWP_SHOWWINDOW);

				m_tabCtrl.SetCurSel(count);
			}
		}
	}
}

BOOL CAppSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect tabRect;

	m_tabCtrl.GetWindowRect(tabRect);
	// Set the size and location of the child windows based on the tab control
	m_tabItemRect.left = 13;
	m_tabItemRect.top = 35;
	m_tabItemRect.right = tabRect.Width() - 7;
	m_tabItemRect.bottom = tabRect.Height() - 38;

	// Connection settings tab
	m_generalSettingsTab.Create(CAppGeneralSettingDlg::IDD,this);	
	// BootLoader Commands settings tab
	m_scriptSettingsTab.Create(CScriptSettingsDlg::IDD, this);
	// Command history settings tab
	m_tftpServerSettingsTab.Create(CTFTPSettingsDlg::IDD, this);	
	// Send E-mail tab
	m_appearanceSettingsTab.Create(CAppAppearanceSettingsDlg::IDD, this);
	// View meesaging tab
	m_boardPowerCtrlSettingsTab.Create(CBoardPowerCtrlSettingDlg::IDD, this);

	// Set the titles for each tab
	TCITEM tabItem;
	tabItem.mask = TCIF_TEXT;

	tabItem.pszText = _T("General");
	m_tabCtrl.InsertItem(0, &tabItem);

	tabItem.pszText = _T("Scripts");
	m_tabCtrl.InsertItem(1, &tabItem);

	tabItem.pszText = _T("TFTP Server");
	m_tabCtrl.InsertItem(2, &tabItem);

	tabItem.pszText = _T("Appearance");
	m_tabCtrl.InsertItem(3, &tabItem);

	tabItem.pszText = _T("Power Control");
	m_tabCtrl.InsertItem(4, &tabItem);

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAppSettingsDlg::OnBnClickedOk()
{
	m_generalSettingsTab.OnOk();
	m_scriptSettingsTab.OnOk();
	m_appearanceSettingsTab.OnOk();
	m_tftpServerSettingsTab.OnOk();
	m_boardPowerCtrlSettingsTab.OnOk();
	OnOK();
}
