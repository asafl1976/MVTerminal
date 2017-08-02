// ODPGCtrlRWRegModelessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MarvellDeviceHwModuleModelessDlg.h"
#include "MarvellDeviceHwModuleBasicDlg.h"


// CMarvellDeviceHwModuleModelessDlg dialog
int CMarvellDeviceHwModuleModelessDlg::m_nWinPos = 0;

CMarvellDeviceHwModuleModelessDlg::CMarvellDeviceHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CMarvellDeviceHwModuleBasicDlg(CMarvellDeviceHwModuleModelessDlg::IDD, pParent)
{
}

CMarvellDeviceHwModuleModelessDlg::~CMarvellDeviceHwModuleModelessDlg()
{
}

void CMarvellDeviceHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CMarvellDeviceHwModuleBasicDlg::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CMarvellDeviceHwModuleModelessDlg, CMarvellDeviceHwModuleBasicDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MARVELL_DEVICE_HWMODULE_READ_RADIO, OnBnClickedReadRadio)
	ON_BN_CLICKED(IDC_MARVELL_DEVICE_HWMODULE_WRITE_RADIO, OnBnClickedWriteRadio)
END_MESSAGE_MAP()

void  CMarvellDeviceHwModuleModelessDlg::AlignWindow()
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
		nXPos += CMarvellDeviceHwModuleModelessDlg::m_nWinPos;
        nYPos += CMarvellDeviceHwModuleModelessDlg::m_nWinPos;
		CMarvellDeviceHwModuleModelessDlg::m_nWinPos += 20;
		if(CMarvellDeviceHwModuleModelessDlg::m_nWinPos == 220)
			CMarvellDeviceHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

BOOL CMarvellDeviceHwModuleModelessDlg::OnInitDialog()
{
	CMarvellDeviceHwModuleBasicDlg::OnInitDialog();
	AlignWindow();

	if(m_pMarvellDeviceModule != NULL)
	{
		SetWindowText(m_pMarvellDeviceModule->GetDisplayName());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMarvellDeviceHwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pMarvellDeviceModule,NULL);
	CMarvellDeviceHwModuleBasicDlg::OnDestroy();
}


void CMarvellDeviceHwModuleModelessDlg::OnCancel()
{
	DestroyWindow();
}

void CMarvellDeviceHwModuleModelessDlg::OnBnClickedOk()
{
	CMarvellDeviceHwModuleBasicDlg::OnBnClickedOk();

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pMarvellDeviceModule,NULL);
	if(m_params.m_bRead == TRUE)
	{
		if(m_pMarvellDeviceModule->ReadRegister(m_params.m_nOffset,&m_params.m_nValue) == FALSE)
		{
			MessageBox("Marvell Device: Error while reading register!");
			::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceModule,NULL);
			return;
		}
		CString sValue;
		sValue.Format("%x",m_params.m_nValue);
		SetDlgItemText(IDC_MARVELL_DEVICE_HWMODULE_VALUE_EDIT,sValue);
	}
	else
	{
		if(m_pMarvellDeviceModule->WriteRegister(m_params.m_nOffset,m_params.m_nValue) == FALSE)
		{
			MessageBox("Marvell Device: Error while writing register!");
			::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceModule,NULL);
			return;
		}
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceModule,NULL);
}

void CMarvellDeviceHwModuleModelessDlg::PostNcDestroy()
{
	CMarvellDeviceHwModuleModelessDlg::m_nWinPos -= 20;
	if(CMarvellDeviceHwModuleModelessDlg::m_nWinPos < 0)
		CMarvellDeviceHwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}

// CODPGHwModuleModelessDlg message handlers


void  CMarvellDeviceHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pMarvellDeviceModule = dynamic_cast<CMarvellDeviceHwModule*>(pItem);
}


BOOL  CMarvellDeviceHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CMarvellDeviceHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CMarvellDeviceHwModuleModelessDlg::Clone()
{
	return new CMarvellDeviceHwModuleModelessDlg();
}