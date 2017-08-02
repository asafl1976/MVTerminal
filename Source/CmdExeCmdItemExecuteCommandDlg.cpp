// CEExeCmdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemExecuteCommandDlg.h"


// CCmdExeCmdItemExecuteCommandDlg dialog


CCmdExeCmdItemExecuteCommandDlg::CCmdExeCmdItemExecuteCommandDlg(CWnd* pParent /*=NULL*/)
: CBasicItemDlg(CCmdExeCmdItemExecuteCommandDlg::IDD, pParent)
{
}

CCmdExeCmdItemExecuteCommandDlg::~CCmdExeCmdItemExecuteCommandDlg()
{
}

void CCmdExeCmdItemExecuteCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_TARGET_WIN_COMBO, m_targetWinCombo);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemExecuteCommandDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCmdExeCmdItemExecuteCommandDlg message handlers

void CCmdExeCmdItemExecuteCommandDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_CE_EXECUTE_CMD_EDIT,m_params.m_sCmd);

	if(m_params.m_sCmd.IsEmpty() == TRUE)
	{
		MessageBox("Please enter a valid string!",NULL,MB_ICONERROR);
		return;
	}
	GetDlgItemText(IDC_TARGET_WIN_COMBO,m_params.m_sComPort);

	m_pItem->SetParams(GetParams());

	m_pItem->SetDisplayName("Execute command \"" + m_params.m_sCmd+ "\" (" + m_params.m_sComPort + ")");

	OnOK();
}

BOOL CCmdExeCmdItemExecuteCommandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	SetDlgItemText(IDC_CE_EXECUTE_CMD_EDIT,m_params.m_sCmd);

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

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdExeCmdItemExecuteCommandDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemExecuteCommand*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemExecuteCommandDlg::Clone()
{
	return new CCmdExeCmdItemExecuteCommandDlg();
}