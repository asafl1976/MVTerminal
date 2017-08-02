// ODPGCtrlRWRegModelessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MarvellDeviceSysProfilerHwModuleModelessDlg.h"
#include "MarvellDeviceSysProfilerHwModule.h"
#include "ProfilerEventManager.h"
#include "HwModuleCtrlI2CUnit.h"
#include "SUB20HwModuleCtrlI2CUnitDrv.h"
#include "libsub.h"

UINT UpdateGraphsThread(LPVOID pParam);

// CMarvellDeviceHwModuleModelessDlg dialog
int CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos = 0;

CMarvellDeviceSysProfilerHwModuleModelessDlg::CMarvellDeviceSysProfilerHwModuleModelessDlg(CWnd* pParent /*=NULL*/)
	: CMarvellDeviceSysProfilerHwModuleBasicDlg(CMarvellDeviceSysProfilerHwModuleModelessDlg::IDD, pParent)
{
	m_nSampleRate        = 1;
	m_nGridSize          = 11;
	m_nSampleInterval    = 4;
	m_bUpdateGraph       = true;
	m_bShowGrid          = true;
	m_sMeasureUnits	     = "";

	m_nSampleArray1Index = 0;
	m_nSampleArray2Index = 0;
	m_nSampleArray3Index = 0;
	m_nSampleArray4Index = 0;

	m_bSampleArray1Overlaped = false;
	m_bSampleArray2Overlaped = false;
	m_bSampleArray3Overlaped = false;
	m_bSampleArray4Overlaped = false;

	m_nMinSample1 = 1000000;
	m_nMaxSample1 = 0;
	m_nMinSample2 = 1000000;
	m_nMaxSample2 = 0;
	m_nMinSample3 = 1000000;
	m_nMaxSample3 = 0;
	m_nMinSample4 = 1000000;
	m_nMaxSample4 = 0;

	memset(m_sampleArray1,0,SYS_PROFILER_SAMPLE_ARRAY_SIZE);
	memset(m_sampleArray2,0,SYS_PROFILER_SAMPLE_ARRAY_SIZE);
	memset(m_sampleArray3,0,SYS_PROFILER_SAMPLE_ARRAY_SIZE);
	memset(m_sampleArray4,0,SYS_PROFILER_SAMPLE_ARRAY_SIZE);

	// Load settings from file
	if(LoadSettingsFromFile() == FALSE) 
	{		
		m_bLoadFileOnStartup = false;
	}
}

CMarvellDeviceSysProfilerHwModuleModelessDlg::~CMarvellDeviceSysProfilerHwModuleModelessDlg()
{
}
BOOL CMarvellDeviceSysProfilerHwModuleModelessDlg::LoadSettingsFromFile()
{
	CString			sLine;
	CStdioFile		cfgFile;

	if(cfgFile.Open(m_sAppDataPath +  "\\sysProfilerSettings.ini", CFile::modeRead | CFile::typeText ) == FALSE) 
		return FALSE;

	cfgFile.ReadString(sLine);
	if(sLine.Find("=TRUE") != -1)
	{
		m_bLoadFileOnStartup = true;
		cfgFile.ReadString(sLine);
		m_sStartupFilePath = sLine;
	}
	if(sLine.Find("=FALSE") != -1)
	{
		m_bLoadFileOnStartup = false;
	}
	cfgFile.Close();
	return TRUE;
}

BOOL CMarvellDeviceSysProfilerHwModuleModelessDlg::SaveSettingsToFile()
{
	CStdioFile  cfgFile;

	// Load settings from file
	if(cfgFile.Open(m_sAppDataPath +  "\\sysProfilerSettings.ini", 
		              CFile::modeCreate | CFile::modeWrite | CFile::typeText ) != TRUE) 
	{
		return FALSE;
	}

	if(m_bLoadFileOnStartup == true)
		cfgFile.WriteString("LoadFileOnStartup=TRUE\n");
	else
		cfgFile.WriteString("LoadFileOnStartup=FALSE\n");

	cfgFile.WriteString(m_sStartupFilePath + "\n");

	cfgFile.Close();
	return TRUE; 
}


void CMarvellDeviceSysProfilerHwModuleModelessDlg::DoDataExchange(CDataExchange* pDX)
{
	CMarvellDeviceSysProfilerHwModuleBasicDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMarvellDeviceSysProfilerHwModuleModelessDlg, CMarvellDeviceSysProfilerHwModuleBasicDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

void  CMarvellDeviceSysProfilerHwModuleModelessDlg::AlignWindow()
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
		nXPos += CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos;
        nYPos += CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos;
		CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos += 20;
		if(CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos == 220)
			CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos = 0;

	    ::SetWindowPos(m_hWnd, HWND_TOP, nXPos, nYPos, 
          rectDlg.Width(), rectDlg.Height(), SWP_NOCOPYBITS);
    }
}

