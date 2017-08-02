// ODPGControlRWRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MarvellDeviceHwModuleBasicDlg.h"
#include "MarvellDeviceHwModule.h"

// CMarvellDeviceHwModuleBasicDlg dialog

CMarvellDeviceHwModuleBasicDlg::CMarvellDeviceHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CBasicItemDlg(nIDTemplate, pParentWnd)
{
	m_params.m_bRead = TRUE;
}

CMarvellDeviceHwModuleBasicDlg::~CMarvellDeviceHwModuleBasicDlg()
{
}

void CMarvellDeviceHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CMarvellDeviceHwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MARVELL_DEVICE_HWMODULE_READ_RADIO, OnBnClickedReadRadio)
	ON_BN_CLICKED(IDC_MARVELL_DEVICE_HWMODULE_WRITE_RADIO, OnBnClickedWriteRadio)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CMarvellDeviceHwModuleBasicDlg message handlers

BOOL CMarvellDeviceHwModuleBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	AlignWindow();

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
		
	CString sVal,sOffset;

	CheckDlgButton(IDC_MARVELL_DEVICE_HWMODULE_WRITE_RADIO,false);
	CheckDlgButton(IDC_MARVELL_DEVICE_HWMODULE_READ_RADIO,false);

	sVal.Format("%x",m_params.m_nValue);
	SetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT,sVal);

	sOffset.Format("%x",m_params.m_nOffset);
	SetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_OFFSET_EDIT,sOffset);

	if(m_params.m_bRead == TRUE)
	{
		CheckDlgButton(IDC_MARVELL_DEVICE_HWMODULE_READ_RADIO,true);
		GetDlgItem(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		CheckDlgButton(IDC_MARVELL_DEVICE_HWMODULE_WRITE_RADIO,true);
		GetDlgItem(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMarvellDeviceHwModuleBasicDlg::OnBnClickedOk()
{
	CString sValue, sOffset;

	GetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT, sValue);	
	sscanf(sValue,"%x",&m_params.m_nValue);

	GetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_OFFSET_EDIT, sOffset);	
	sscanf(sOffset,"%x",&m_params.m_nOffset);
}


void CMarvellDeviceHwModuleBasicDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CMarvellDeviceHwModuleBasicDlg::OnBnClickedReadRadio()
{
	GetDlgItem(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT)->EnableWindow(FALSE);
	SetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT,"");
	m_params.m_bRead = TRUE;

}

void CMarvellDeviceHwModuleBasicDlg::OnBnClickedWriteRadio()
{
	GetDlgItem(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT)->EnableWindow(TRUE);
	m_params.m_bRead = FALSE;
}
