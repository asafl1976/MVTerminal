// CEToggleComConnectionCmdItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemToggleComConnectionDlg.h"

// CCmdExeCmdItemToggleComConnectionDlg dialog

CCmdExeCmdItemToggleComConnectionDlg::CCmdExeCmdItemToggleComConnectionDlg(CWnd* pParent /*=NULL*/)
: CBasicItemDlg(CCmdExeCmdItemToggleComConnectionDlg::IDD, pParent)
{
}


CCmdExeCmdItemToggleComConnectionDlg::~CCmdExeCmdItemToggleComConnectionDlg()
{
}

void CCmdExeCmdItemToggleComConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_TARGET_WIN_COMBO, m_targetWinCombo);
	DDX_Control(pDX, IDC_CE_COM_TOGGLE_DELAY_EDIT, m_delayEditCtrl);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemToggleComConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CE_COM_TOGGLE_DIS_CHECK, OnBnClickedCeComToggleDisCheck)
	ON_BN_CLICKED(IDC_CE_COM_TOGGLE_CON_CHECK, OnBnClickedCeComToggleConCheck)
END_MESSAGE_MAP()


// CCmdExeCmdItemToggleComConnectionDlg message handlers


BOOL CCmdExeCmdItemToggleComConnectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);


	/* Intialize com ports combo*/
	vector<CString>           comPortVec;
	vector<CString>::iterator iter,endIter;
	((CMVTerminalApp*)AfxGetApp())->EnumerateSerialPorts(comPortVec);
	iter    = comPortVec.begin();
	endIter = comPortVec.end();
	for(; iter != endIter; iter++)
		m_targetWinCombo.AddString(*iter);

	if(	m_targetWinCombo.SelectString(-1,m_params.m_sComPort) == CB_ERR)
	{
		CMVTerminalView *p = ((CMVTerminalApp*)AfxGetApp())->GetActiveView();
		if(p != NULL)
		{
			// Select current open window
			m_params.m_sComPort = p->GetConnectionParams().GetComPort();
		}
		m_targetWinCombo.SelectString(-1,m_params.m_sComPort);
	}

	CheckDlgButton(IDC_CE_COM_TOGGLE_CON_CHECK,m_params.m_bConnect);
	CheckDlgButton(IDC_CE_COM_TOGGLE_DIS_CHECK,m_params.m_bDisconnect);
	SetDlgItemText(IDC_CE_COM_TOGGLE_DELAY_EDIT,m_params.m_sDelay);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdExeCmdItemToggleComConnectionDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_TARGET_WIN_COMBO,m_params.m_sComPort);
	GetDlgItemText(IDC_CE_COM_TOGGLE_DELAY_EDIT,m_params.m_sDelay);
	m_pItem->SetParams(GetParams());

	m_pItem->SetDisplayName("Toggle COM port connection (" + m_params.m_sComPort + ")");

	OnOK();
}

void CCmdExeCmdItemToggleComConnectionDlg::OnBnClickedCeComToggleDisCheck()
{
	m_params.m_bDisconnect = !m_params.m_bDisconnect;
}

void CCmdExeCmdItemToggleComConnectionDlg::OnBnClickedCeComToggleConCheck()
{
	m_params.m_bConnect = !m_params.m_bConnect;
}

void CCmdExeCmdItemToggleComConnectionDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemToggleComConnection*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemToggleComConnectionDlg::Clone()
{
	return new CCmdExeCmdItemToggleComConnectionDlg();
}