BOOL CMarvellDeviceSysProfilerHwModuleModelessDlg::OnInitDialog()
{
	CMarvellDeviceSysProfilerHwModuleBasicDlg::OnInitDialog();

	if(m_pMarvellDeviceSysProfilerHwModule != NULL)
	{
		SetWindowText(m_pMarvellDeviceSysProfilerHwModule->GetDisplayName());
	}
	else
	{
		AfxMessageBox("m_pMarvellDeviceSysProfilerHwModule = NULL");
	}
	m_loadEventsBtn.SetTheme(xtpButtonThemeOffice2000);

	UpdateGraphs();
     // Create thread
	CProfilerEventManager::GetInstance().RegisterObserver(this);
	CProfilerEventManager::GetInstance().SetHwModuleItem(m_pMarvellDeviceSysProfilerHwModule);

	CheckDlgButton(IDC_RELOAD_LAST_EVENTS_CHECK,m_bLoadFileOnStartup);
	if(m_bLoadFileOnStartup == true)
	{
		CheckDlgButton(IDC_RELOAD_LAST_EVENTS_CHECK,true);
		LoadEvents(m_sStartupFilePath);
	}

	//Set I2C unit to 200kHZ
	CHwModuleCtrlI2CUnit* pI2CUnit = (CHwModuleCtrlI2CUnit*)m_pMarvellDeviceSysProfilerHwModule->GetParentUnit();
	int nFreq = pI2CUnit->GetFrequency();
	if(nFreq < 140000)
	{	
		pI2CUnit->SetFrequency(150000);
		if(pI2CUnit->GetFrequency() < 140000)
		{
			pI2CUnit->SetFrequency(100000);
			CString s;
			s.Format("%d",nFreq);
			AfxMessageBox("Attempt to modify I2C unit frequency to 200Khz failed... falling back to 100Khz ",MB_ICONASTERISK);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnDestroy()
{
	CMarvellDeviceSysProfilerHwModuleBasicDlg::OnDestroy();
}


void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnCancel()
{
	SaveSettingsToFile();
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);
	CProfilerEventManager::GetInstance().UnRegisterObserver(this);
	DestroyWindow();
}


void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnStart()
{	
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);

	m_bUpdateGraph = true;
	m_startStopBtn.SetWindowText("Pause");
	m_loadEventsBtn.EnableWindow(FALSE);
	m_cancelBtn.EnableWindow(FALSE);

	BOOL bEnable = FALSE;
	UINT nMenuf = bEnable ? (MF_BYCOMMAND) : (MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu)
	{
		pSysMenu->EnableMenuItem(SC_CLOSE, nMenuf);
	}

	m_pThread = AfxBeginThread(UpdateGraphsThread, (LPVOID)this,THREAD_PRIORITY_NORMAL);
	//m_pThread->m_bAutoDelete = FALSE;

}
void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnPause()
{
	m_bUpdateGraph = false;
	m_startStopBtn.SetWindowText("Pausing...");
	m_startStopBtn.EnableWindow(FALSE);
	m_startStopBtn.RedrawWindow();
	
	int nRetry = 10;
	DWORD dwExitCode = STILL_ACTIVE;
	while( (dwExitCode == STILL_ACTIVE) && (nRetry-- > 0) )
	{
		GetExitCodeThread(m_pThread->m_hThread, &dwExitCode);
		//Sleep(50);
	}
	if(nRetry == 0)
	{
		m_startStopBtn.SetWindowText("Pause");
		AfxMessageBox("Unable to pause profiler",MB_ICONASTERISK);
	}
	else
	{
		m_startStopBtn.SetWindowText("Start");
		m_loadEventsBtn.EnableWindow(TRUE);
		m_cancelBtn.EnableWindow(TRUE);

		BOOL bEnable = TRUE;
		UINT nMenuf = bEnable ? (MF_BYCOMMAND) : (MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if(pSysMenu)
		{
			pSysMenu->EnableMenuItem(SC_CLOSE, nMenuf);
		}
	}
	m_startStopBtn.EnableWindow(TRUE);
}

void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnBnClickedOk()
{
#if 0
	//int sub_i2c_hs_rw( sub_handle hndl, int mcode, int count, struct sub_i2c_hs_xfer* px ); 
	CHwModuleCtrlI2CUnit* pI2CUnit = (CHwModuleCtrlI2CUnit*)m_pMarvellDeviceSysProfilerHwModule->GetParentUnit();
	CSUB20HwModuleCtrlI2CUnitDrv* pDrv = (CSUB20HwModuleCtrlI2CUnitDrv*)pI2CUnit->GetDriver();

														   //d0 02 03 28
	struct sub_i2c_hs_xfer xfer[2] = { 
		                               { 0x64, 0, 4, 0, "\xd0\x02\x03\x28", 0 },
									   { 0x64, 0, 4, 0, "\x11\x22\x33\x44", 0 }									   
									 };

	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_SELECT,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);
	if(pDrv->HighSpeedRW(xfer,2) == FALSE)
	{
		AfxMessageBox("HS error");
	}
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);

	//pDrv->Hi
	return;
#endif

	CString sBtnText;
	m_startStopBtn.GetWindowText(sBtnText);
	if(sBtnText == "Start")
	{
		OnStart();
	}
	if(sBtnText == "Pause")
	{
		OnPause();

		//Deselect only when dlg dies
		//::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_NOTIFY_DESELECT,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);
	}
}




