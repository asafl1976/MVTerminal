// MVTerminalUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MVTerminalUpdateDlg.h"
#include ".\mvterminalupdatedlg.h"


// CMVTerminalUpdateDlg dialog

IMPLEMENT_DYNAMIC(CMVTerminalUpdateDlg, CDialog)
CMVTerminalUpdateDlg::CMVTerminalUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMVTerminalUpdateDlg::IDD, pParent)
{
}

CMVTerminalUpdateDlg::~CMVTerminalUpdateDlg()
{
}

void CMVTerminalUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MVT_UPDATE_HISTORY_EDIT, m_changeLogEdit);
	DDX_Control(pDX, IDOK, m_updateBtn);
	DDX_Control(pDX, IDCANCEL, m_ignoreBtn);
	DDX_Control(pDX, IDC_MVT_UPDATE_STATIC, m_newVersionMsgStatic);
	
}


BEGIN_MESSAGE_MAP(CMVTerminalUpdateDlg, CDialog)
END_MESSAGE_MAP()


// CMVTerminalUpdateDlg message handlers

BOOL CMVTerminalUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_updateBtn.SetTheme(xtpButtonThemeOffice2000);
	m_ignoreBtn.SetTheme(xtpButtonThemeOffice2000);

	SetDlgItemText(IDC_MVT_UPDATE_STATIC,m_sNewVersionMsg);
	m_changeLogEdit.SetWindowText(m_sChangeLog);
	m_newVersionMsgStatic.SetFontStyle(FS_BOLD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
