#include "StdAfx.h"
#include "SUB20HwModuleCtrlADCUnitVolItemModelessDlg.h"
#include "SUB20HwModuleCtrlADCUnitVoltageItem.h"

UINT UpdateVoltageGraphThread(LPVOID pParam);

CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::CSUB20HwModuleCtrlADCUnitVolItemModelessDlg(CWnd* pParent)
: CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(pParent)

{
	m_nSampleCount      = 0;
	m_nSampleArrayIndex = 0;
	m_nGridSize         = 15;
	m_nSampleInterval   = 4;
	m_nAverage          = 0;
	m_nMinSample        = 100000;
	m_nMaxSample        = 0;
	m_nSampleRate       = 300;
	m_bUpdateGraph      = false;
	m_bShowAvInGraph    = false;
	m_bShowGrid         = true;
	m_sMeasureUnits	    = "mV";


	m_nAmp    = 3300;
	m_nOffset = 1650;

	m_sampleValuelineColor   = RGB(255, 255, 0);
	m_AvsampleValuelineColor = RGB(0, 255, 0);
}

CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::~CSUB20HwModuleCtrlADCUnitVolItemModelessDlg(void)
{
}


void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::OnDestroy()
{

	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pVolItem,NULL);

	if(m_pUpdateGraphThread != NULL)
	{
		m_bUpdateGraph = false;
		Sleep(100);//Wait for thread to finish
		m_pUpdateGraphThread->SuspendThread();
		delete m_pUpdateGraphThread;
	}
	CDialog::OnDestroy();
}

BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlADCUnitVolItemModelessDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_PAUSE_BTN, OnBnClickedHwModuleAdcModDlgPauseBtn)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_CHECK, OnBnClickedHwModuleAdcModDlgVolStatAvrgCheck)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_RESET_BTN, OnBnClickedHwModuleAdcModDlgResetBtn)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_SHOW_GRID_CHECK, OnBnClickedHwModuleAdcModDlgShowGridCheck)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT, OnEnChangeHwModuleAdcModDlgVolNameEdit)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_XL_SNAPSHOT_BTN, OnBnClickedHwModuleAdcModDlgXlSnapshotBtn)

	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_RATE_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolSampleRateSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_INTERVAL_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolSampleIntervalSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolGridSizeSlider)

END_MESSAGE_MAP()