UINT UpdateGraphsThread(LPVOID pParam)
{
	CMarvellDeviceSysProfilerHwModuleModelessDlg *phObjectHandle = static_cast<CMarvellDeviceSysProfilerHwModuleModelessDlg *>(pParam);

	//System init (typically will be exected once)
	CProfilerEventManager::GetInstance().SystemInit();
	CProfilerEventManager::GetInstance().InitTimeStamp();

	while(phObjectHandle->m_bUpdateGraph)
	{
		if(phObjectHandle->UpdateUIGraphData() == FALSE)
		{
			break;
		}
	}
	return 1;
}

void CMarvellDeviceSysProfilerHwModuleModelessDlg::PostNcDestroy()
{
	CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos -= 20;
	if(CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos < 0)
		CMarvellDeviceSysProfilerHwModuleModelessDlg::m_nWinPos = 0;

	//Notify owner (the TreeView control) that dlg is destroyed
	::SendMessage(GetOwner()->m_hWnd,WM_HW_MODULE_ITEM_MODELESS_DLG_DESTROYED,(WPARAM)m_pMarvellDeviceSysProfilerHwModule,NULL);

	delete this;
}

// CODPGHwModuleModelessDlg message handlers

void  CMarvellDeviceSysProfilerHwModuleModelessDlg::SetHwModuleItem(CHwModuleItem *pItem)
{
	m_pMarvellDeviceSysProfilerHwModule = dynamic_cast<CMarvellDeviceSysProfilerHwModule*>(pItem);
}


BOOL  CMarvellDeviceSysProfilerHwModuleModelessDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CMarvellDeviceSysProfilerHwModuleModelessDlg::IDD,pParentWnd);
}

CBasicItemDlg *CMarvellDeviceSysProfilerHwModuleModelessDlg::Clone()
{
	return new CMarvellDeviceSysProfilerHwModuleModelessDlg();
}

