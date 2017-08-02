#pragma once
#include "2DPushGraph.h"
#include "resource.h"
#include "BasicItemDlg.h"


#define SAMPLE_ARRAY_SIZE 1000
#define VOL_PUSHGRAPH_VAL_SAMPLE_LINE          0
#define VOL_PUSHGRAPH_AV_VAL_SAMPLE_LINE	   1

class CSUB20HwModuleCtrlADCUnitVCItemBasicDlg : public CBasicItemDlg
{

public:

	CXTPButton			m_resetStatBtn;
	CXTPButton			m_okBtn;
	CXTPButton			m_cancelBtn;
	CXTPButton			m_PauseBtn;
	CXTPButton			m_XLSnapShotBtn;
	CSliderCtrl			m_gridSizeSlider;
	C2DPushGraph		m_PushGraph;
	CSliderCtrl			m_GraphSampleRateSlider;

	int					m_nGridSize;
	int					m_nSampleInterval;
	int                 m_sampleArray[SAMPLE_ARRAY_SIZE];
	int  				m_nSampleCount;
	bool				m_bArrayIsFull;
	int                 m_nSampleArrayIndex;
	int	    		    m_nMin;
	int     			m_nMax;
	int	    		    m_nAmp;
	int     			m_nOffset;
	CSliderCtrl			m_sampleIntervalSlider;
	float  				m_nAverage;
	int 				m_nMinSample;
	int 				m_nMaxSample;
	CString				m_sMeasureUnits; 
	CString				m_sName;
	bool				m_bUpdateGraph;
	int					m_nSampleRate;
	bool				m_bShowAvInGraph;
	bool				m_bShowGrid;
	COLORREF			m_sampleValuelineColor;
	COLORREF			m_AvsampleValuelineColor;
	CWinThread*         m_pUpdateGraphThread;

	CSliderCtrl         m_ampSlider;
	int				    m_ampSliderVal;

	CSliderCtrl      	m_offsetSlider;
	int				    m_offsetSliderVal;

protected:
	CSUB20HwModuleCtrlADCUnitVCItemBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CSUB20HwModuleCtrlADCUnitVCItemBasicDlg(void);

	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL  		OnInitDialog();
	//afx_msg void  		OnBnClickedOk();
	afx_msg void  		OnBnClickedHwModuleAdcModDlgPauseBtn();
	afx_msg void  		OnNMReleasedcaptureHwModuleAdcModDlgVolSampleRateSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  		OnBnClickedHwModuleAdcModDlgVolStatAvrgCheck();
	afx_msg void  		OnBnClickedHwModuleAdcModDlgResetBtn();
	afx_msg void  		OnBnClickedHwModuleAdcModDlgShowGridCheck();
	afx_msg void  		OnNMReleasedcaptureHwModuleAdcModDlgVolSampleIntervalSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void  		OnNMReleasedcaptureHwModuleAdcModDlgVolGridSizeSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnBnClickedHwModuleAdcModDlgXlSnapshotBtn();

	virtual void		UpdateGraph();
	virtual void		StopUpdateThread()  = 0;
	virtual void		StartUpdateThread() = 0;

	DECLARE_MESSAGE_MAP()
};
