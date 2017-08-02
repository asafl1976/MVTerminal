// ViewPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ViewPropertiesDlg.h"
#include ".\viewpropertiesdlg.h"
#include "Markup.h"

// CViewPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CViewPropertiesDlg, CDialog)
CViewPropertiesDlg::CViewPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewPropertiesDlg::IDD, pParent)
{
	m_nDlgType = NEW_SESSION_DLG_TYPE;

}

CViewPropertiesDlg::~CViewPropertiesDlg()
{
}

void CViewPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VIEW_TAB, m_tabCtrl);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CViewPropertiesDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_VIEW_TAB, OnSelchangeTab)
END_MESSAGE_MAP()


// CViewPropertiesDlg message handlers
void CViewPropertiesDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// When the dialog is shown, display the first window
	if (bShow)
	{
		ShowWindowNumber(0);
	}
}

void CViewPropertiesDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the number of the currently selected tab, and show it
	ShowWindowNumber(m_tabCtrl.GetCurFocus());

	// Do something with the "formal parameters" so the compiler is happy in warning level 4
	pNMHDR = NULL;
	pResult = NULL;
}

void CViewPropertiesDlg::ShowWindowNumber(int number)
{
	// This example uses four windows
	int windowCount = 5;

	// Validate the parameter
	if ((number >= 0) && (number < windowCount))
	{
		// Create and assign pointers to each window
		CDialog *m_dPointer[5];

		m_dPointer[0] = &m_connectionSettingsTab;
		m_dPointer[1] = &m_bootLoaderParamsSettingsTab;
		m_dPointer[2] = &m_cmdHistorySettingsTab;
		m_dPointer[3] = &m_sendMailTab;
		m_dPointer[4] = &m_viewMessagingTab;

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

BOOL CViewPropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect tabRect;

	m_tabCtrl.GetWindowRect(tabRect);
	// Set the size and location of the child windows based on the tab control
	m_tabItemRect.left = 15;
	m_tabItemRect.top = 35;
	m_tabItemRect.right = tabRect.Width() - 7;
	m_tabItemRect.bottom = tabRect.Height() - 38;

	// Connection settings tab
	m_connectionSettingsTab.Create(CConnectionSettingsDlg::IDD,this);	
	// BootLoader Commands settings tab
	m_bootLoaderParamsSettingsTab.Create(CBootLoaderParamsSettingsDlg::IDD, this);
	// Command history settings tab
	m_cmdHistorySettingsTab.Create(CCmdHistorySettingsDlg::IDD, this);	
	// Send E-mail tab
	m_sendMailTab.Create(CSendMailDlg::IDD, this);
	
	// Set enable messaging user settings - before create
	BOOL bEnMessages = ((CMVTerminalApp*)AfxGetApp())->GetAppSettingsDlg().GetGeneralSettingTab().GetEnableIPMMessagesOnStartup();
	m_viewMessagingTab.SetMessagingEn(bEnMessages);

	// View meesaging tab
	m_viewMessagingTab.Create(CViewMessagingDlg::IDD, this);

	// Set the titles for each tab
	TCITEM tabItem;
	tabItem.mask = TCIF_TEXT;

	tabItem.pszText = _T("Connection");
	m_tabCtrl.InsertItem(0, &tabItem);

	tabItem.pszText = _T("Boot Loader");
	m_tabCtrl.InsertItem(1, &tabItem);

	tabItem.pszText = _T("History");
	m_tabCtrl.InsertItem(2, &tabItem);

	tabItem.pszText = _T("Send E-Mail");
	m_tabCtrl.InsertItem(3, &tabItem);

	tabItem.pszText = _T("Messaging");
	m_tabCtrl.InsertItem(4, &tabItem);

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CViewPropertiesDlg::OnBnClickedOk()
{		
	m_connectionSettingsTab.OnOK();
	m_bootLoaderParamsSettingsTab.OnOK();
	m_cmdHistorySettingsTab.OnOK();
	m_sendMailTab.OnOK();
	m_viewMessagingTab.OnOK();

	//Save params
	m_viewPropertiesParams.m_connectionParams     = m_connectionSettingsTab.GetConnectionParams();
	m_viewPropertiesParams.m_bootLoaderParams     = m_bootLoaderParamsSettingsTab.GetBootLoaderParams();
	m_viewPropertiesParams.m_cmdHistoryParams     = m_cmdHistorySettingsTab.GetCmdHistoryParams();
	m_viewPropertiesParams.m_sendMailParams       = m_sendMailTab.GetMailParams();
	m_viewPropertiesParams.m_viewMessagingParams  = m_viewMessagingTab.GetViewMessagingParams();

	if(m_connectionSettingsTab.IsPortAlreadyOpen() == FALSE)
	{
		OnOK();	
	}
}

void CViewPropertiesDlg::SetDlgType(int nType)
{
	m_nDlgType = nType;
	m_connectionSettingsTab.SetDlgType(nType);
	m_bootLoaderParamsSettingsTab.SetDlgType(nType);
}


CString CViewPropertiesDlg::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddSubDoc( m_connectionSettingsTab.Serialize() );
	xmlWriter.AddSubDoc( m_bootLoaderParamsSettingsTab.Serialize() );
	xmlWriter.AddSubDoc( m_cmdHistorySettingsTab.Serialize() );
	xmlWriter.AddSubDoc( m_sendMailTab.Serialize() );
	xmlWriter.AddSubDoc( m_viewMessagingTab.Serialize() );

	return xmlWriter.GetDoc(); 
}

BOOL CViewPropertiesDlg::DeSerialize(CMarkup& xmlReader)
{
	if(m_connectionSettingsTab.DeSerialize(xmlReader) == false)
		return FALSE;
	if(m_bootLoaderParamsSettingsTab.DeSerialize(xmlReader) == false)
		return FALSE;
	if(m_cmdHistorySettingsTab.DeSerialize(xmlReader) == false)
		return FALSE;
	if(m_sendMailTab.DeSerialize(xmlReader) == false)
		return FALSE;
	if(m_viewMessagingTab.DeSerialize(xmlReader) == false)
		return FALSE;

	//UpdateWindowData();

	return TRUE;
}

void CViewPropertiesDlg::UpdateWindowData()
{
	m_connectionSettingsTab.UpdateWindowData();
	m_bootLoaderParamsSettingsTab.UpdateWindowData();
	m_cmdHistorySettingsTab.UpdateWindowData();
	m_sendMailTab.UpdateWindowData();
	m_viewMessagingTab.UpdateWindowData();
}
