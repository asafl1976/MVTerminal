// CEWaitForStringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ODPGPatternSelectBitDlg.h"
#include ".\ODPGPatternSelectBitDlg.h"


IMPLEMENT_DYNAMIC(CODPGPatternSelectBitDlg, CDialog)
CODPGPatternSelectBitDlg::CODPGPatternSelectBitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CODPGPatternSelectBitDlg::IDD, pParent)
{
	m_nMaxBit = 16;
	m_nSelBit = 0;
}

CODPGPatternSelectBitDlg::~CODPGPatternSelectBitDlg()
{
}

void CODPGPatternSelectBitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_okBtn);
	DDX_Control(pDX, IDC_ODPG_PATTERN_BIT_COMBO, m_vicBitCombo);
}


BEGIN_MESSAGE_MAP(CODPGPatternSelectBitDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_ODPG_PATTERN_BIT_COMBO, OnCbnSelchangePatternBitCombo)
END_MESSAGE_MAP()


// CCmdExeCmdItemClearWindowDlg message handlers

void CODPGPatternSelectBitDlg::OnBnClickedOk()
{
	CString str;	
	GetDlgItemText(IDC_ODPG_PATTERN_BIT_COMBO,str);
	sscanf_s(str,"%d",&m_nSelBit);
	OnOK();
}

BOOL CODPGPatternSelectBitDlg::OnInitDialog()
{	
	CString	 sBit;

	CDialog::OnInitDialog();
	m_okBtn.SetTheme(xtpButtonThemeOffice2000);

	for(int i = 0; i < m_nMaxBit; i++)
	{
		sBit.Format("%d",i);
		m_vicBitCombo.AddString(sBit);
	}

	sBit.Format("%d",m_nSelBit);
	m_vicBitCombo.SelectString(-1,sBit);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CODPGPatternSelectBitDlg::OnCbnSelchangePatternBitCombo()
{
}
