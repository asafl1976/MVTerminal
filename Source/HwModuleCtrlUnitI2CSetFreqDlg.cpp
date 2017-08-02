// HwModuleCtrlUnitI2CSetFreqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "HwModuleCtrlUnitI2CSetFreqDlg.h"


// CHwModuleCtrlUnitI2CSetFreqDlg dialog

IMPLEMENT_DYNAMIC(CHwModuleCtrlUnitI2CSetFreqDlg, CDialog)

CHwModuleCtrlUnitI2CSetFreqDlg::CHwModuleCtrlUnitI2CSetFreqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHwModuleCtrlUnitI2CSetFreqDlg::IDD, pParent)
{
	m_nFreq = 100000;
	m_pI2CUnit = NULL;
}

CHwModuleCtrlUnitI2CSetFreqDlg::~CHwModuleCtrlUnitI2CSetFreqDlg()
{
}

void CHwModuleCtrlUnitI2CSetFreqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
}


BEGIN_MESSAGE_MAP(CHwModuleCtrlUnitI2CSetFreqDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CHwModuleCtrlUnitI2CSetFreqDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHwModuleCtrlUnitI2CSetFreqDlg message handlers

void CHwModuleCtrlUnitI2CSetFreqDlg::OnBnClickedOk()
{
	CString sFreq;

	GetDlgItemText(IDC_HW_MODULE_CTRL_UNIT_I2C_SET_FREQ_EDIT,sFreq);

	if(sFreq.IsEmpty() == TRUE)
	{
		AfxMessageBox("Invalid I2C frequency!",NULL,MB_ICONERROR);
		return;
	}
	sscanf(sFreq,"%d",&m_nFreq);

	if(m_pI2CUnit == NULL)
	{
		MessageBox("Error while setting I2C frequency!",NULL,MB_ICONERROR);
		return;
	}
	m_pI2CUnit->SetFrequency(m_nFreq);
	m_nFreq = m_pI2CUnit->GetFrequency();

	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CHwModuleCtrlUnitI2CSetFreqDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);

	if(m_pI2CUnit == NULL)
	{
		MessageBox("Error while setting I2C frequency!",NULL,MB_ICONERROR);
		return FALSE;
	}

	m_nFreq = m_pI2CUnit->GetFrequency();
	CString sFreq;
	sFreq.Format("%d",m_nFreq);
	SetDlgItemText(IDC_HW_MODULE_CTRL_UNIT_I2C_SET_FREQ_EDIT,sFreq);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHwModuleCtrlUnitI2CSetFreqDlg::SetHwModuleCtrlI2CUnit(CHwModuleCtrlI2CUnit *pUnit)
{
	m_pI2CUnit = pUnit;
}

