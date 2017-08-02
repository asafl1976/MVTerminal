// CEToggleIODlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "CmdExeCmdItemUnknownI2CReadWriteDlg.h"

// CCmdExeCmdItemUnknownI2CReadWriteDlg dialog
CCmdExeCmdItemUnknownI2CReadWriteDlg::CCmdExeCmdItemUnknownI2CReadWriteDlg(CWnd* pParent /*=NULL*/)
	: CUnknownI2CHwModuleBasicDlg(CCmdExeCmdItemUnknownI2CReadWriteDlg::IDD, pParent)
{
}

CCmdExeCmdItemUnknownI2CReadWriteDlg::~CCmdExeCmdItemUnknownI2CReadWriteDlg()
{
}

void CCmdExeCmdItemUnknownI2CReadWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GEN_I2C_DEV_CTRL_HEX_EDIT, m_dataHexEdit);
	DDX_Control(pDX, IDOK, m_exeBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}

BEGIN_MESSAGE_MAP(CCmdExeCmdItemUnknownI2CReadWriteDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_GEN_I2C_DEV_CTRL_HEX_EDIT, OnEnSetfocusGenI2cDevCtrlHexEdit)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_READ_RADIO, OnBnClickedGenI2cDevCtrlReadRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_WRITE_RADIO, OnBnClickedGenI2cDevCtrlWriteRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK, OnBnClickedGenI2cDevCtrlSendStopCheck)
END_MESSAGE_MAP()

BOOL CCmdExeCmdItemUnknownI2CReadWriteDlg::OnInitDialog()
{
	if(m_pItem != NULL)
	{
		m_params = m_pItem->GetParams();
	}

	CUnknownI2CHwModuleBasicDlg::OnInitDialog();

	SetWindowText(m_pItem->GetHwModuleItem()->GetDisplayName());
	SetDlgItemText(IDOK,"OK");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdExeCmdItemUnknownI2CReadWriteDlg::OnBnClickedOk()
{
	CString sDisplayName;

	CUnknownI2CHwModuleBasicDlg::OnBnClickedOk();

	if(m_params.m_nByteCount == -1)
	{
		//Invalid byte count - set to -1 if CUnknownI2CHwModuleBasicDlg find that BC is empty or invalid
		return;
	}

	if(m_params.m_bRead == TRUE)
	{
		sDisplayName.Format("%s: I2C Read Transaction (BC = %d)",m_pItem->GetHwModuleItem()->GetDisplayName(),m_params.m_nByteCount);
	}
	else
	{
		sDisplayName.Format("%s: I2C Write Transaction (BC = %d)",m_pItem->GetHwModuleItem()->GetDisplayName(),m_params.m_nByteCount);
	}
	m_pItem->SetDisplayName(sDisplayName);
	m_pItem->SetParams(m_params);

	OnOK();
}

CBasicItemDlg* CCmdExeCmdItemUnknownI2CReadWriteDlg::Clone()
{
	return new CCmdExeCmdItemUnknownI2CReadWriteDlg();
}

void CCmdExeCmdItemUnknownI2CReadWriteDlg::SetCmdItem(CCmdExeCmdItem* pItem)
{
	m_pItem = dynamic_cast<CCmdExeCmdItemUnknownI2CReadWrite*>(pItem);
}



