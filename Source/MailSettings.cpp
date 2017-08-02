// MailSettings.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MailSettings.h"
#include ".\mailsettings.h"


// CMailSettings dialog

IMPLEMENT_DYNAMIC(CMailSettings, CDialog)
CMailSettings::CMailSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSettings::IDD, pParent)
{
	m_sSender     = "MVTerminal@marvell.com";
	m_sMode       = "MIME";
	m_sMailServer = "msilexch01.marvell.com";
}

CMailSettings::~CMailSettings()
{
}

void CMailSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
}


BEGIN_MESSAGE_MAP(CMailSettings, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void      CMailSettings::SetSender(CString sSender)
{
	m_sSender = sSender;
}
void      CMailSettings::SetMode(CString sMode)
{
	m_sMode = sMode;
}
void      CMailSettings::SetMailServer(CString sMailServer)
{
	m_sMailServer = sMailServer;
}

CString   CMailSettings::GetSender()
{
	return m_sSender;
}

CString   CMailSettings::GetMode()
{
	return m_sMode;
}

CString   CMailSettings::GetMailServer()
{
	return m_sMailServer;
}

BOOL CMailSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	SetDlgItemText(IDC_SENDER,m_sSender);
	SetDlgItemText(IDC_MAILSERVER,m_sMailServer);
	((CComboBox*)GetDlgItem(IDC_MODE))->SelectString(-1,m_sMode);

	m_OkBtn.SetTheme(xtpButtonThemeOffice2000);
	m_CancelBtn.SetTheme(xtpButtonThemeOffice2000);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void CMailSettings::OnBnClickedOk()
{
	GetDlgItemText(IDC_SENDER,m_sSender);
	if((m_sSender.Find('@') == -1) || (m_sSender.Find('.') == -1))
	{
        MessageBox("Invalid sender address!",NULL,MB_ICONERROR);
		return;
	}
	GetDlgItemText(IDC_MAILSERVER,m_sMailServer);
	if(m_sMailServer.Find('.') == -1)
	{
        MessageBox("Invalid mail server address!",NULL,MB_ICONERROR);
		return;
	}

	GetDlgItemText(IDC_MODE,m_sMode);
	OnOK();
}
