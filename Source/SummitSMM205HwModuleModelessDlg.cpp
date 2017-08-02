// SummitSMM205ModelessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SummitSMM205HwModuleModelessDlg.h"
#include "SummitSMM205HwModule.h"
#include "SummitSMM205HwModuleSettingsDlg.h"

int CSummitSMM205HwModuleModelessDlg::m_nWinPos = 0;
// CSummitSMM205HwModuleModelessDlg dialog

CSummitSMM205HwModuleModelessDlg::CSummitSMM205HwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CSummitSMM205HwModuleBasicDlg(CSummitSMM205HwModuleModelessDlg::IDD, pParent)
{
}

CSummitSMM205HwModuleModelessDlg::~CSummitSMM205HwModuleModelessDlg()
{
}

void CSummitSMM205HwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CSummitSMM205HwModuleBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSummitSMM205HwModuleModelessDlg, CSummitSMM205HwModuleBasicDlg)
	ON_WM_DESTROY()	
	ON_BN_CLICKED(IDC_SUMMIT_SMM205_CTRL_WRITE_RADIO, OnBnClickedSummitSmm205CtrlWriteRadio)
	ON_BN_CLICKED(IDC_SUMMIT_SMM205_CTRL_READ_RADIO, OnBnClickedSummitSmm205CtrlReadRadio)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SUMMIT_DLG_SETTINGS_BTN, OnBnClickedSummitDlgSettingsBtn)
END_MESSAGE_MAP()


void  CSummitSMM205HwModuleModelessDlg::AlignWindow()
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
		nXPos += CSummitSMM205HwModuleModelessDlg::m_nWinPos;
        nYPos += CSummitSMM205HwModuleModelessDlg::m_nWinPos;
		CSummitSMM205HwModuleModelessDlg::m_nWinPos += 20;
		if(CSummitSMM205HwModuleModelessDlg::m_nWinPos == 220)
			CSummitSMM205HwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

// CSummitSMM205HwModuleModelessDlg message handlers

void CSummitSMM205HwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pSummitHwModule,NULL);
	CSummitSMM205HwModuleBasicDlg::OnDestroy();
}

void CSummitSMM205HwModuleModelessDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CSummitSMM205HwModuleModelessDlg::OnInitDialog()
{
	CSummitSMM205HwModuleBasicDlg::OnInitDialog();

	CSummitSMM205HwModuleModelessDlg::AlignWindow();

	if(m_pSummitHwModule != NULL)
	{
		SetWindowText(m_pSummitHwModule->GetDisplayName());
	}

	CString sMin, sMax;
	sMin.Format("%d",m_pSummitHwModule->GetMinValue());
	sMax.Format("%d",m_pSummitHwModule->GetMaxValue());
	SetDlgItemText(IDC_SUMMIT_MIN_MAX_STATIC,"[" + sMin + ".." + sMax + "]");
	SetWindowText(m_pSummitHwModule->GetDisplayName());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSummitSMM205HwModuleModelessDlg::OnBnClickedOk()
{
	CSummitSMM205HwModuleBasicDlg::OnBnClickedOk();

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pSummitHwModule,NULL);
	if(m_params.m_bWrite == TRUE)
	{
		if((m_params.m_nVoltage < m_pSummitHwModule->GetMinValue()) ||
			(m_params.m_nVoltage > m_pSummitHwModule->GetMaxValue()))
		{
			MessageBox("Voltage value exceeds min/max limitations!",NULL,MB_ICONERROR);
			::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pSummitHwModule,NULL);
			return;
		}
		if(m_pSummitHwModule->SetVoltage(m_params.m_nVoltage) == FALSE)
		{
			MessageBox("Summit: Error while setting voltage!");
			::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pSummitHwModule,NULL);
			return;
		}
	}
	else
	{
		if(m_pSummitHwModule->GetVoltage(&m_params.m_nVoltage) == TRUE)
		{
			CString sVoltage;
			sVoltage.Format("%d",m_params.m_nVoltage);
			SetDlgItemText(IDC_SUMMIT_SMM205_CTRL_VOL_EDIT,sVoltage);
		}
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pSummitHwModule,NULL);
}

void CSummitSMM205HwModuleModelessDlg::PostNcDestroy()
{
	CSummitSMM205HwModuleModelessDlg::m_nWinPos -= 20;
	if(CSummitSMM205HwModuleModelessDlg::m_nWinPos < 0)
		CSummitSMM205HwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}

void CSummitSMM205HwModuleModelessDlg::OnBnClickedSummitDlgSettingsBtn()
{
	CSummitSMM205HwModuleSettingsDlg settingsDlg;

	CString sWinTitle;
	
	GetWindowText(sWinTitle);
	settingsDlg.SetName(sWinTitle);

	CString sMin, sMax, sName, sExtRef;
	sMin.Format("%d",m_pSummitHwModule->GetMinValue());
	sMax.Format("%d",m_pSummitHwModule->GetMaxValue());
	sExtRef.Format("%d",m_pSummitHwModule->GetExtReferenceValue());
	settingsDlg.SetMin(sMin);
	settingsDlg.SetMax(sMax);
	settingsDlg.SetExtRef(sExtRef);
	settingsDlg.SetActiveChannel(m_pSummitHwModule->GetActiveChannel());

	if(settingsDlg.DoModal() == IDOK)
	{
		int nMin, nMax, nExtRef;
		VOLTAGE_CTRL_CHANNEL eChannel;

		sMin = settingsDlg.GetMin();
		sMax = settingsDlg.GetMax();
		sExtRef = settingsDlg.GetExtRef();
		eChannel = settingsDlg.GetActiveChannel();

		sscanf((LPCSTR)sMin,"%d",&nMin);
		sscanf((LPCSTR)sMax,"%d",&nMax);
		sscanf((LPCSTR)sExtRef,"%d",&nExtRef);
		m_pSummitHwModule->SetMinValue(nMin);
		m_pSummitHwModule->SetMaxValue(nMax);
		m_pSummitHwModule->SetExtReferenceValue(nExtRef);
		m_pSummitHwModule->SetActiveChannel(eChannel);

		//Update min/max changes
		CString sMin, sMax, sExtRef;
		sMin.Format("%d",m_pSummitHwModule->GetMinValue());
		sMax.Format("%d",m_pSummitHwModule->GetMaxValue());
		SetDlgItemText(IDC_SUMMIT_MIN_MAX_STATIC,"[" + sMin + ".." + sMax + "]");

		//Notify name change only if differnt
		sName = settingsDlg.GetName();
		if(sName != m_pSummitHwModule->GetDisplayName())
		{
			::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_MODELESS_DLG_DIS_NAME_CHANGE,(WPARAM)m_pSummitHwModule,(LPARAM)&sName);
			//Name should be updated at this point (send message returns after change is made)
			SetWindowText(m_pSummitHwModule->GetDisplayName());
		}
	}
}

void  CSummitSMM205HwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pSummitHwModule = dynamic_cast<CSummitSMM205HwModule*>(pItem);
}

BOOL  CSummitSMM205HwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CSummitSMM205HwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CSummitSMM205HwModuleModelessDlg::Clone()
{
	return new CSummitSMM205HwModuleModelessDlg();
}
