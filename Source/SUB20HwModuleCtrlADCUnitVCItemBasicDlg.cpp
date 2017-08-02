#include "StdAfx.h"
#include "SUB20HwModuleCtrlADCUnitVCItemBasicDlg.h"
#include "XLEzAutomation.h"

CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::CSUB20HwModuleCtrlADCUnitVCItemBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
:CBasicItemDlg(nIDTemplate,pParentWnd)
{
	m_bArrayIsFull = false;
	m_nSampleCount = 0;
	m_ampSliderVal = 23;
	m_offsetSliderVal = 23;
}

CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::~CSUB20HwModuleCtrlADCUnitVCItemBasicDlg(void)
{
	memset(m_sampleArray,0,SAMPLE_ARRAY_SIZE);
}

BEGIN_MESSAGE_MAP(CSUB20HwModuleCtrlADCUnitVCItemBasicDlg, CDialog)
END_MESSAGE_MAP()

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_RESET_BTN, m_resetStatBtn);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_RATE_SLIDER, m_GraphSampleRateSlider);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_PAUSE_BTN, m_PauseBtn);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_XL_SNAPSHOT_BTN, m_XLSnapShotBtn);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_VOL_SAMPLE_INTERVAL_SLIDER, m_sampleIntervalSlider);
	DDX_Control(pDX, IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_SLIDER, m_gridSizeSlider);
	DDX_Control(pDX, IDC_SUB20_HWMODULE_ADC_CUR_VOL_AMP_SLIDER, m_ampSlider);
	DDX_Control(pDX, IDC_SUB20_HWMODULE_ADC_CUR_VOL_OFFSET_SLIDER, m_offsetSlider);
	DDX_Slider(pDX, IDC_SUB20_HWMODULE_ADC_CUR_VOL_AMP_SLIDER, m_ampSliderVal);
	DDX_Slider(pDX, IDC_SUB20_HWMODULE_ADC_CUR_VOL_OFFSET_SLIDER, m_offsetSliderVal);

}

