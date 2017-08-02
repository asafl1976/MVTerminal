// ODPGControModelesslDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "ODPGHwModuleModelessDlg.h"
#include "ODPGHwModuleRegAccessModelessDlg.h"


// CODPGHwModuleModelessDlg dialog
int CODPGHwModuleModelessDlg::m_nWinPos = 0;

CODPGHwModuleModelessDlg::CODPGHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CODPGHwModuleBasicDlg(CODPGHwModuleModelessDlg::IDD, pParent)
{ 
	m_pRWRegDlg = NULL;
}

CODPGHwModuleModelessDlg::~CODPGHwModuleModelessDlg()
{
	if(m_pRWRegDlg != NULL)
		delete m_pRWRegDlg;
}

void CODPGHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CODPGHwModuleBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CODPGHwModuleModelessDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ODPG_CTRL_RW_REG_BTN, OnBnClickedOdpgCtrlRwRegBtn)
	ON_BN_CLICKED(IDC_ODPG_CTRL_READ_COMP_DATA_CHECK, OnBnClickedOdpgCtrlReadCompDataCheck)
	ON_BN_CLICKED(IDC_ODPG_CTRL_DATA_BUS_RADIO, OnBnClickedOdpgCtrlDataBusRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_BUS_RADIO, OnBnClickedOdpgCtrlBusRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_CONT_RADIO, OnBnClickedOdpgCtrlStopCondContRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_SINGLE_PATTERN_RADIO, OnBnClickedOdpgCtrlStopCondSinglePatternRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_ADDR_RADIO, OnBnClickedOdpgCtrlStopCondPreDefAddrRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_ERR_DATA_RADIO, OnBnClickedOdpgCtrlStopCondErrDataRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_STOP_COND_PRE_DEF_PHASE_COUNT_RADIO, OnBnClickedOdpgCtrlStopCondPreDefPhaseCountRadio)
	ON_NOTIFY(NM_DBLCLK, IDC_ODPG_CTRL_PATTERN_TREE, OnNMDblclkOdpgCtrlPatternTree)
	ON_NOTIFY(NM_CLICK, IDC_ODPG_CTRL_PATTERN_TREE, OnNMClickOdpgCtrlPatternTree)
	ON_CBN_SELCHANGE(IDC_ODPG_CTRL_TARGET_DEVICE_COMBO, OnCbnSelchangeOdpgCtrlTargetDeviceCombo)
	ON_CBN_SELCHANGE(IDC_ODPG_CTRL_BUS_WIDTH_COMBO, OnCbnSelchangeOdpgCtrlBusWidthCombo)
	ON_BN_CLICKED(IDC_ODPG_CTRL_READ_RADIO, OnBnClickedOdpgCtrlReadRadio)
	ON_BN_CLICKED(IDC_ODPG_CTRL_WRITE_RADIO, OnBnClickedOdpgCtrlWriteRadio)
END_MESSAGE_MAP()


void CODPGHwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pODPGModule,NULL);
	CODPGHwModuleBasicDlg::OnDestroy();
}

void  CODPGHwModuleModelessDlg::AlignWindow()
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
		nXPos += CODPGHwModuleModelessDlg::m_nWinPos;
        nYPos += CODPGHwModuleModelessDlg::m_nWinPos;
		CODPGHwModuleModelessDlg::m_nWinPos += 20;
		if(CODPGHwModuleModelessDlg::m_nWinPos == 220)
			CODPGHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

void CODPGHwModuleModelessDlg::PostNcDestroy()
{
	CODPGHwModuleModelessDlg::m_nWinPos -= 20;
	if(CODPGHwModuleModelessDlg::m_nWinPos < 0)
		CODPGHwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}

BOOL CODPGHwModuleModelessDlg::SetODPGParams()
{
	if(m_pODPGModule == NULL)
	{
		MessageBox("ODPG module = NULL!",NULL,MB_ICONERROR);
		return FALSE;
	}
	if(m_addrEditCtrl.GetValue(m_params.m_nAddress) == FALSE)
	{
		MessageBox("Enter valid base address!",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(m_params.m_pPattern == NULL)
	{
		MessageBox("No pattern was selected!",NULL,MB_ICONERROR);
		return FALSE;
	}
	m_pODPGModule->SetParams(m_params);
	return TRUE;
}

BOOL CODPGHwModuleModelessDlg::ActivateODPG()
{
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pODPGModule,NULL);

	if(m_pODPGModule->Set() == FALSE)
	{
		MessageBox("ODPG Initialization failed!",NULL,MB_ICONERROR);
		return FALSE;
	}	

	if(m_pODPGModule->Start() == FALSE)
	{
		MessageBox("ODPG Start failed!",NULL,MB_ICONERROR);
		return FALSE;
	}

	if(m_params.m_eStopCond == ODPG_STOP_COND_SINGLE_PATTERN)
	{
		if(m_pODPGModule->Stop() == FALSE)
		{
			MessageBox("ODPG stop failed!",NULL,MB_ICONERROR);
			return FALSE;
		}
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pODPGModule,NULL);
	return TRUE;
}
void CODPGHwModuleModelessDlg::OnOK()
{	
	CString sWinText;

	GetDlgItem(IDOK)->GetWindowText(sWinText);
	if(sWinText == "Stop")
	{
		if(m_pODPGModule->Stop() == FALSE)
		{
			MessageBox("ODPG stop failed!",NULL,MB_ICONERROR);
			return;
		}
		GetDlgItem(IDOK)->SetWindowText("Start");
		return;
	}

	if(SetODPGParams() == FALSE)
	{
		return;
	}
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	ActivateODPG();
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	if(m_params.m_eStopCond == ODPG_STOP_COND_CONTINUOUS)
	{
		GetDlgItem(IDOK)->SetWindowText("Stop");
	}

}

void CODPGHwModuleModelessDlg::OnCancel()
{
	DestroyWindow();
}

// CODPGHwModuleModelessDlg message handlers

BOOL CODPGHwModuleModelessDlg::OnInitDialog()
{
	CODPGHwModuleBasicDlg::OnInitDialog();
	AlignWindow();

	SetWindowText(m_pODPGModule->GetDisplayName());
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CODPGHwModuleModelessDlg::OnBnClickedOdpgCtrlRwRegBtn()
{
	if(m_pRWRegDlg == NULL)
	{
		m_pRWRegDlg = new CODPGHwModuleRegAccessModelessDlg;
		m_pRWRegDlg->SetHwModuleItem(m_pODPGModule);
		if(m_pRWRegDlg->Create(this) == FALSE) 
        { 
			delete m_pRWRegDlg;
            m_pRWRegDlg = NULL;
            return;
        } 

		m_pRWRegDlg->ShowWindow(SW_SHOW);
		m_pRWRegDlg->SetForegroundWindow();
	}
    else /* already exists */
    {
		m_pRWRegDlg->ShowWindow(SW_SHOW);
		if(m_pRWRegDlg->IsIconic())   // in case the minimize button is enabled
		{
			m_pRWRegDlg->ShowWindow(SW_RESTORE);
		}
		m_pRWRegDlg->SetForegroundWindow();
	}
}
void  CODPGHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pODPGModule = dynamic_cast<CODPGHwModule*>(pItem);
}

BOOL  CODPGHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CODPGHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CODPGHwModuleModelessDlg::Clone()
{
	return new CODPGHwModuleModelessDlg();
}
