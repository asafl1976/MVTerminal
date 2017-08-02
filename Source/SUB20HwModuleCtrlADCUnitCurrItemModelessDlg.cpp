#include "StdAfx.h"
#include "SUB20HwModuleCtrlADCUnitCurrItemModelessDlg.h"
#include "SUB20HwModuleCtrlADCUnitCurrentItem.h"

UINT UpdateCurrentGraphThread(LPVOID pParam);


CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg(CWnd* pParent)
: CSUB20HwModuleCtrlADCUnitVCItemModelessDlg(pParent)
{
	memset(m_sampleArray,0,SAMPLE_ARRAY_SIZE);
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
	m_sMeasureUnits	    = "mA";
	m_nResistorVal      = 47;

	m_nAmp    = 15000;
	m_nOffset = 7500;

	m_sampleValuelineColor   = RGB(128, 255, 255);
	m_AvsampleValuelineColor = RGB(0, 255, 0);
}

CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::~CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg(void)
{
}

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::OnDestroy()
{
	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pCurrentItem,NULL);
	if(m_pUpdateGraphThread != NULL)
	{
		m_bUpdateGraph = false;
		Sleep(100);//Wait for thread to finish
		m_pUpdateGraphThread->SuspendThread();
		delete m_pUpdateGraphThread;
	}
	CDialog::OnDestroy();
}

BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_PAUSE_BTN, OnBnClickedHwModuleAdcModDlgPauseBtn)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_CHECK, OnBnClickedHwModuleAdcModDlgVolStatAvrgCheck)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_RESET_BTN, OnBnClickedHwModuleAdcModDlgResetBtn)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_SHOW_GRID_CHECK, OnBnClickedHwModuleAdcModDlgShowGridCheck)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT, OnEnChangeHwModuleAdcModDlgVolNameEdit)
	ON_EN_CHANGE(IDC_HW_MODULE_ADC_MOD_DLG_VOL_RESISTOR_EDIT, OnEnChangeHwModuleAdcModDlgVolResistorEdit)
	ON_BN_CLICKED(IDC_HW_MODULE_ADC_MOD_DLG_XL_SNAPSHOT_BTN, OnBnClickedHwModuleAdcModDlgXlSnapshotBtn)

	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_RATE_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolSampleRateSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_INTERVAL_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolSampleIntervalSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_SLIDER, OnNMReleasedcaptureHwModuleAdcModDlgVolGridSizeSlider)

END_MESSAGE_MAP()

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::OnEnChangeHwModuleAdcModDlgVolNameEdit()
{
	CString sName;

	GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT,sName);
	//Notify name change only if differnt
	if(sName != m_sName)
	{
		m_sName = sName;
		::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_UNIT_ITEM_DIS_NAME_CHANGE,(WPARAM)m_pCurrentItem,(LPARAM)&m_sName);
	}
    //Name should be updated at this point
	SetWindowText(m_pCurrentItem->GetDisplayName());
}