void CMarvellDeviceSysProfilerHwModuleModelessDlg::UpdateGraph(CSysProfilerEvent* pEvent, int nGraphIndex,double fEventSampleData, double fSampleRate)
{
	CSysProfilerTreeView *pTreeView;
	C2DPushGraph         *pGraph;
	int                   nLine,nGraphStaticID,*pSampleArrayIndex;
	UINT32				  *pSampleArray;
	bool				  *pSampleArrayOverlaped;
	
	switch(nGraphIndex)
	{
	case 1:
		pTreeView = &m_eventsTreeView1;
		pGraph = &m_PushGraph1;
		nLine = PUSHGRAPH1_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH1_STATIC;
		pSampleArray = m_sampleArray1;
		pSampleArrayIndex = &m_nSampleArray1Index;
		pSampleArrayOverlaped = &m_bSampleArray1Overlaped;
		break;
	case 2:
		pTreeView = &m_eventsTreeView2;
		pGraph = &m_PushGraph2;
		nLine = PUSHGRAPH2_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH2_STATIC;
		pSampleArray = m_sampleArray2;
		pSampleArrayIndex = &m_nSampleArray2Index;
		pSampleArrayOverlaped = &m_bSampleArray2Overlaped;
		break;
	case 3:
		pTreeView = &m_eventsTreeView3;
		pGraph = &m_PushGraph3;
		nLine = PUSHGRAPH3_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH3_STATIC;
		pSampleArray = m_sampleArray3;
		pSampleArrayIndex = &m_nSampleArray3Index;
		pSampleArrayOverlaped = &m_bSampleArray3Overlaped;
		break;
	case 4:
		pTreeView = &m_eventsTreeView4;
		pGraph = &m_PushGraph4;
		nLine = PUSHGRAPH4_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH4_STATIC;
		pSampleArray = m_sampleArray4;
		pSampleArrayIndex = &m_nSampleArray4Index;
		pSampleArrayOverlaped = &m_bSampleArray4Overlaped;
		break;
	default:
		return;
	}
	CString sSampleRate;
	CString sSampleData;
	CString sMinMax;
	CString sUnits = pEvent->GetUnits();
	CString sTitle = pEvent->GetName();

	sMinMax.Format("Min: %0.1f%s  |  Max: %0.1f%s",pEvent->GetMinSampleData(),sUnits,pEvent->GetMaxSampleData(),sUnits);
	sSampleRate.Format("%0.2fms",fSampleRate);
	sSampleData.Format("%0.1f%s",(float)fEventSampleData,sUnits);

	pGraph->SetLineWidth((pEvent->GetEnableTwoDotLine() == TRUE)? 2:1);
	pGraph->Push((int)fEventSampleData,nGraphIndex,pEvent->GetGraphLineColor());
	pSampleArray[(*pSampleArrayIndex)] = (UINT32)fEventSampleData;
	(*pSampleArrayIndex) = (*pSampleArrayIndex) + 1;
	if((*pSampleArrayIndex) == SYS_PROFILER_SAMPLE_ARRAY_SIZE)
	{
		(*pSampleArrayOverlaped) = true;
		(*pSampleArrayIndex) = 0;
	}
	pGraph->Update();

	sTitle = sTitle + "  |  " + sSampleData + "  |  " + sMinMax + "  |  " +  sSampleRate;
	SetDlgItemText(nGraphStaticID,sTitle);
}

BOOL CMarvellDeviceSysProfilerHwModuleModelessDlg::UpdateUIGraphData()
{
	try
	{
		double fSampleRateMS = CProfilerEventManager::GetInstance().GetTimeStamp();

		//Execute something before all events
		CProfilerEventManager::GetInstance().OnEventsBegin();
		//Loop all active events and push data to graphs
		vector<CSysProfilerEvent*>::iterator iter = m_activeEventsVec.begin(), endIter = m_activeEventsVec.end();
		for(; iter != endIter; iter++)
		{
			if((*iter)->m_nRC == 0)
				continue;

			double fEventSampleData = (*iter)->GetData();
			if(m_eventsTreeView1.GetSelectedEvent() == (*iter))
			{
				UpdateGraph((*iter),1,fEventSampleData,fSampleRateMS);
			}
			if(m_eventsTreeView2.GetSelectedEvent() == (*iter))
			{
				UpdateGraph((*iter),2,fEventSampleData,fSampleRateMS);
			}
			if(m_eventsTreeView3.GetSelectedEvent() == (*iter))
			{
				UpdateGraph((*iter),3,fEventSampleData,fSampleRateMS);
			}
			if(m_eventsTreeView4.GetSelectedEvent() == (*iter))
			{
				UpdateGraph((*iter),4,fEventSampleData,fSampleRateMS);
			}
		}
		//Execute something after all events
		CProfilerEventManager::GetInstance().OnEventsEnd();
	}
	catch(string s)
	{
		AfxMessageBox(s.c_str());
		return FALSE;
	}

	return TRUE;
}

void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnSysProfilerModelAddedEvent(CSysProfilerEvent *pEvent)
{
	AddEvent(pEvent);
	m_eventsTreeView1.AddEvent(pEvent);
	m_eventsTreeView2.AddEvent(pEvent);
	m_eventsTreeView3.AddEvent(pEvent);
	m_eventsTreeView4.AddEvent(pEvent);
}

void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnSysProfilerModelAddedLogLine(CString sLogLine)
{
	if(m_pLogDlg != NULL)
		m_pLogDlg->AddLogLine(sLogLine);
}
void CMarvellDeviceSysProfilerHwModuleModelessDlg::OnSysProfilerModelRemoveAllEvents()
{
	m_activeEventsVec.clear();
	m_eventsTreeView1.RemoveAllEvents();
	m_eventsTreeView2.RemoveAllEvents();
	m_eventsTreeView3.RemoveAllEvents();
	m_eventsTreeView4.RemoveAllEvents();
}
