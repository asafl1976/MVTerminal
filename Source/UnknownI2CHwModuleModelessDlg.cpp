// UnknownI2CDevCtrlModelessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "UnknownI2CHwModuleModelessDlg.h"


// CUnknownI2CHwModuleModelessDlg dialog
int CUnknownI2CHwModuleModelessDlg::m_nWinPos = 0;

CUnknownI2CHwModuleModelessDlg::CUnknownI2CHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CUnknownI2CHwModuleBasicDlg(CUnknownI2CHwModuleModelessDlg::IDD, pParent)
{
}

CUnknownI2CHwModuleModelessDlg::~CUnknownI2CHwModuleModelessDlg()
{
}

void CUnknownI2CHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CUnknownI2CHwModuleBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnknownI2CHwModuleModelessDlg, CUnknownI2CHwModuleBasicDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_EN_SETFOCUS(IDC_GEN_I2C_DEV_CTRL_HEX_EDIT, OnEnSetfocusGenI2cDevCtrlHexEdit)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_READ_RADIO, OnBnClickedGenI2cDevCtrlReadRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_WRITE_RADIO, OnBnClickedGenI2cDevCtrlWriteRadio)
	ON_BN_CLICKED(IDC_GEN_I2C_DEV_CTRL_SEND_STOP_CHECK, OnBnClickedGenI2cDevCtrlSendStopCheck)
END_MESSAGE_MAP()


void CUnknownI2CHwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pUnknownI2CHwModule,NULL);
	CUnknownI2CHwModuleBasicDlg::OnDestroy();
}

void  CUnknownI2CHwModuleModelessDlg::AlignWindow()
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
		nXPos += CUnknownI2CHwModuleModelessDlg::m_nWinPos;
        nYPos += CUnknownI2CHwModuleModelessDlg::m_nWinPos;
		CUnknownI2CHwModuleModelessDlg::m_nWinPos += 20;
		if(CUnknownI2CHwModuleModelessDlg::m_nWinPos == 220)
			CUnknownI2CHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

void CUnknownI2CHwModuleModelessDlg::PostNcDestroy()
{
	CUnknownI2CHwModuleModelessDlg::m_nWinPos -= 20;
	if(CUnknownI2CHwModuleModelessDlg::m_nWinPos < 0)
		CUnknownI2CHwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}

void CUnknownI2CHwModuleModelessDlg::OnCancel()
{
	DestroyWindow();
}

// CUnknownI2CHwModuleModelessDlg message handlers

BOOL CUnknownI2CHwModuleModelessDlg::OnInitDialog()
{
	CUnknownI2CHwModuleBasicDlg::OnInitDialog();

	CUnknownI2CHwModuleModelessDlg::AlignWindow();

	if(m_pUnknownI2CHwModule != NULL)
	{
		SetWindowText(m_pUnknownI2CHwModule->GetDisplayName());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUnknownI2CHwModuleModelessDlg::OnOK()
{
	CString sCount;

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pUnknownI2CHwModule,NULL);
	if(m_params.m_bRead == TRUE)
	{
		GetDlgItemText(IDC_GEN_I2C_DEV_CTRL_READ_BC_EDIT,sCount);
		if(sCount.IsEmpty() == TRUE)
		{
			MessageBox("Invalid byte count value!",NULL,MB_ICONERROR);
			return;
		}
		sscanf(sCount,"%d",&m_params.m_nByteCount);		
		m_pUnknownI2CHwModule->ReadData(m_params.m_nByteCount,(UCHAR*)m_params.m_data,m_params.m_bStopBit);			
		m_params.m_data[m_params.m_nByteCount] = NULL;
		m_dataHexEdit.SetData(m_params.m_data,m_params.m_nByteCount);

	}
	else //write
	{
		m_params.m_nByteCount = m_dataHexEdit.GetData(m_params.m_data,64);
		// Set end of data
		m_params.m_data[m_params.m_nByteCount] = NULL;
		m_pUnknownI2CHwModule->WriteData(m_params.m_nByteCount,m_params.m_data,m_params.m_bStopBit);
	}	
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pUnknownI2CHwModule,NULL);
}

void  CUnknownI2CHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pUnknownI2CHwModule = dynamic_cast<CUnknownI2CHwModule*>(pItem);
}

BOOL  CUnknownI2CHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CUnknownI2CHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CUnknownI2CHwModuleModelessDlg::Clone()
{
	return new CUnknownI2CHwModuleModelessDlg();
}