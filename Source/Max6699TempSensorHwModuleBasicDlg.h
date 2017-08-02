#pragma once

#include "BasicItemDlg.h"
#include "afxwin.h"
#include "2DPushGraph.h"
#include "2DPushGraph.h"
#include "resource.h"
#include "ColorStaticST.h"

// CMax6699TempSensorHwModuleBasicDlg dialog
#define SAMPLE_ARRAY_SIZE 1000
#define TEMP_PUSHGRAPH_VAL_SAMPLE_LINE          0

class CMax6699TempSensorHwModuleBasicDlg : public CBasicItemDlg
{
protected:
	CXTPButton			m_cancelBtn;
	C2DPushGraph		m_PushGraph;
	COLORREF			m_sampleValuelineColor;
	CColorStaticST		m_tempTextLabel;
	CWinThread*         m_pUpdateGraphThread;
	CFont				m_font;
	COLORREF            colorMap[70];

	int                 m_sampleArray[SAMPLE_ARRAY_SIZE];
	int                 m_nSampleArrayIndex;

public:
	CMax6699TempSensorHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	virtual ~CMax6699TempSensorHwModuleBasicDlg();


    // Dialog Data
	enum { IDD = IDD_TEMPERATURE_MONITOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL		OnInitDialog();

	virtual void		UpdateGraph();
	virtual void		StopUpdateThread()  = 0;
	virtual void		StartUpdateThread() = 0;

	DECLARE_MESSAGE_MAP()
public:
};
