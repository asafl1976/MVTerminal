// SummitSMM205HwModuleSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SummitSMM205HwModuleSettingsDlg.h"
#include ".\summitsmm205hwmodulesettingsdlg.h"


// CSummitSMM205HwModuleSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSummitSMM205HwModuleSettingsDlg, CDialog)
CSummitSMM205HwModuleSettingsDlg::CSummitSMM205HwModuleSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSummitSMM205HwModuleSettingsDlg::IDD, pParent)
{
}

CSummitSMM205HwModuleSettingsDlg::~CSummitSMM205HwModuleSettingsDlg()
{
}

void CSummitSMM205HwModuleSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CSummitSMM205HwModuleSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SUMMIT_CHANNEL_A_RADIO, &CSummitSMM205HwModuleSettingsDlg::OnBnClickedSummitChannelARadio)
	ON_BN_CLICKED(IDC_SUMMIT_CHANNEL_B_RADIO, &CSummitSMM205HwModuleSettingsDlg::OnBnClickedSummitChannelBRadio)
END_MESSAGE_MAP()


// CSummitSMM205HwModuleSettingsDlg message handlers

BOOL CSummitSMM205HwModuleSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	SetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_NAME_EDIT,m_sName);
	SetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_MIN_EDIT,m_sMin);
	SetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_MAX_EDIT,m_sMax);
	SetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_EXT_REF_EDIT,m_sExtRef);

	if(m_eActiveChannel == VOLTAGE_CTRL_CHANNEL_A)
	{
		CheckDlgButton(IDC_SUMMIT_CHANNEL_A_RADIO,true);
		CheckDlgButton(IDC_SUMMIT_CHANNEL_B_RADIO,false);
	}
	if(m_eActiveChannel == VOLTAGE_CTRL_CHANNEL_B)
	{
		CheckDlgButton(IDC_SUMMIT_CHANNEL_B_RADIO,true);
		CheckDlgButton(IDC_SUMMIT_CHANNEL_A_RADIO,false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSummitSMM205HwModuleSettingsDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_NAME_EDIT,m_sName);
	GetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_MIN_EDIT,m_sMin);
	GetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_MAX_EDIT,m_sMax);
	GetDlgItemText(IDC_SUMMIT_SMM205_SETTINGS_EXT_REF_EDIT,m_sExtRef);

	if(m_sMin.IsEmpty() || m_sMax.IsEmpty() || m_sExtRef.IsEmpty())
	{
		MessageBox("Invalid Min/Max/ExtRef values!",NULL,MB_ICONERROR);
		return;
	}
	
	OnOK();
}

void CSummitSMM205HwModuleSettingsDlg::OnBnClickedSummitChannelARadio()
{
	SetActiveChannel(VOLTAGE_CTRL_CHANNEL_A);
}

void CSummitSMM205HwModuleSettingsDlg::OnBnClickedSummitChannelBRadio()
{
	SetActiveChannel(VOLTAGE_CTRL_CHANNEL_B);
}
