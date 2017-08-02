// SummitSMM205Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SummitSMM205HwModuleBasicDlg.h"
#include "SummitSMM205HwModule.h"


// CSummitSMM205HwModuleBasicDlg dialog

CSummitSMM205HwModuleBasicDlg::CSummitSMM205HwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
: CBasicItemDlg(nIDTemplate,pParentWnd)
{
}

CSummitSMM205HwModuleBasicDlg::~CSummitSMM205HwModuleBasicDlg()
{
}

void CSummitSMM205HwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_exeBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_SUMMIT_DLG_SETTINGS_BTN, m_settingBtn);
}


BEGIN_MESSAGE_MAP(CSummitSMM205HwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDC_SUMMIT_SMM205_CTRL_WRITE_RADIO, OnBnClickedSummitSmm205CtrlWriteRadio)
	ON_BN_CLICKED(IDC_SUMMIT_SMM205_CTRL_READ_RADIO, OnBnClickedSummitSmm205CtrlReadRadio)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CSummitSMM205HwModuleBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CSummitSMM205HwModuleBasicDlg::AlignWindow();

	m_exeBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_settingBtn.SetTheme(xtpButtonThemeOffice2000);

	CheckDlgButton(IDC_SUMMIT_SMM205_CTRL_READ_RADIO ,!m_params.m_bWrite);
	CheckDlgButton(IDC_SUMMIT_SMM205_CTRL_WRITE_RADIO, m_params.m_bWrite);

	CString sVoltage;
	sVoltage.Format("%d",m_params.m_nVoltage);
	SetDlgItemText(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT, sVoltage);

	GetDlgItem(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT)->EnableWindow(m_params.m_bWrite);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSummitSMM205HwModuleBasicDlg::OnBnClickedSummitSmm205CtrlWriteRadio()
{
	m_params.m_bWrite = TRUE;
	GetDlgItem(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT)->EnableWindow(TRUE);	
}

void CSummitSMM205HwModuleBasicDlg::OnBnClickedSummitSmm205CtrlReadRadio()
{
	m_params.m_bWrite = FALSE;
	SetDlgItemText(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT,"0");	
	GetDlgItem(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT)->EnableWindow(FALSE);	

}

void CSummitSMM205HwModuleBasicDlg::OnBnClickedOk()
{
	CString sVoltage;

	GetDlgItemText(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT, sVoltage);	
	sscanf(sVoltage,"%d",&m_params.m_nVoltage);
}

