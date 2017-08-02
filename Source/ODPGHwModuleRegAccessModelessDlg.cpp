// ODPGCtrlRWRegModelessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ODPGHwModuleRegAccessModelessDlg.h"
#include "ODPGHwModuleModelessDlg.h"


// CODPGHwModuleRegAccessModelessDlg dialog

CODPGHwModuleRegAccessModelessDlg::CODPGHwModuleRegAccessModelessDlg(CWnd* pParent /*=NULL*/)
	: CODPGHwModuleRegAccessBasicDlg(CODPGHwModuleRegAccessModelessDlg::IDD, pParent)
{
}

CODPGHwModuleRegAccessModelessDlg::~CODPGHwModuleRegAccessModelessDlg()
{
}

void CODPGHwModuleRegAccessModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CODPGHwModuleRegAccessBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CODPGHwModuleRegAccessModelessDlg, CODPGHwModuleRegAccessBasicDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ODPG_CTRL_EXE_BTN, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ODPG_CTRL_REG_READ_RADIO, OnBnClickedOdpgCtrlRegReadRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_REG_WRITE_RADIO, OnBnClickedOdpgCtrlRegWriteRadio)
END_MESSAGE_MAP()

void CODPGHwModuleRegAccessModelessDlg::OnDestroy()
{
	((CODPGHwModuleModelessDlg*)GetParent())->InvalidateDlg();
	CODPGHwModuleRegAccessBasicDlg::OnDestroy();
}

void CODPGHwModuleRegAccessModelessDlg::PostNcDestroy()
{
	delete this;
}

void CODPGHwModuleRegAccessModelessDlg::OnOK()
{
	UINT	nRegOffset,nRegValue;
	CString sRegOffset,sRegValue;

	if(m_pODPGModule == NULL)
	{
		return;
	}
	GetDlgItemText(IDC_ODPG_CTRL_REG_RW_OFFSET_EDIT,sRegOffset);
	sscanf(sRegOffset,"%x",&nRegOffset);
	if(m_params.m_bRead == TRUE)
	{	
		if(m_pODPGModule->ReadRegister32(nRegOffset,&nRegValue) == FALSE)
		{
			MessageBox("Write register failed!",NULL,MB_ICONERROR);
			return;
		}

		sRegValue.Format("%x",nRegValue);
		SetDlgItemText(IDC_ODPG_CTRL_REG_RW_VALUE_EDIT,sRegValue);
	}
	else
	{
		GetDlgItemText(IDC_ODPG_CTRL_REG_RW_VALUE_EDIT,sRegValue);
		sscanf(sRegValue,"%x",&nRegValue);
		if(m_pODPGModule->WriteRegister32(nRegOffset,nRegValue) == FALSE)
		{
			MessageBox("Write register failed!",NULL,MB_ICONERROR);
			return;
		}
	}
}

void CODPGHwModuleRegAccessModelessDlg::OnCancel()
{
	DestroyWindow();
}


// CODPGHwModuleModelessDlg message handlers

BOOL CODPGHwModuleRegAccessModelessDlg::OnInitDialog()
{
	CODPGHwModuleRegAccessBasicDlg::OnInitDialog();
	AlignWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void  CODPGHwModuleRegAccessModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pODPGModule = dynamic_cast<CODPGHwModule*>(pItem);
}

BOOL  CODPGHwModuleRegAccessModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CODPGHwModuleRegAccessModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CODPGHwModuleRegAccessModelessDlg::Clone()
{
	return new CODPGHwModuleRegAccessModelessDlg();
}