#pragma once
#include "afxwin.h"
#include "BasicItemDlg.h"
#include "MarvellDeviceSysProfilerHwModule.h"
#include "2DPushGraph.h"
#include "SysProfilerTreeView.h"
#include "SysProfilerObserver.h"
#include "SysProfilerLogDlg.h"

#define PUSHGRAPH1_LINE   1
#define PUSHGRAPH2_LINE   2
#define PUSHGRAPH3_LINE   3
#define PUSHGRAPH4_LINE   4
#define SYS_PROFILER_SAMPLE_ARRAY_SIZE 200

// MarvellDeviceSysProfilerHwModuleBasicDlg dialog

class CMarvellDeviceSysProfilerHwModuleBasicDlg : public CBasicItemDlg 
{
protected:
	C2DPushGraph		 m_PushGraph1;
	C2DPushGraph		 m_PushGraph2;
	C2DPushGraph		 m_PushGraph3;
	C2DPushGraph		 m_PushGraph4;
	COLORREF			 m_minMaxLabelColor;

	CSysProfilerTreeView m_eventsTreeView1;
	CSysProfilerTreeView m_eventsTreeView2;
	CSysProfilerTreeView m_eventsTreeView3;
	CSysProfilerTreeView m_eventsTreeView4;

	bool				m_bLoadFileOnStartup;
	CString				m_sStartupFilePath;
	CString				m_sAppDataPath;

	int					 m_nGridSize;
	int					 m_nSampleInterval;
	int	    		     m_nMin;
	int     			 m_nMax;
	CString				 m_sMeasureUnits; 
	CString				 m_sName;
	bool				 m_bShowGrid;
	CSysProfilerLogDlg*  m_pLogDlg;

	UINT32				 m_sampleArray1[SYS_PROFILER_SAMPLE_ARRAY_SIZE];
	int					 m_nSampleArray1Index;
	bool				 m_bSampleArray1Overlaped;
	int 				 m_nMinSample1;
	int 				 m_nMaxSample1;

	UINT32				 m_sampleArray2[SYS_PROFILER_SAMPLE_ARRAY_SIZE];
	int					 m_nSampleArray2Index;
	bool				 m_bSampleArray2Overlaped;
	int 				 m_nMinSample2;
	int 				 m_nMaxSample2;

	UINT32				 m_sampleArray3[SYS_PROFILER_SAMPLE_ARRAY_SIZE];
	int					 m_nSampleArray3Index;
	bool				 m_bSampleArray3Overlaped;
	int 				 m_nMinSample3;
	int 				 m_nMaxSample3;

	UINT32				 m_sampleArray4[SYS_PROFILER_SAMPLE_ARRAY_SIZE];
	int					 m_nSampleArray4Index;
	bool				 m_bSampleArray4Overlaped;
	int 				 m_nMinSample4;
	int 				 m_nMaxSample4;

	vector<CSysProfilerEvent*>		    m_activeEventsVec;
	virtual HTREEITEM	    GetMouseClickedItem(CSysProfilerTreeView* pTreeView, NMHDR *pNMHDR, LRESULT *pResult);
	virtual void			AddEvent(CSysProfilerEvent* pEvent);
	virtual void			AddActiveEvent(CSysProfilerEvent* pEvent);
	virtual void		    RemoveActiveEvent(CSysProfilerEvent* pEvent);
	virtual LRESULT			OnSysProfilerLogModelessDlgDestroyed(WPARAM a, LPARAM b);
	virtual void			HandleEventSelection(CSysProfilerTreeView* pTreeView, int nGraphIndex);
	virtual void			ShowEvents();
	virtual void		    LoadEvents(CString sFile);
public:
	int					 m_nSampleRate;
	bool				 m_bUpdateGraph;
	CXTPButton			 m_startStopBtn;
	CXTPButton		     m_loadEventsBtn;
	CXTPButton           m_optionBtn;
	CXTPButton			 m_cancelBtn;


public:
	CMarvellDeviceSysProfilerHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CMarvellDeviceSysProfilerHwModuleBasicDlg();


// Dialog Data
	enum { IDD = IDD_MARVELL_DEVICE_SYSTEM_PROFILER_HWMODULE_DLG };


protected:
	virtual void 			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL 			OnInitDialog();

	afx_msg void 			OnBnClickedCancel();
    afx_msg void		    OnBnClickedOk();
	virtual void			InitEventGraph(CSysProfilerEvent* pEvent, int nPushGraphIndex);
	virtual void			ResetEventGraph(int nPushGraphIndex);
	virtual void			OpenLogView();
	virtual void			DumpToExcell(int nGraphIndex);
	virtual void		    HandleMenuSelection(UINT nMenuIndex);
	afx_msg void		    UpdateGraphs();
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSysProfilerLoadEventsBtn();
	afx_msg void OnNMClickMarvellDeviceSpTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickMarvellDeviceSpTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickMarvellDeviceSpTree3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickMarvellDeviceSpTree4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSysProfilerOptionButton();
	afx_msg void OnBnClickedReloadLastEventsCheck();
	afx_msg void OnNMRClickMarvellDeviceSpTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnSelchangingMarvellDeviceSpTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickMarvellDeviceSpTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickMarvellDeviceSpTree3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickMarvellDeviceSpTree4(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
};