BOOL CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::AlignWindow();

	CString sSampleRate;
	sSampleRate.Format("%d",m_nSampleRate);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_SAMPLE_RATE_STATIC,sSampleRate);

	m_XLSnapShotBtn.EnableWindow(FALSE);
	m_XLSnapShotBtn.SetTheme(xtpButtonThemeOffice2000);
	m_okBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_resetStatBtn.SetTheme(xtpButtonThemeOffice2000);
	m_PauseBtn.SetTheme(xtpButtonThemeOffice2000);

	m_PushGraph.CreateFromStatic(IDC_HW_MODULE_ADC_MOD_DLG_GRAPH_STATIC, this);	

	CString sAvVol;

	CheckDlgButton(IDC_HW_MODULE_ADC_MOD_DLG_SHOW_GRID_CHECK,m_bShowGrid);
	CheckDlgButton(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_CHECK,m_bShowAvInGraph);

	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MIN_EDIT,"0");
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MAX_EDIT,"0");
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_EDIT,"0");


	//Set grid size slider
	m_gridSizeSlider.SetRange(5,50);
	m_gridSizeSlider.SetTic(1);
	m_gridSizeSlider.SetPos(m_nGridSize);

	//Set sample rate slider
	m_GraphSampleRateSlider.SetRange(1,500);
	m_GraphSampleRateSlider.SetTic(50);
	m_GraphSampleRateSlider.SetPos(m_nSampleRate);

	//Set interval slider
	m_sampleIntervalSlider.SetRange(1,20);
	m_sampleIntervalSlider.SetTic(5);
	m_sampleIntervalSlider.SetPos(m_nSampleInterval);

	CString sSampleInt;
	sSampleInt.Format("%d",m_nSampleInterval);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_SAMPLE_INT_STATIC,sSampleInt);

	CString sGridSize;
	sGridSize.Format("%d",m_nGridSize);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_STATIC,sGridSize);

	m_PushGraph.AddLine( VOL_PUSHGRAPH_VAL_SAMPLE_LINE,    m_sampleValuelineColor);
	m_PushGraph.SetTextColor(m_sampleValuelineColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
} 

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::UpdateGraph()
{
	CString sMax,sMin;

	sMax.Format("%d"+m_sMeasureUnits,m_nMax);
	sMin.Format("%d"+m_sMeasureUnits,m_nMin);
	m_PushGraph.SetLabelForMax( sMax ); 
	m_PushGraph.SetLabelForMin( sMin );
	m_PushGraph.ShowGrid( m_bShowGrid);
	m_PushGraph.ShowLabels( true);
	m_PushGraph.SetInterval( m_nSampleInterval );
	m_PushGraph.SetGridSize( m_nGridSize );

	if( m_nMin == m_nMax )
		m_PushGraph.SetPeekRange( m_nMin, m_nMax + 1);	
	else
		m_PushGraph.SetPeekRange( m_nMin, m_nMax);	

	m_PushGraph.Update();
}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedHwModuleAdcModDlgXlSnapshotBtn()
{
	//CXLEzAutomation class constructor starts Excel and creates empty worksheet  
	CXLEzAutomation XL;

	//All data will be stored by szData
	CString szData, sData;
	int nSampleArrayIndex, sampleArrayShadow[SAMPLE_ARRAY_SIZE], sampleArray[SAMPLE_ARRAY_SIZE];
	nSampleArrayIndex = m_nSampleArrayIndex;

	memcpy(sampleArrayShadow,m_sampleArray,SAMPLE_ARRAY_SIZE*sizeof(int));
	memset(sampleArray,0,SAMPLE_ARRAY_SIZE);

	int j = 0;
	if(m_bArrayIsFull == true)
	{
		//Copy the most recent samples first 
		for(int i = 0; i < (SAMPLE_ARRAY_SIZE - nSampleArrayIndex); i++)
		{
			sampleArray[j] = sampleArrayShadow[nSampleArrayIndex + i];
			j++;
		}
	}

	for(int i = 0; i < nSampleArrayIndex; i++,j++)
	{
		sampleArray[j] = sampleArrayShadow[i];
	}
	for(int i = 0; i < j; i++)
	{
		double x,y;
		x = i;
		y = sampleArray[i] ;
		sData.Format("%f\t%f\n", x, y);
		szData = szData + sData;
	}

	//Use clipboard export function to move all data to Excel worksheet
	XL.ExportCString(szData);
	//Y values for this plot are in Column = 2
	XL.CreateXYChart(2);

}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedHwModuleAdcModDlgPauseBtn()
{
	m_bUpdateGraph = !m_bUpdateGraph;
	if(m_bUpdateGraph == false)
	{
		m_PauseBtn.SetWindowText("Start");
		m_XLSnapShotBtn.EnableWindow(TRUE);
		//m_bUpdateGraph change to false causes the thread to stop
	}
	else
	{
		m_PauseBtn.SetWindowText("Pause");
		m_XLSnapShotBtn.EnableWindow(FALSE);
		StartUpdateThread();
	}
}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnNMReleasedcaptureHwModuleAdcModDlgVolSampleRateSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nSampleRate = m_GraphSampleRateSlider.GetPos();
	CString sSampleRate;
	sSampleRate.Format("%d",m_nSampleRate);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_SAMPLE_RATE_STATIC,sSampleRate);
   *pResult = 0;
}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedHwModuleAdcModDlgVolStatAvrgCheck()
{
	m_bShowAvInGraph = !m_bShowAvInGraph;
	if(m_bShowAvInGraph == false)
		m_PushGraph.RemoveLine(VOL_PUSHGRAPH_AV_VAL_SAMPLE_LINE);
	else
		m_PushGraph.AddLine(VOL_PUSHGRAPH_AV_VAL_SAMPLE_LINE,m_AvsampleValuelineColor);

}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedHwModuleAdcModDlgResetBtn()
{ 
	m_nAverage          = 0;
	m_nSampleCount      = 0;
	m_nMinSample        = 100000;
	m_nMaxSample        = 0;
	m_nSampleArrayIndex = 0;
	memset(m_sampleArray,0,SAMPLE_ARRAY_SIZE);

	CString sMax,sMin,sAv;
	sMax.Format("%d",m_nMaxSample);
	sMin.Format("%d",m_nMinSample);
	sAv.Format("%0.2f",m_nAverage);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MIN_EDIT,"0");
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_MAX_EDIT,sMax);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_AVRG_EDIT,sAv);

}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedHwModuleAdcModDlgShowGridCheck()
{
	m_bShowGrid = !m_bShowGrid;
	m_PushGraph.ShowGrid(m_bShowGrid);
	m_PushGraph.Update();
	GetDlgItem(IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_SLIDER)->EnableWindow(m_bShowGrid);
}


void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnNMReleasedcaptureHwModuleAdcModDlgVolSampleIntervalSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nSampleInterval = m_sampleIntervalSlider.GetPos();
	
	CString sSampleInt;
	sSampleInt.Format("%d",m_nSampleInterval);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_STAT_SAMPLE_INT_STATIC,sSampleInt);
	m_PushGraph.SetInterval( m_nSampleInterval );
	*pResult = 0;
}

void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnNMReleasedcaptureHwModuleAdcModDlgVolGridSizeSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_nGridSize = m_gridSizeSlider.GetPos();
	CString sGridSize;
	sGridSize.Format("%d",m_nGridSize);
	SetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_GRID_SIZE_STATIC,sGridSize);
	m_PushGraph.SetGridSize( m_nGridSize );
   *pResult = 0;
}

#if 0
void CSUB20HwModuleCtrlADCUnitVCItemBasicDlg::OnBnClickedOk()
{
	//CString sMinVol,sMaxVol;
	CString sName;

	//GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_MIN_RANGE_EDIT,sMinVol);
	//GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_MAX_RANGE_EDIT,sMaxVol);
	GetDlgItemText(IDC_HW_MODULE_ADC_MOD_DLG_VOL_NAME_EDIT,sName);

	//if((sMinVol.IsEmpty() == TRUE) || (sMinVol.IsEmpty() == TRUE))
	//{
	//	MessageBox("Invalid input for min/max voltage!",NULL,MB_ICONERROR);
	//	return;
	//}
	//sscanf_s(sMinVol,"%d",&m_nMin);
	//sscanf_s(sMaxVol,"%d",&m_nMax);
}
#endif

