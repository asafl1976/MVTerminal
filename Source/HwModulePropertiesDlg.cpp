// HwModulePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "HwModulePropertiesDlg.h"
#include ".\hwmodulepropertiesdlg.h"


// CHwModulePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CHwModulePropertiesDlg, CDialog)
CHwModulePropertiesDlg::CHwModulePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHwModulePropertiesDlg::IDD, pParent)
{
}

CHwModulePropertiesDlg::~CHwModulePropertiesDlg()
{
}

void CHwModulePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHwModulePropertiesDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CHwModulePropertiesDlg message handlers

void CHwModulePropertiesDlg::OnBnClickedOk()
{
	CString sDisplayName;

	GetDlgItemText(IDC_HW_MODULE_PROP_DIS_NAME,sDisplayName);
	if(sDisplayName.IsEmpty() == TRUE)
	{
		MessageBox("Invalid display name!",NULL,MB_ICONERROR);
		return;
	}
	m_sDisplayName = sDisplayName;
	OnOK();
}

BOOL CHwModulePropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_sName + " Properties");
	SetDlgItemText(IDC_HW_MODULE_PROP_DIS_NAME,m_sDisplayName);
	SetDlgItemText(IDC_HW_MODULE_PROP_NAME,m_sName);
	SetDlgItemText(IDC_HW_MODULE_PROP_IF_NAME,m_sInterfaceName);
	SetDlgItemText(IDC_HW_MODULE_PROP_ID,m_sID);
	if(m_sInterfaceName == "I2C")
	{
		SetDlgItemText(IDC_ID_STATIC,"I2C Address:");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