BOOL CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::OnInitDialog()
{
	if(m_pCurrentItem == NULL)
		return FALSE;


	m_nMin = m_pCurrentItem->GetMin();
	m_nMax = m_pCurrentItem->GetMax();

	m_nResistorVal = m_pCurrentItem->GetResistorValue();

	CSUB20HwModuleCtrlADCUnitVCItemModelessDlg::OnInitDialog();

	SetWindowText(m_pCurrentItem->GetDisplayName());
	m_sName = m_pCurrentItem->GetDisplayNameSuffix() ;
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT,m_sName);

		//Set amp and offset sliders
	m_ampSlider.SetRange(0, 15000, FALSE);
	m_ampSlider.SetPos(m_nAmp);

	m_offsetSlider.SetRange(0, 15000, FALSE);
	m_offsetSlider.SetPos(m_nOffset);

	CString sAmp;
	sAmp.Format("%d",m_nAmp);
	SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_AMP_SLIDER_STATIC,sAmp);
	
	CString sOffset;
	sOffset.Format("%d",m_nOffset);
	SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_OFFSET_SLIDER_STATIC,sOffset);

	CString sResVal;
	sResVal.Format("%0.2f",m_nResistorVal);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_RESISTOR_EDIT,sResVal);

	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_CUR_RANGE_STATIC,"Range (mA):");
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_CUR_STATS_STATIC,"Statistics (mA):");

	UpdateGraph();

    // Create thread
    m_pUpdateGraphThread = AfxBeginThread(UpdateCurrentGraphThread,  (LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

UINT UpdateCurrentGraphThread(LPVOID pParam)
{
	CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg *phObjectHandle = static_cast<CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg *>(pParam);

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

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::UpdateUIGraphData(  )
{
	UINT  nSampleSum;
	int   nCurrent;

	if(m_pCurrentItem->GetCurrent(m_nResistorVal,&nCurrent) == FALSE)
	{
		nCurrent = 0;
	}
	m_nSampleCount++;
	//If current is negetive, force to 0
	if(nCurrent < 0)
	{
		nCurrent = 0;
	}

	m_sampleArray[m_nSampleArrayIndex] = nCurrent;
	m_nSampleArrayIndex++;
	if(m_nSampleArrayIndex == SAMPLE_ARRAY_SIZE)
		m_nSampleArrayIndex = 0;

	if(m_nSampleCount > SAMPLE_ARRAY_SIZE)
	{
		m_nSampleCount = SAMPLE_ARRAY_SIZE;
		m_bArrayIsFull = true;
	}

	nSampleSum = GetSampleSum(m_sampleArray,m_nSampleCount);

	m_nMinSample = (nCurrent < m_nMinSample)? nCurrent:m_nMinSample;
	m_nMaxSample = (nCurrent > m_nMaxSample)? nCurrent:m_nMaxSample;;

	m_nAverage = (float)((float)nSampleSum/(float)m_nSampleCount);
	m_PushGraph.Push((int)nCurrent,  VOL_PUSHGRAPH_VAL_SAMPLE_LINE);
	if(m_bShowAvInGraph == true)
	{
		m_PushGraph.Push((int)m_nAverage, VOL_PUSHGRAPH_AV_VAL_SAMPLE_LINE);	
	}
	m_PushGraph.Update();
	CString sMax,sMin,sAv;
	sMax.Format("%d",m_nMaxSample);
	sMin.Format("%d",m_nMinSample);
	sAv.Format("%0.2f",m_nAverage);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MIN_EDIT,sMin);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MAX_EDIT,sMax);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_EDIT,sAv);
	m_nSampleCount++;
}

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::StopUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->SuspendThread();
	}
}

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::StartUpdateThread()
{
	if(m_pUpdateGraphThread != NULL)
	{
		m_pUpdateGraphThread->ResumeThread();
	}
}
void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::OnEnChangeHwModuleAdcModDlgVolResistorEdit()
{
	//Get resistor value
	CString sResVal;
	GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_RESISTOR_EDIT,sResVal);
	if(sResVal.GetLength() == 0)
	{
		MessageBox("Invalid resistor value!",NULL,MB_ICONERROR);
		return;
	}

	for(int i = 0; i < sResVal.GetLength() ; i++)
	{
		switch(sResVal[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			break;
		default:
			MessageBox("Invalid resistor value!",NULL,MB_ICONERROR);
			return;
		}
	}
	sscanf(sResVal,"%f",&m_nResistorVal);
	m_pCurrentItem->SetResistorValue(m_nResistorVal);

}


void  CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pCurrentItem = dynamic_cast<CSUB20HwModuleCtrlADCUnitCurrentItem*>(pItem);
}

BOOL  CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::Clone()
{
	return new CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg();
}

void CSUB20HwModuleCtrlADCUnitCurrItemModelessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
		m_nOffset = m_offsetSlider.GetPos();;
		CString sOffset;
		sOffset.Format("%d",m_nOffset);
		SetDlgItemText(IDC_HW_MODULE_CTRL_CUR_VOL_OFFSET_SLIDER_STATIC,sOffset);

	}
	m_nMin = m_nOffset - (m_nAmp/2);
	m_nMax = m_nOffset + (m_nAmp/2);

	m_pCurrentItem->SetMin(m_nMin);
	m_pCurrentItem->SetMax(m_nMax);

	UpdateGraph();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
