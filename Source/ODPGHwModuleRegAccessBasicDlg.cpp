// ODPGControlRWRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ODPGHwModuleRegAccessBasicDlg.h"
#include ".\ODPGHwModuleRegAccessBasicDlg.h"
#include ".\ODPGHwModuleModelessDlg.h"
#include "odpghwmodule.h"

// CODPGHwModuleRegAccessBasicDlg dialog

CODPGHwModuleRegAccessBasicDlg::CODPGHwModuleRegAccessBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CBasicItemDlg(nIDTemplate, pParentWnd)
{
	m_params.m_bRead = TRUE;
}

CODPGHwModuleRegAccessBasicDlg::~CODPGHwModuleRegAccessBasicDlg()
{
}

void CODPGHwModuleRegAccessBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ODPG_CTRL_EXE_BTN, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CODPGHwModuleRegAccessBasicDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ODPG_CTRL_REG_READ_RADIO, OnBnClickedOdpgCtrlRegReadRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_REG_WRITE_RADIO, OnBnClickedOdpgCtrlRegWriteRadio)
END_MESSAGE_MAP()

// CODPGHwModuleRegAccessBasicDlg message handlers

BOOL CODPGHwModuleRegAccessBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	AlignWindow();

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
		
	CheckDlgButton(IDC_ODPG_CTRL_REG_WRITE_RADIO,false);
	CheckDlgButton(IDC_ODPG_CTRL_REG_READ_RADIO,false);
	SetDlgItemText(IDC_ODPG_CTRL_REG_RW_VALUE_EDIT,m_params.m_sValue);
	SetDlgItemText(IDC_ODPG_CTRL_REG_RW_OFFSET_EDIT,m_params.m_sOffset);

	if(m_params.m_bRead == TRUE)
	{
		CheckDlgButton(IDC_ODPG_CTRL_REG_READ_RADIO,true);
	}
	else
	{
		CheckDlgButton(IDC_ODPG_CTRL_REG_WRITE_RADIO,true);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CODPGHwModuleRegAccessBasicDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CODPGHwModuleRegAccessBasicDlg::OnBnClickedOdpgCtrlRegReadRadio()
{
	SetDlgItemText(IDC_ODPG_CTRL_REG_RW_VALUE_EDIT,"");
	m_params.m_bRead = TRUE;
}

void CODPGHwModuleRegAccessBasicDlg::OnBnClickedOdpgCtrlRegWriteRadio()
{
	m_params.m_bRead = FALSE;
}
