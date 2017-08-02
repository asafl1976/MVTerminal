// CESetDelayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemDelayDlg.h"


// CCmdExeCmdItemDelayDlg dialog

CCmdExeCmdItemDelayDlg::CCmdExeCmdItemDelayDlg(CWnd* pParent /*=NULL*/)
: CBasicItemDlg(CCmdExeCmdItemDelayDlg::IDD, pParent)
{
}

CCmdExeCmdItemDelayDlg::~CCmdExeCmdItemDelayDlg()
{
}

void CCmdExeCmdItemDelayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CCmdExeCmdItemDelayDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CE_SET_DELAY_FIXED_RADIO, OnBnClickedCeSetDelayFixedRadio)
	ON_BN_CLICKED(IDC_CE_SET_DELAY_RAND_RADIO, OnBnClickedCeSetDelayRandRadio)
END_MESSAGE_MAP()


// CCmdExeCmdItemDelayDlg message handlers

void CCmdExeCmdItemDelayDlg::OnBnClickedOk()
{
	CString s;
	
	switch(m_params.m_type)
	{
	case CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY:
		GetDlgItemText(IDC_CE_SET_DELAY_FIXED_EDIT,s);
		if(s.IsEmpty())
			s = "0";
		sscanf(s,"%d",&m_params.m_nVal);
		break;
	case CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY:
		GetDlgItemText(IDC_CE_SET_DELAY_RAND_LOW_EDIT,s);
		if(s.IsEmpty())
			s = "0";
		sscanf(s,"%d",&m_params.m_nVal);

		GetDlgItemText(IDC_CE_SET_DELAY_RAND_HIGH_EDIT,s);
		if(s.IsEmpty())
			s = "0";
		sscanf(s,"%d",&m_params.m_nHighVal);
		break;
	}
	m_pItem->SetParams(GetParams());

	CString sDisplayName;
	switch(m_params.m_type)
	{
	case CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY:
		sDisplayName.Format("Delay = %d (ms)",m_params.m_nVal); 
		break;
	case CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY:
		sDisplayName.Format("Delay = Rand[%d:%d] (ms)",m_params.m_nVal,m_params.m_nHighVal); 
		break;
	default:
		break;
	}
	m_pItem->SetDisplayName(sDisplayName);

	OnOK();
}

void CCmdExeCmdItemDelayDlg::OnBnClickedCeSetDelayFixedRadio()
{
	m_params.m_type  = CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY;
	CheckDlgButton(IDC_CE_SET_DELAY_FIXED_RADIO,TRUE);
	CheckDlgButton(IDC_CE_SET_DELAY_RAND_RADIO,FALSE);

	GetDlgItem(IDC_CE_SET_DELAY_FIXED_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CE_SET_DELAY_RAND_LOW_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CE_SET_DELAY_RAND_HIGH_EDIT)->EnableWindow(FALSE);
}

void CCmdExeCmdItemDelayDlg::OnBnClickedCeSetDelayRandRadio()
{
	m_params.m_type = CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY;
	CheckDlgButton(IDC_CE_SET_DELAY_FIXED_RADIO,FALSE);
	CheckDlgButton(IDC_CE_SET_DELAY_RAND_RADIO,TRUE);
	
	GetDlgItem(IDC_CE_SET_DELAY_FIXED_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CE_SET_DELAY_RAND_LOW_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CE_SET_DELAY_RAND_HIGH_EDIT)->EnableWindow(TRUE);

}

BOOL CCmdExeCmdItemDelayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	
	CString s;

	switch(m_params.m_type)
	{
	case CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY:
		OnBnClickedCeSetDelayFixedRadio();
		s.Format("%d",m_params.m_nVal);
		SetDlgItemText(IDC_CE_SET_DELAY_FIXED_EDIT,s);
		break;
	case CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY:
		OnBnClickedCeSetDelayRandRadio();
		s.Format("%d",m_params.m_nVal);
		SetDlgItemText(IDC_CE_SET_DELAY_RAND_LOW_EDIT,s);
		s.Format("%d",m_params.m_nHighVal);
		SetDlgItemText(IDC_CE_SET_DELAY_RAND_HIGH_EDIT,s);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCmdExeCmdItemDelayDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemDelay*>(pItem);
}


CBasicItemDlg *CCmdExeCmdItemDelayDlg::Clone()
{
	return new CCmdExeCmdItemDelayDlg();
}

