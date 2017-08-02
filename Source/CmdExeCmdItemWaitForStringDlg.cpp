// CEWaitForStringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemWaitForStringDlg.h"


// CCmdExeCmdItemWaitForStringDlg dialog

CCmdExeCmdItemWaitForStringDlg::CCmdExeCmdItemWaitForStringDlg(CWnd* pParent /*=NULL*/)
: CBasicItemDlg(CCmdExeCmdItemWaitForStringDlg::IDD, pParent)
{
}



CCmdExeCmdItemWaitForStringDlg::~CCmdExeCmdItemWaitForStringDlg()
{
}

void CCmdExeCmdItemWaitForStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_TARGET_WIN_COMBO, m_targetWinCombo);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemWaitForStringDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCmdExeCmdItemWaitForStringDlg message handlers

void CCmdExeCmdItemWaitForStringDlg::OnBnClickedOk()
{
	CString sTimeout;

	GetDlgItemText(IDC_CE_WAIT_FOR_STR_EDIT,m_params.m_sStr);
	if(m_params.m_sStr.IsEmpty() == TRUE)
	{
		MessageBox("Please enter a valid string!",NULL,MB_ICONERROR);
		return;
	}
	GetDlgItemText(IDC_CE_WAIT_FOR_STR_TIMEOUT_EDIT,sTimeout);
	GetDlgItemText(IDC_TARGET_WIN_COMBO,m_params.m_sComPort);

	sscanf(sTimeout,"%d",&m_params.m_nTimeout);
	m_pItem->SetParams(GetParams());

	CString sDisplayName;

	sTimeout.Format("%d",m_params.m_nTimeout);
	sDisplayName = "Wait for string \"" + m_params.m_sStr + "\" with timeout=" + sTimeout + " ms";
	sDisplayName += " (" + m_params.m_sComPort + ")";
	m_pItem->SetDisplayName(sDisplayName);

	OnOK();
}

BOOL CCmdExeCmdItemWaitForStringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	SetDlgItemText(IDC_CE_WAIT_FOR_STR_EDIT,m_params.m_sStr);
	CString s;
	s.Format("%d",m_params.m_nTimeout);	
	SetDlgItemText(IDC_CE_WAIT_FOR_STR_TIMEOUT_EDIT,s);


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

void CCmdExeCmdItemWaitForStringDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemWaitForString*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemWaitForStringDlg::Clone()
{
	return new CCmdExeCmdItemWaitForStringDlg();
}


