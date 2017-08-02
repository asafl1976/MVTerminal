#include "StdAfx.h"
#include ".\clocketphwmodulemodelessdlg.h"


int CClockETPHwModuleModelessDlg::m_nWinPos = 0;
// CClockETPHwModuleModelessDlg dialog

CClockETPHwModuleModelessDlg::CClockETPHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CClockETPHwModuleBasicDlg(CClockETPHwModuleModelessDlg::IDD, pParent)
{
}


CClockETPHwModuleModelessDlg::~CClockETPHwModuleModelessDlg(void)
{
}

void CClockETPHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CClockETPHwModuleBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClockETPHwModuleModelessDlg, CClockETPHwModuleBasicDlg)
	ON_WM_DESTROY()	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


void  CClockETPHwModuleModelessDlg::AlignWindow()
{
    CRect rectFrame, rectDlg;
    CWnd* pMainWnd = AfxGetMainWnd();
    if(pMainWnd != NULL)
    {
        pMainWnd->GetClientRect(rectFrame);
        pMainWnd->ClientToScreen(rectFrame);
        GetWindowRect(rectDlg);

        int nXPos = rectFrame.left + (rectFrame.Width() / 2) 
                                   - (rectDlg.Width() / 2);
        int nYPos = rectFrame.top + (rectFrame.Height() / 3) 
                                  - (rectDlg.Height() / 3);
        
		//Cascade windows
		nXPos += CClockETPHwModuleModelessDlg::m_nWinPos;
        nYPos += CClockETPHwModuleModelessDlg::m_nWinPos;
		CClockETPHwModuleModelessDlg::m_nWinPos += 20;
		if(CClockETPHwModuleModelessDlg::m_nWinPos == 220)
			CClockETPHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

// CSummitSMM205HwModuleModelessDlg message handlers

void CClockETPHwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pClockETPHwModule,NULL);
	CClockETPHwModuleBasicDlg::OnDestroy();
}

void CClockETPHwModuleModelessDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CClockETPHwModuleModelessDlg::OnInitDialog()
{
	CClockETPHwModuleBasicDlg::OnInitDialog();

	CClockETPHwModuleModelessDlg::AlignWindow();

	if(m_pClockETPHwModule != NULL)
	{
		SetWindowText(m_pClockETPHwModule->GetDisplayName());
	}

	CString sFreq, sRef, sRange;
	sFreq.Format("%d",m_params.m_nFrequency);
	sRef.Format("%d",m_params.m_nRef);
	sRange.Format("%d",m_params.m_nRange);

	SetDlgItemText(IDC_CLOCK_ETP_CTRL_FREQ_EDIT, sFreq);
	SetDlgItemText(IDC_CLOCK_ETP_CTRL_REF_EDIT, sRef);
	SetDlgItemText(IDC_CLOCK_ETP_CTRL_RANGE_EDIT, sRange);

 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CClockETPHwModuleModelessDlg::OnBnClickedOk()
{
	CClockETPHwModuleBasicDlg::OnBnClickedOk();

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pClockETPHwModule,NULL);
	if(m_pClockETPHwModule->SetFrequancy(m_params.m_nFrequency,m_params.m_nRef,m_params.m_nRange) == FALSE)
	{
		MessageBox("Clock ETP: 1 Error while setting frequency!");
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pClockETPHwModule,NULL);
}

void CClockETPHwModuleModelessDlg::PostNcDestroy()
{
	CClockETPHwModuleModelessDlg::m_nWinPos -= 20;
	if(CClockETPHwModuleModelessDlg::m_nWinPos < 0)
		CClockETPHwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}

void  CClockETPHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pClockETPHwModule = dynamic_cast<CClockETPHwModule*>(pItem);
}

BOOL  CClockETPHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CClockETPHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CClockETPHwModuleModelessDlg::Clone()
{
	return new CClockETPHwModuleModelessDlg();
}