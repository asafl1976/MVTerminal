#include "StdAfx.h"
#include ".\ClockETPHwModuleBasicDlg.h"

//IMPLEMENT_DYNAMIC(CClockETPHwModuleBasicDlg, CDialog)

CClockETPHwModuleBasicDlg::CClockETPHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
: CBasicItemDlg(nIDTemplate,pParentWnd)
{
}

CClockETPHwModuleBasicDlg::~CClockETPHwModuleBasicDlg(void)
{
}
void CClockETPHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_exeBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);

}

BEGIN_MESSAGE_MAP(CClockETPHwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CClockETPHwModuleBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CClockETPHwModuleBasicDlg::AlignWindow();

	m_exeBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CClockETPHwModuleBasicDlg::OnBnClickedOk()
{
	CString sFrequency, sRef, sRange;

	GetDlgItemText(IDC_CLOCK_ETP_CTRL_FREQ_EDIT, sFrequency);	
	sscanf(sFrequency,"%d",&m_params.m_nFrequency);

	GetDlgItemText(IDC_CLOCK_ETP_CTRL_REF_EDIT, sRef);	
	sscanf(sRef,"%d",&m_params.m_nRef);

	GetDlgItemText(IDC_CLOCK_ETP_CTRL_RANGE_EDIT, sRange);	
	sscanf(sRange,"%d",&m_params.m_nRange);

	
}