void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::OnEnChangeHwModuleAdcModDlgVolNameEdit()
{
	CString sName;

	GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT,sName);
	//Notify name change only if differnt
	if(sName != m_sName)
	{
		m_sName = sName;
		::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_UNIT_ITEM_DIS_NAME_CHANGE,(WPARAM)m_pVolItem,(LPARAM)&m_sName);
	}
    //Name should be updated at this point
	SetWindowText(m_pVolItem->GetDisplayName());
}
BOOL CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::OnInitDialog()
{
	if(m_pVolItem == NULL)
		return FALSE;

	m_nMin = m_pVolItem->GetMin();
	m_nMax = m_pVolItem->GetMax();

	CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::OnInitDialog();

	//Disable Resistor ctrls
	GetDlgItem(IDC_HW_MODULE_ADC_MOD_DLG_VOL_RESISTOR_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_HW_MODULE_ADC_MOD_DLG_VOL_RESISTOR_VAL_STATIC)->ShowWindow(FALSE);

		//Set amp and offset sliders
	m_ampSlider.SetRange(0, 3300, FALSE);
	m_ampSlider.SetPos(m_nAmp);

	m_offsetSlider.SetRange(0, 3300, FALSE);
	m_offsetSlider.SetPos(m_nOffset);

	//Set amp and offset sliders
	m_ampSlider.SetRange(0, 3300, FALSE);
	m_ampSlider.SetPos(m_nAmp);

	m_offsetSlider.SetRange(0, 3300, FALSE);
	m_offsetSlider.SetPos(m_nOffset);


	CString sMaxAmp;
	sMaxAmp.Format("%d",3300);
	SetDlgItemText(IDC_MAX_AMP_STATIC,sMaxAmp);

	CString sMaxOffset;
	sMaxOffset.Format("%d",3300);
	SetDlgItemText(IDC_MAX_OFFSET_STATIC,sMaxOffset);

	CString sAmp;
	sAmp.Format("%d",m_nAmp);
	SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_AMP_SLIDER_STATIC,sAmp);
	
	CString sOffset;
	sOffset.Format("%d",m_nOffset);
	SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_OFFSET_SLIDER_STATIC,sOffset);



	SetWindowText(m_pVolItem->GetDisplayName());
	m_sName = m_pVolItem->GetDisplayNameSuffix() ;
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT,m_sName);
	UpdateGraph();

     // Create thread
    m_pUpdateGraphThread = AfxBeginThread(UpdateVoltageGraphThread, (LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
UINT UpdateVoltageGraphThread(LPVOID pParam)
{
	CSUB20HwModuleCtrlADCUnitVolItemModelessDlg *phObjectHandle = static_cast<CSUB20HwModuleCtrlADCUnitVolItemModelessDlg *>(pParam);

	while(1)
	{
		phObjectHandle->UpdateUIGraphData();
		if(phObjectHandle->m_nSampleRate > 5)
		{
			Sleep(phObjectHandle->m_nSampleRate);
		}
		if(phObjectHandle->m_bUpdateGraph == FALSE)
		{
			phObjectHandle->StopUpdateThread();
		}
	}
}

void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::UpdateUIGraphData()
{
	UINT  nSampleSum;
	int nVoltage;

	if(m_pVolItem->GetVoltage(&nVoltage) == FALSE)
	{
		nVoltage = 0;
	}
	m_nSampleCount++;

	m_sampleArray[m_nSampleArrayIndex] = nVoltage;
	m_nSampleArrayIndex++;

	if(m_nSampleArrayIndex == SAMPLE_ARRAY_SIZE)
	{
		m_nSampleArrayIndex = 0;
		m_bArrayIsFull = true;
	}

	if(m_nSampleCount > SAMPLE_ARRAY_SIZE)
	{
		m_nSampleCount = SAMPLE_ARRAY_SIZE;
	}

	nSampleSum = GetSampleSum(m_sampleArray,m_nSampleCount);

	m_nMinSample = (nVoltage < m_nMinSample)? nVoltage:m_nMinSample;
	m_nMaxSample = (nVoltage > m_nMaxSample)? nVoltage:m_nMaxSample;;
	m_nAverage = (float)((float)nSampleSum/(float)m_nSampleCount);

	m_PushGraph.Push(nVoltage,  VOL_PUSHGRAPH_VAL_SAMPLE_LINE);
	if(m_bShowAvInGraph == true)
		m_PushGraph.Push((int)m_nAverage, VOL_PUSHGRAPH_AV_VAL_SAMPLE_LINE);	
	m_PushGraph.Update();

	CString sMax,sMin,sAv;
	sMax.Format("%d",m_nMaxSample);
	sMin.Format("%d",m_nMinSample);
	sAv.Format("%0.2f",m_nAverage);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MIN_EDIT,sMin);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MAX_EDIT,sMax);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_EDIT,sAv);
}


void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::StopUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->SuspendThread(); 
	}
}

void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::StartUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->ResumeThread();
	}
}


void  CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pVolItem = dynamic_cast<CSUB20HwModuleCtrlADCUnitVoltageItem*>(pItem);
}

BOOL  CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::Clone()
{
	return new CSUB20HwModuleCtrlADCUnitVolItemModelessDlg();
}


void CSUB20HwModuleCtrlADCUnitVolItemModelessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar->GetDlgCtrlID() == IDC_SUB20_HWMODULE_ADC_CUR_VOL_AMP_SLIDER)
	{
		m_nAmp = m_ampSlider.GetPos();
		CString sAmp;
		sAmp.Format("%d",m_nAmp);
		SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_AMP_SLIDER_STATIC,sAmp);
	}
	if(pScrollBar->GetDlgCtrlID() == IDC_SUB20_HWMODULE_ADC_CUR_VOL_OFFSET_SLIDER)
	{
		m_nOffset = m_offsetSlider.GetPos();

		CString sOffset;
		sOffset.Format("%d",m_nOffset);
		SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_OFFSET_SLIDER_STATIC,sOffset);
	}
	
	m_nMin = m_nOffset - (m_nAmp/2);
	m_nMax = m_nOffset + (m_nAmp/2);

	m_pVolItem->SetMin(m_nMin);
	m_pVolItem->SetMax(m_nMax);


	UpdateGraph();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


