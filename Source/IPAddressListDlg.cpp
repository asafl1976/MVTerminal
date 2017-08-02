// IPAddressListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "IPAddressListDlg.h"
#include ".\ipaddresslistdlg.h"


// CIPAddressListDlg dialog

IMPLEMENT_DYNAMIC(CIPAddressListDlg, CDialog)
CIPAddressListDlg::CIPAddressListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPAddressListDlg::IDD, pParent)
{
}

CIPAddressListDlg::~CIPAddressListDlg()
{
}

void CIPAddressListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ADDRESS_LIST, m_ipListCtrl);
	DDX_Control(pDX, IDOK, m_okBtn);
}


BEGIN_MESSAGE_MAP(CIPAddressListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_IP_ADDRESS_LIST, OnLbnDblclkIpAddressList)
END_MESSAGE_MAP()


// CIPAddressListDlg message handlers

void CIPAddressListDlg::OnBnClickedOk()
{
	int			nIndex = m_ipListCtrl.GetCurSel();

	if(nIndex == LB_ERR)
	{
		MessageBox("Please select IP address!",NULL,MB_ICONERROR);
		return;
	}
	m_ipListCtrl.GetText(nIndex,m_sSelection);

	OnOK();
}
void CIPAddressListDlg::AddIPAddress(CString sIPAddress)
{
	m_strArray.Add(sIPAddress);
}


BOOL CIPAddressListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	for(int nStr = 0; nStr < m_strArray.GetSize(); nStr++)
		m_ipListCtrl.AddString(m_strArray[nStr]);

	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CString CIPAddressListDlg::GetSelection()
{
	return m_sSelection;
}


void CIPAddressListDlg::OnLbnDblclkIpAddressList()
{
	OnBnClickedOk();
}
