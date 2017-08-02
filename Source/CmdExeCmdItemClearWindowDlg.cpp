// CEClearWinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemClearWindowDlg.h"
#include "CmdExeCmdItemClearWindow.h"

// CCmdExeCmdItemClearWindowDlg dialog

CCmdExeCmdItemClearWindowDlg::CCmdExeCmdItemClearWindowDlg(CWnd* pParent /*=NULL*/)
	: CBasicItemDlg(CCmdExeCmdItemClearWindowDlg::IDD, pParent)
{
}

CCmdExeCmdItemClearWindowDlg::~CCmdExeCmdItemClearWindowDlg()
{
}

void CCmdExeCmdItemClearWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TARGET_WIN_COMBO, m_targetWinCombo);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemClearWindowDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCmdExeCmdItemClearWindowDlg message handlers

void CCmdExeCmdItemClearWindowDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_TARGET_WIN_COMBO,m_params.m_sComPort);
	m_pItem->SetParams(m_params);
	m_pItem->SetDisplayName("Clear window (" + m_params.m_sComPort + ")");
	OnOK();
}

BOOL CCmdExeCmdItemClearWindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CCmdExeCmdItemClearWindowDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemClearWindow*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemClearWindowDlg::Clone()
{
	return new CCmdExeCmdItemClearWindowDlg();
}


