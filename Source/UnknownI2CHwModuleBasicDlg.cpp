// UnknownI2CDevCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "UnknownI2CHwModuleBasicDlg.h"



// CUnknownI2CHwModuleBasicDlg dialog

CUnknownI2CHwModuleBasicDlg::CUnknownI2CHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
: CBasicItemDlg(nIDTemplate,pParentWnd)
{

}

CUnknownI2CHwModuleBasicDlg::~CUnknownI2CHwModuleBasicDlg()
{
}

void CUnknownI2CHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GEN_I2C_DEV_CTRL_HEX_EDIT, m_dataHexEdit);
	DDX_Control(pDX, IDOK, m_exeBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
}


BEGIN_MESSAGE_MAP(CUnknownI2CHwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_GEN_I2C_DEV_CTRL_HEX_EDIT, OnEnSetfocusGenI2cDevCtrlHexEdit)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_READ_RADIO, OnBnClickedGenI2cDevCtrlReadRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_WRITE_RADIO, OnBnClickedGenI2cDevCtrlWriteRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK, OnBnClickedGenI2cDevCtrlSendStopCheck)
END_MESSAGE_MAP()


// CUnknownI2CHwModuleBasicDlg message handlers

void CUnknownI2CHwModuleBasicDlg::OnBnClickedOk()
{
	CString sCount;

	if(m_params.m_bRead == TRUE)
	{
		GetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,sCount);
		if((sCount.IsEmpty() == TRUE) || (sCount == "0"))
		{
			MessageBox("Invalid byte count value!",NULL,MB_ICONERROR);
			m_params.m_nByteCount = -1;
			return;
		}
		sscanf(sCount,"%d",&m_params.m_nByteCount);
	}
	else //write
	{
		m_params.m_nByteCount = m_dataHexEdit.GetData(m_params.m_data,64);
		if(m_params.m_nByteCount == 0)
		{
			MessageBox("Invalid byte count value - enter data in data window!",NULL,MB_ICONERROR);
			m_params.m_nByteCount = -1;
			return;
		}
		// Set end of data
		m_params.m_data[m_params.m_nByteCount] = NULL;
	}	
}

void CUnknownI2CHwModuleBasicDlg::OnBnClickedCancel()
{
	
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL CUnknownI2CHwModuleBasicDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	CUnknownI2CHwModuleBasicDlg::AlignWindow();

	m_exeBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);

	GetDlgItem(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK)->EnableWindow(m_params.m_bRead);

	CheckDlgButton(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK, m_params.m_bStopBit);
	CheckDlgButton(IDC_GEN_I2C_DEV_CTRL_READ_RADIO, m_params.m_bRead);
	CheckDlgButton(IDC_GEN_I2C_DEV_CTRL_WRITE_RADIO, !m_params.m_bRead);

	m_dataHexEdit.EnableWindow(FALSE);

	if(m_params.m_bRead == FALSE)
	{
		m_dataHexEdit.EnableWindow(TRUE);
		SetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,"1");
		GetDlgItem(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT)->EnableWindow(m_params.m_bRead);
	}

	const int len = (int)strlen((char *)m_params.m_data);	
	m_dataHexEdit.SetData(m_params.m_data, len, 64);
	m_dataHexEdit.SetBPR(8);
	m_dataHexEdit.SetOptions(FALSE, TRUE, FALSE, FALSE);
	//m_dataHexEdit.SetAddressBase(0);

	CString sBC;
	sBC.Format("%d",m_params.m_nByteCount);
	SetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,sBC);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CUnknownI2CHwModuleBasicDlg::ResetData()
{
	m_dataHexEdit.SetData((LPBYTE)"", 0, 64);
	m_dataHexEdit.RedrawWindow();
	m_params.m_data[0]	= NULL;
}

void CUnknownI2CHwModuleBasicDlg::OnEnSetfocusGenI2cDevCtrlHexEdit()
{
	m_dataHexEdit.RedrawWindow();
}

void CUnknownI2CHwModuleBasicDlg::OnBnClickedGenI2cDevCtrlReadRadio()
{
	m_params.m_bRead = TRUE;

	//SetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,"0");
	GetDlgItem(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT)->EnableWindow(m_params.m_bRead);

	CheckDlgButton(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK,TRUE);
	GetDlgItem(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK)->EnableWindow(TRUE);

	m_dataHexEdit.EnableWindow(FALSE);

	ResetData();
}

void CUnknownI2CHwModuleBasicDlg::OnBnClickedGenI2cDevCtrlWriteRadio()
{
	m_params.m_bRead = FALSE;

	//SetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,"0");
	GetDlgItem(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT)->EnableWindow(m_params.m_bRead);
	
	CheckDlgButton(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK,TRUE);
	GetDlgItem(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK)->EnableWindow(FALSE);
	m_dataHexEdit.EnableWindow(TRUE);
	ResetData();
}

void CUnknownI2CHwModuleBasicDlg::OnBnClickedGenI2cDevCtrlSendStopCheck()
{
	m_params.m_bStopBit = !m_params.m_bStopBit;
}
