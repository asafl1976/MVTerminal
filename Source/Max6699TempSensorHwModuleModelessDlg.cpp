#include "StdAfx.h"
#include "Max6699TempSensorHwModuleModelessDlg.h"

UINT UpdateTemperatureGraphThread(LPVOID pParam);

int CMax6699TempSensorHwModuleModelessDlg::m_nWinPos = 0;
// CSummitSMM205HwModuleModelessDlg dialog

CMax6699TempSensorHwModuleModelessDlg::CMax6699TempSensorHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CMax6699TempSensorHwModuleBasicDlg(CMax6699TempSensorHwModuleModelessDlg::IDD, pParent)
{
	m_nSampleArrayIndex = 0;
}

CMax6699TempSensorHwModuleModelessDlg::~CMax6699TempSensorHwModuleModelessDlg()
{
}

void CMax6699TempSensorHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CMax6699TempSensorHwModuleBasicDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMax6699TempSensorHwModuleModelessDlg, CMax6699TempSensorHwModuleBasicDlg)
	ON_WM_DESTROY()	
END_MESSAGE_MAP()


void  CMax6699TempSensorHwModuleModelessDlg::AlignWindow()
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
		nXPos += CMax6699TempSensorHwModuleModelessDlg::m_nWinPos;
        nYPos += CMax6699TempSensorHwModuleModelessDlg::m_nWinPos;
		CMax6699TempSensorHwModuleModelessDlg::m_nWinPos += 20;
		if(CMax6699TempSensorHwModuleModelessDlg::m_nWinPos == 220)
			CMax6699TempSensorHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

// CSummitSMM205HwModuleModelessDlg message handlers

void CMax6699TempSensorHwModuleModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pTempSensorHwModule,NULL);
	CMax6699TempSensorHwModuleBasicDlg::OnDestroy();
}

void CMax6699TempSensorHwModuleModelessDlg::OnCancel()
{
	StopUpdateThread();
	DestroyWindow();
}

BOOL CMax6699TempSensorHwModuleModelessDlg::OnInitDialog()
{
	CMax6699TempSensorHwModuleBasicDlg::OnInitDialog();

	CMax6699TempSensorHwModuleModelessDlg::AlignWindow();

	if(m_pTempSensorHwModule != NULL)
	{
		SetWindowText(m_pTempSensorHwModule->GetDisplayName());
	}

	CString sTemp;
	int nTemp = 0;
	sTemp.Format("%d C°",m_pTempSensorHwModule->GetTemperature(&nTemp));
	SetDlgItemText(IDC_TEMP_LABEL_STATIC,sTemp);
	SetWindowText(m_pTempSensorHwModule->GetDisplayName());

    // Create thread
    m_pUpdateGraphThread = AfxBeginThread(UpdateTemperatureGraphThread,  (LPVOID)this/*,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED*/);
	//m_pUpdateGraphThread->ResumeThread();
	//StartUpdateThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

UINT UpdateTemperatureGraphThread(LPVOID pParam)
{
	CMax6699TempSensorHwModuleModelessDlg *phObjectHandle = static_cast<CMax6699TempSensorHwModuleModelessDlg *>(pParam);
	while(1)
	{
		if(phObjectHandle != NULL)
		{
			phObjectHandle->UpdateUIGraphData();
			Sleep(500);
		}
	}
}

void CMax6699TempSensorHwModuleModelessDlg::PostNcDestroy()
{
	StopUpdateThread();

	CMax6699TempSensorHwModuleModelessDlg::m_nWinPos -= 20;
	if(CMax6699TempSensorHwModuleModelessDlg::m_nWinPos < 0)
		CMax6699TempSensorHwModuleModelessDlg::m_nWinPos = 0;

	delete this;
}


void  CMax6699TempSensorHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pTempSensorHwModule = dynamic_cast<CMax6699TempSensorHwModule*>(pItem);
}

BOOL  CMax6699TempSensorHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CMax6699TempSensorHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CMax6699TempSensorHwModuleModelessDlg::Clone()
{
	return new CMax6699TempSensorHwModuleModelessDlg();
}

void CMax6699TempSensorHwModuleModelessDlg::StopUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->SuspendThread();
	}
}

void CMax6699TempSensorHwModuleModelessDlg::StartUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->ResumeThread();
	}
}


void CMax6699TempSensorHwModuleModelessDlg::UpdateUIGraphData()
{
	int   nTemperature = 0;

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pTempSensorHwModule,NULL);
	if(m_pTempSensorHwModule->GetTemperature(&nTemperature) == FALSE)
	{
		nTemperature = 0;
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pTempSensorHwModule,NULL);

	if(nTemperature < 0)
		nTemperature = 0;

	int colorMapIndex = nTemperature/2;
	if(colorMapIndex > 69)
		colorMapIndex = 69;
	m_tempTextLabel.SetTextColor(colorMap[colorMapIndex]);

	m_PushGraph.Push((int)nTemperature,  TEMP_PUSHGRAPH_VAL_SAMPLE_LINE);
	m_PushGraph.Update();
	
	CString sTemp;
	sTemp.Format("%d C°",nTemperature);
	SetDlgItemText(IDC_TEMP_LABEL_STATIC,sTemp);



}