// ODPGControlRWRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "MarvellDeviceSysProfilerHwModuleBasicDlg.h"
#include "MarvellDeviceHwModule.h"
#include "ProfilerEventManager.h"
#include "SysProfilerLogDlg.h"
#include "XLEzAutomation.h"

#define ID_SYS_PROFILER_OPEN_LOG_VIEW	       (WM_USER+200)
#define ID_SYS_PROFILER_TREE1_EXPORT_TO_EXCELL (WM_USER+201)
#define ID_SYS_PROFILER_TREE2_EXPORT_TO_EXCELL (WM_USER+202)
#define ID_SYS_PROFILER_TREE3_EXPORT_TO_EXCELL (WM_USER+203)
#define ID_SYS_PROFILER_TREE4_EXPORT_TO_EXCELL (WM_USER+204)

#define ID_SYS_PROFILER_TREE1_USE_ALT_COLOR_ITEM	  (WM_USER+205)
#define ID_SYS_PROFILER_TREE2_USE_ALT_COLOR_ITEM	  (WM_USER+206)
#define ID_SYS_PROFILER_TREE3_USE_ALT_COLOR_ITEM	  (WM_USER+207)
#define ID_SYS_PROFILER_TREE4_USE_ALT_COLOR_ITEM	  (WM_USER+208)

#define ID_SYS_PROFILER_TREE1_INC_LINE_WIDTH	      (WM_USER+211)
#define ID_SYS_PROFILER_TREE2_INC_LINE_WIDTH	      (WM_USER+212)
#define ID_SYS_PROFILER_TREE3_INC_LINE_WIDTH	      (WM_USER+213)
#define ID_SYS_PROFILER_TREE4_INC_LINE_WIDTH	      (WM_USER+214)

#define ID_SYS_PROFILER_TREE1_RESET_MIN_MAX	      (WM_USER+215)
#define ID_SYS_PROFILER_TREE2_RESET_MIN_MAX	      (WM_USER+216)
#define ID_SYS_PROFILER_TREE3_RESET_MIN_MAX	      (WM_USER+217)
#define ID_SYS_PROFILER_TREE4_RESET_MIN_MAX	      (WM_USER+218)


// MarvellDeviceSysProfilerHwModuleBasicDlg dialog

CMarvellDeviceSysProfilerHwModuleBasicDlg::CMarvellDeviceSysProfilerHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CBasicItemDlg(nIDTemplate, pParentWnd)
{
	m_pLogDlg            = NULL;
	m_bLoadFileOnStartup = false;
	// Set installation dir
	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
}

CMarvellDeviceSysProfilerHwModuleBasicDlg::~CMarvellDeviceSysProfilerHwModuleBasicDlg()
{
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_startStopBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_SYS_PROFILER_LOAD_EVENTS_BTN, m_loadEventsBtn);

	DDX_Control(pDX, IDC_MARVELL_DEVICE_SP_TREE1, m_eventsTreeView1);
	DDX_Control(pDX, IDC_MARVELL_DEVICE_SP_TREE2, m_eventsTreeView2);
	DDX_Control(pDX, IDC_MARVELL_DEVICE_SP_TREE3, m_eventsTreeView3);
	DDX_Control(pDX, IDC_MARVELL_DEVICE_SP_TREE4, m_eventsTreeView4);
	DDX_Control(pDX, IDC_SYS_PROFILER_OPTION_BUTTON, m_optionBtn);
}


BEGIN_MESSAGE_MAP(CMarvellDeviceSysProfilerHwModuleBasicDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SYS_PROFILER_LOAD_EVENTS_BTN, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedSysProfilerLoadEventsBtn)
	ON_NOTIFY(NM_CLICK, IDC_MARVELL_DEVICE_SP_TREE1, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree1)
	ON_NOTIFY(NM_CLICK, IDC_MARVELL_DEVICE_SP_TREE2, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree2)
	ON_NOTIFY(NM_CLICK, IDC_MARVELL_DEVICE_SP_TREE3, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree3)
	ON_NOTIFY(NM_CLICK, IDC_MARVELL_DEVICE_SP_TREE4, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree4)
	ON_BN_CLICKED(IDC_SYS_PROFILER_OPTION_BUTTON, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedSysProfilerOptionButton)
	ON_MESSAGE(WM_SYS_PROFILER_MODELESS_DLG_DESTROYED, OnSysProfilerLogModelessDlgDestroyed)

	ON_BN_CLICKED(IDC_RELOAD_LAST_EVENTS_CHECK, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedReloadLastEventsCheck)
	ON_NOTIFY(NM_RCLICK, IDC_MARVELL_DEVICE_SP_TREE1, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree1)
	ON_NOTIFY(NM_RCLICK, IDC_MARVELL_DEVICE_SP_TREE2, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree2)
	ON_NOTIFY(NM_RCLICK, IDC_MARVELL_DEVICE_SP_TREE3, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree3)
	ON_NOTIFY(NM_RCLICK, IDC_MARVELL_DEVICE_SP_TREE4, &CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree4)
END_MESSAGE_MAP()

// CMarvellDeviceHwModuleBasicDlg message handlers

BOOL CMarvellDeviceSysProfilerHwModuleBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	AlignWindow();

	m_eventsTreeView1.CreateImageList();
	m_eventsTreeView2.CreateImageList();
	m_eventsTreeView3.CreateImageList();
	m_eventsTreeView4.CreateImageList();

	m_startStopBtn.SetTheme(xtpButtonThemeOffice2000);
	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_optionBtn.SetTheme(xtpButtonThemeOffice2000);
		
	m_PushGraph1.CreateFromStatic(IDC_MARVELL_DEVICE_SP_GRAPH1, this);	
	m_PushGraph1.ShowGrid( m_bShowGrid);
	m_PushGraph1.ShowLabels( true);
	m_PushGraph1.SetInterval( m_nSampleInterval );
	m_PushGraph1.SetGridSize( m_nGridSize );

	m_PushGraph2.CreateFromStatic(IDC_MARVELL_DEVICE_SP_GRAPH2, this);	
	m_PushGraph2.ShowGrid( m_bShowGrid);
	m_PushGraph2.ShowLabels( true);
	m_PushGraph2.SetInterval( m_nSampleInterval );
	m_PushGraph2.SetGridSize( m_nGridSize );

	m_PushGraph3.CreateFromStatic(IDC_MARVELL_DEVICE_SP_GRAPH3, this);	
	m_PushGraph3.ShowGrid( m_bShowGrid);
	m_PushGraph3.ShowLabels( true);
	m_PushGraph3.SetInterval( m_nSampleInterval );
	m_PushGraph3.SetGridSize( m_nGridSize );

	m_PushGraph4.CreateFromStatic(IDC_MARVELL_DEVICE_SP_GRAPH4, this);	
	m_PushGraph4.ShowGrid( m_bShowGrid);
	m_PushGraph4.ShowLabels( true);
	m_PushGraph4.SetInterval( m_nSampleInterval );
	m_PushGraph4.SetGridSize( m_nGridSize );

	m_startStopBtn.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::UpdateGraphs()
{
	//m_PushGraph.Update();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedOk()
{
}


void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::LoadEvents(CString sFile)
{
	ResetEventGraph(1);
	ResetEventGraph(2);
	ResetEventGraph(3);
	ResetEventGraph(4);
	CProfilerEventManager::GetInstance().RemoveAllEvents();
	CProfilerEventManager::GetInstance().LoadConfigFile(sFile);
	m_sStartupFilePath = sFile;
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedSysProfilerLoadEventsBtn()
{
	char strFilter[] = { "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||" };
	CFileDialog fileDlg(TRUE, ".xml", "", OFN_HIDEREADONLY, strFilter);

	if(fileDlg.DoModal() == IDOK)
	{
		LoadEvents(fileDlg.GetPathName());
	}
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::ShowEvents()
{
	vector<CSysProfilerEvent*>::iterator iter, endIter;

	iter = m_activeEventsVec.begin();
	endIter = m_activeEventsVec.end();

	CString s,rc;

	for(; iter != endIter; iter++)
	{
		rc.Format("RC=%d",(*iter)->m_nRC);
		s = s + (*iter)->GetName() + " " + rc + " | ";
	}
	AfxMessageBox(s);
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::AddEvent(CSysProfilerEvent* pEvent)
{
	vector<CSysProfilerEvent*>::iterator iter, endIter;
	iter = m_activeEventsVec.begin();
	endIter = m_activeEventsVec.end();

	for(; iter != endIter; iter++)
	{
		if((*iter) == pEvent)
		{
			return;
		}
	}
	m_activeEventsVec.push_back(pEvent);
}


void CMarvellDeviceSysProfilerHwModuleBasicDlg::AddActiveEvent(CSysProfilerEvent* pEvent)
{
	vector<CSysProfilerEvent*>::iterator iter, endIter;
	iter = m_activeEventsVec.begin();
	endIter = m_activeEventsVec.end();

	for(; iter != endIter; iter++)
	{
		if((*iter) == pEvent)
		{
			pEvent->m_nRC++;
			return;
		}
	}
	pEvent->m_nRC++;
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::RemoveActiveEvent(CSysProfilerEvent* pEvent)
{
	vector<CSysProfilerEvent*>::iterator iter, endIter;
	iter = m_activeEventsVec.begin();
	endIter = m_activeEventsVec.end();

	for(; iter != endIter; iter++)
	{
		if((*iter) == pEvent)
		{
			if(pEvent->m_nRC > 0)
				pEvent->m_nRC--;

			break;
		}
	}
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::ResetEventGraph(int nPushGraphIndex)
{
	C2DPushGraph* pGraph = NULL;
	int			  nLine,nGraphStaticID;

	switch(nPushGraphIndex)
	{
	case 1:
		pGraph = &m_PushGraph1;
		nLine = PUSHGRAPH1_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH1_STATIC;
		break;
	case 2:
		pGraph = &m_PushGraph2;
		nLine = PUSHGRAPH2_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH2_STATIC;
		break;
	case 3:
		pGraph = &m_PushGraph3;
		nLine = PUSHGRAPH3_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH3_STATIC;
		break;
	case 4:
		pGraph = &m_PushGraph4;
		nLine = PUSHGRAPH4_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH4_STATIC;
		break;
	default:
		return;
	}

	SetDlgItemText(nGraphStaticID,"");
	pGraph->SetLabelForMax( "" ); 
	pGraph->SetLabelForMin( "" );
	pGraph->RemoveLine(nLine);	 
	pGraph->Update();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::InitEventGraph(CSysProfilerEvent* pEvent,int nPushGraphIndex)
{
	C2DPushGraph* pGraph = NULL;
	int			  nLine,nGraphStaticID;

	switch(nPushGraphIndex)
	{
	case 1:
		pGraph = &m_PushGraph1;
		nLine = PUSHGRAPH1_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH1_STATIC;
		break;
	case 2:
		pGraph = &m_PushGraph2;
		nLine = PUSHGRAPH2_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH2_STATIC;
		break;
	case 3:
		pGraph = &m_PushGraph3;
		nLine = PUSHGRAPH3_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH3_STATIC;
		break;
	case 4:
		pGraph = &m_PushGraph4;
		nLine = PUSHGRAPH4_LINE;
		nGraphStaticID = IDC_EVENT_GRAPH4_STATIC;
		break;
	default:
		return;
	}
		
	CString sTitle = pEvent->GetName();
	SetDlgItemText(nGraphStaticID,sTitle);

	pGraph->SetTextColor( pEvent->GetDefaultColor());
	CString sMaxLabel,sMinLabel;
	sMaxLabel.Format("%d",pEvent->GetMax());
	sMaxLabel = sMaxLabel + pEvent->GetUnits();
	sMinLabel.Format("%d",pEvent->GetMin());
	sMinLabel = sMinLabel + pEvent->GetUnits();
	pGraph->SetLabelForMax( sMaxLabel ); 
	pGraph->SetLabelForMin( sMinLabel );
	pGraph->SetPeekRange( pEvent->GetMin(), pEvent->GetMax());	 
	pGraph->AddLine( nLine, pEvent->GetDefaultColor());
	pGraph->Update();
}


void CMarvellDeviceSysProfilerHwModuleBasicDlg::HandleEventSelection(CSysProfilerTreeView* pTreeView, int nGraphIndex)
{
	// TODO: Add your control notification handler code here
    HTREEITEM item,selItem;
    UINT flags;
    // verify that we have a mouse click in the check box area
    DWORD pos = GetMessagePos();
    CPoint point(LOWORD(pos), HIWORD(pos));
    pTreeView->ScreenToClient(&point);
    item    = pTreeView->HitTest(point, &flags);
	//Get the item that was previously selected
	//selItem = pTreeView->GetSelectedItem();
	CSysProfilerEvent* pCurrGraphEvent = (CSysProfilerEvent*)pTreeView->GetSelectedEvent();
	if(item == pTreeView->GetRootItem())
	{
 		ResetEventGraph(nGraphIndex);
	    if(pCurrGraphEvent != NULL)
	    {
			selItem = pTreeView->GetTreeItem(pCurrGraphEvent);
			pTreeView->SetItemBold(selItem,FALSE);
			pTreeView->SetSelectedEvent(NULL);
			RemoveActiveEvent(pCurrGraphEvent);
		}
		CString sBtnText;
		m_startStopBtn.GetWindowText(sBtnText);
		if((sBtnText == "Pause") && (m_bUpdateGraph == false))
			m_startStopBtn.SetWindowText( "Start");
		m_startStopBtn.EnableWindow(m_activeEventsVec.size() != 0);
		m_bUpdateGraph = (m_activeEventsVec.size() != 0);
		return;
	}
    if(item != NULL)
    {
		CSysProfilerEvent* pGraphEvent = (CSysProfilerEvent*)pTreeView->GetItemData(item);
		if(pCurrGraphEvent != pGraphEvent)
		{
			if(pGraphEvent != NULL)
			{
				//remove current event
				if(pCurrGraphEvent != NULL)
				{
	 				ResetEventGraph(nGraphIndex);
					selItem = pTreeView->GetTreeItem(pCurrGraphEvent);
					pTreeView->SetItemBold(selItem,FALSE);
					pTreeView->SetSelectedEvent(NULL);
					RemoveActiveEvent(pCurrGraphEvent);
				}
				//Set event bold
				pTreeView->SetItemBold(item,TRUE);
				pTreeView->SetSelectedEvent(pGraphEvent);
				InitEventGraph(pGraphEvent,nGraphIndex);
				AddActiveEvent(pGraphEvent);
			}
		}
		CString sBtnText;
		m_startStopBtn.GetWindowText(sBtnText);
		if((sBtnText == "Pause") && (m_bUpdateGraph == false))
			m_startStopBtn.SetWindowText( "Start");
		m_startStopBtn.EnableWindow(m_activeEventsVec.size() != 0);
		m_bUpdateGraph = (m_activeEventsVec.size() != 0);
    }
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleEventSelection(&m_eventsTreeView1,1);
   *pResult = 0;
   	//ShowEvents();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleEventSelection(&m_eventsTreeView2,2);
   *pResult = 0;
	//ShowEvents();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree3(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleEventSelection(&m_eventsTreeView3,3);
   *pResult = 0;
	//ShowEvents();
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMClickMarvellDeviceSpTree4(NMHDR *pNMHDR, LRESULT *pResult)
{
	HandleEventSelection(&m_eventsTreeView4,4);
   *pResult = 0;
	//ShowEvents();
}

LRESULT CMarvellDeviceSysProfilerHwModuleBasicDlg::OnSysProfilerLogModelessDlgDestroyed(WPARAM a, LPARAM b) 
{
	if(m_pLogDlg != NULL)
	{
		m_pLogDlg = NULL;
	}
	return 0;
}
void CMarvellDeviceSysProfilerHwModuleBasicDlg::DumpToExcell(int nGraphIndex)
{
	int     *pSampleArrayIndex;
	UINT32	*pSampleArray;
	bool	*pSampleArrayOverlaped;

	switch(nGraphIndex)
	{
	case 1:
		pSampleArray = m_sampleArray1;
		pSampleArrayIndex = &m_nSampleArray1Index;
		pSampleArrayOverlaped = &m_bSampleArray1Overlaped;
		break;
	case 2:
		pSampleArray = m_sampleArray2;
		pSampleArrayIndex = &m_nSampleArray2Index;		
		pSampleArrayOverlaped = &m_bSampleArray2Overlaped;
		break;
	case 3:
		pSampleArray = m_sampleArray3;
		pSampleArrayIndex = &m_nSampleArray3Index;		
		pSampleArrayOverlaped = &m_bSampleArray3Overlaped;
		break;
	case 4:
		pSampleArray = m_sampleArray4;
		pSampleArrayIndex = &m_nSampleArray4Index;		
		pSampleArrayOverlaped = &m_bSampleArray4Overlaped;
		break;
	default:
		return;
	}

	//CXLEzAutomation class constructor starts Excel and creates empty worksheet  
	CXLEzAutomation XL;

	//All data will be stored by szData
	CString szData, sData;
	int		nSampleArrayIndex = (*pSampleArrayIndex);
	UINT32  sampleArray[SYS_PROFILER_SAMPLE_ARRAY_SIZE];

	memset(sampleArray,0,SYS_PROFILER_SAMPLE_ARRAY_SIZE);

	int j = 0;
	//If array was filled at least once..
	if((*pSampleArrayOverlaped) == true)
	{
		//Copy the most recent samples first 
		for(int i = 0; i < (SYS_PROFILER_SAMPLE_ARRAY_SIZE - nSampleArrayIndex); i++)
		{
			sampleArray[j] = pSampleArray[nSampleArrayIndex + i];
			j++;
		}
	}
	for(int i = 0; i < nSampleArrayIndex; i++,j++)
	{
		sampleArray[j] = pSampleArray[i];
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

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OpenLogView()
{
	CWnd*				 pMainWnd;
	
	if(m_pLogDlg == NULL)
	{
		pMainWnd = AfxGetApp()->m_pMainWnd;
		m_pLogDlg = new CSysProfilerLogDlg();

		if(m_pLogDlg == NULL)
			return;

		if(m_pLogDlg->Create(pMainWnd) == FALSE) 
		{ 
			delete m_pLogDlg;
			return;
		} 
		m_pLogDlg->SetOwner(this);
		m_pLogDlg->ShowWindow(SW_SHOW);
		m_pLogDlg->SetForegroundWindow();
		
		CProfilerEventManager::GetInstance().SetEnableLogging(TRUE);
	}
    else /* already open */
    {
		m_pLogDlg->ShowWindow(SW_SHOW);
		if(m_pLogDlg->IsIconic())   // in case the minimize button is enabled
		{
			m_pLogDlg->ShowWindow(SW_RESTORE);
		}
		m_pLogDlg->SetForegroundWindow();
	}
}
void CMarvellDeviceSysProfilerHwModuleBasicDlg::HandleMenuSelection(UINT nMenuIndex)
{	
	CSysProfilerEvent* pEvent;
	HTREEITEM		   hItem = NULL;

	switch(nMenuIndex)
	{
	case ID_SYS_PROFILER_TREE1_USE_ALT_COLOR_ITEM:
		hItem = m_eventsTreeView1.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView1.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetUseAlternateColor(!pEvent->GetUseAlternateColor());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE2_USE_ALT_COLOR_ITEM:
		hItem = m_eventsTreeView2.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView2.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetUseAlternateColor(!pEvent->GetUseAlternateColor());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE3_USE_ALT_COLOR_ITEM:
		hItem = m_eventsTreeView3.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView3.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetUseAlternateColor(!pEvent->GetUseAlternateColor());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE4_USE_ALT_COLOR_ITEM:
		hItem = m_eventsTreeView4.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView4.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetUseAlternateColor(!pEvent->GetUseAlternateColor());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE1_INC_LINE_WIDTH:
		hItem = m_eventsTreeView1.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView1.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetEnableTwoDotLine(!pEvent->GetEnableTwoDotLine());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE2_INC_LINE_WIDTH:
		hItem = m_eventsTreeView2.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView2.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetEnableTwoDotLine(!pEvent->GetEnableTwoDotLine());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE3_INC_LINE_WIDTH:
		hItem = m_eventsTreeView3.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView3.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetEnableTwoDotLine(!pEvent->GetEnableTwoDotLine());
			}
		}
		break;

	case ID_SYS_PROFILER_TREE4_INC_LINE_WIDTH:
		hItem = m_eventsTreeView4.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView4.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->SetEnableTwoDotLine(!pEvent->GetEnableTwoDotLine());
			}
		}
		break;

		
	case ID_SYS_PROFILER_TREE1_RESET_MIN_MAX:
		hItem = m_eventsTreeView1.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView1.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->ResetMinMaxSampleData();
			}
		}
		break;
	case ID_SYS_PROFILER_TREE2_RESET_MIN_MAX:
		hItem = m_eventsTreeView2.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView2.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->ResetMinMaxSampleData();
			}
		}
		break;
	case ID_SYS_PROFILER_TREE3_RESET_MIN_MAX:
		hItem = m_eventsTreeView3.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView3.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->ResetMinMaxSampleData();
			}
		}
		break;
	case ID_SYS_PROFILER_TREE4_RESET_MIN_MAX:
		hItem = m_eventsTreeView4.GetSelectedItem();
		if(hItem != NULL)
		{
			pEvent = (CSysProfilerEvent*)m_eventsTreeView4.GetItemData(hItem);
			if(pEvent != NULL)
			{
				pEvent->ResetMinMaxSampleData();
			}
		}
		break;

	case ID_SYS_PROFILER_TREE1_EXPORT_TO_EXCELL:
		DumpToExcell(1);
		break;
	case ID_SYS_PROFILER_TREE2_EXPORT_TO_EXCELL:
		DumpToExcell(2);
		break;
	case ID_SYS_PROFILER_TREE3_EXPORT_TO_EXCELL:
		DumpToExcell(3);
		break;
	case ID_SYS_PROFILER_TREE4_EXPORT_TO_EXCELL:
		DumpToExcell(4);
		break;

	default:
		break;
	}
}

BOOL CMarvellDeviceSysProfilerHwModuleBasicDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_COMMAND)
	{			
		//The high-order word specifies the notification code if the message is from a control. 
		//If the message is from an accelerator, this value is 1. If the message is from a menu, 
		//this value is zero. 
		//The low-order word specifies the identifier of the menu item, control, or accelerator.
		if(((((UINT)pMsg->wParam)>>16) & 0xFFFF) == 0)
		{
			HandleMenuSelection((UINT)pMsg->wParam & 0xFFFF);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedSysProfilerOptionButton()
{
	OpenLogView();
#if 0
	HTREEITEM	 hItem = NULL;
	CPoint		 point;
	CNewMenu	 itemMenu;
	int			 mMenuItem =0;
	CRect		 rect;
	BOOL		 bSaveCmdEn = 0;

	m_optionBtn.GetWindowRect(rect);

	int x = rect.right ;
	int y = rect.bottom - 25;
    itemMenu.CreatePopupMenu();
    itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_SYS_PROFILER_OPEN_LOG_VIEW ,  "View Log...");
	itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, x , y , this);
#endif

}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnBnClickedReloadLastEventsCheck()
{
	m_bLoadFileOnStartup = !m_bLoadFileOnStartup;	
}

HTREEITEM CMarvellDeviceSysProfilerHwModuleBasicDlg::GetMouseClickedItem(CSysProfilerTreeView* pTreeView, NMHDR *pNMHDR, LRESULT *pResult)
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 pTreeView->ScreenToClient(&point);
	 item = pTreeView->HitTest(point, &uFlags);
	 if(item)
	 { 
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
	 		break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			return item;
		 default:
			break;
	   	}
	 }	
	 *pResult = 0;
	 return NULL;
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;
	 CPoint				  point;
	 CNewMenu			  itemMenu;
	 int				  mMenuItem =0;
 	 CBitmap			  bmpImage;
	 UINT32				  nExportToExcell = MF_GRAYED, nAltColorCheck = MF_CHECKED, nIncreaseLineWidth = MF_CHECKED;

	 if((hItem = GetMouseClickedItem(&m_eventsTreeView1,pNMHDR,pResult)) == NULL)
		 return;

     CSysProfilerEvent* pCurrGraphEvent = (CSysProfilerEvent*)m_eventsTreeView1.GetItemData(hItem);
	 if(pCurrGraphEvent == NULL)
	  	 return;

	 if(pCurrGraphEvent == m_eventsTreeView1.GetSelectedEvent())
	 {
		 nExportToExcell = MF_ENABLED;
	 }

	 if(pCurrGraphEvent->GetUseAlternateColor() == FALSE)
	 {
		 nAltColorCheck = MF_UNCHECKED;
	 }
	 if(pCurrGraphEvent->GetEnableTwoDotLine() == FALSE)
	 {
		 nIncreaseLineWidth = MF_UNCHECKED;
	 }

     GetCursorPos(&point);   
	 itemMenu.CreatePopupMenu();

	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nAltColorCheck, ID_SYS_PROFILER_TREE1_USE_ALT_COLOR_ITEM ,  "Use alternate color");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nIncreaseLineWidth, ID_SYS_PROFILER_TREE1_INC_LINE_WIDTH ,  "Increase line width");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION , ID_SYS_PROFILER_TREE1_RESET_MIN_MAX ,  "Reset Min/Max");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nExportToExcell, ID_SYS_PROFILER_TREE1_EXPORT_TO_EXCELL ,  "Export to Excell...");
	 itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

}


void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;
	 CPoint				  point;
	 CNewMenu			  itemMenu;
	 int				  mMenuItem =0;
 	 CBitmap			  bmpImage;
	 UINT32				  nExportToExcell = MF_GRAYED, nAltColorCheck = MF_CHECKED, nIncreaseLineWidth = MF_CHECKED;

	 if((hItem = GetMouseClickedItem(&m_eventsTreeView2,pNMHDR,pResult)) == NULL)
		 return;

     CSysProfilerEvent* pCurrGraphEvent = (CSysProfilerEvent*)m_eventsTreeView2.GetItemData(hItem);
	 if(pCurrGraphEvent == NULL)
	  	 return;

	 if(pCurrGraphEvent == m_eventsTreeView2.GetSelectedEvent())
	 {
		 nExportToExcell = MF_ENABLED;
	 }

	 if(pCurrGraphEvent->GetUseAlternateColor() == FALSE)
	 {
		 nAltColorCheck = MF_UNCHECKED;
	 }
	 if(pCurrGraphEvent->GetEnableTwoDotLine() == FALSE)
	 {
		 nIncreaseLineWidth = MF_UNCHECKED;
	 }

     GetCursorPos(&point);   
	 itemMenu.CreatePopupMenu();

	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nAltColorCheck, ID_SYS_PROFILER_TREE2_USE_ALT_COLOR_ITEM ,  "Use alternate color");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nIncreaseLineWidth, ID_SYS_PROFILER_TREE2_INC_LINE_WIDTH ,  "Increase line width");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION , ID_SYS_PROFILER_TREE2_RESET_MIN_MAX ,  "Reset Min/Max");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nExportToExcell, ID_SYS_PROFILER_TREE2_EXPORT_TO_EXCELL ,  "Export to Excell...");
	 itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree3(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;
	 CPoint				  point;
	 CNewMenu			  itemMenu;
	 int				  mMenuItem =0;
 	 CBitmap			  bmpImage;
	 UINT32				  nExportToExcell = MF_GRAYED, nAltColorCheck = MF_CHECKED, nIncreaseLineWidth = MF_CHECKED;

	 if((hItem = GetMouseClickedItem(&m_eventsTreeView3,pNMHDR,pResult)) == NULL)
		 return;

     CSysProfilerEvent* pCurrGraphEvent = (CSysProfilerEvent*)m_eventsTreeView3.GetItemData(hItem);
	 if(pCurrGraphEvent == NULL)
	  	 return;

	 if(pCurrGraphEvent == m_eventsTreeView3.GetSelectedEvent())
	 {
		 nExportToExcell = MF_ENABLED;
	 }

	 if(pCurrGraphEvent->GetUseAlternateColor() == FALSE)
	 {
		 nAltColorCheck = MF_UNCHECKED;
	 }

	 if(pCurrGraphEvent->GetEnableTwoDotLine() == FALSE)
	 {
		 nIncreaseLineWidth = MF_UNCHECKED;
	 }

     GetCursorPos(&point);   
	 itemMenu.CreatePopupMenu();

	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nAltColorCheck, ID_SYS_PROFILER_TREE3_USE_ALT_COLOR_ITEM ,  "Use alternate color");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nIncreaseLineWidth, ID_SYS_PROFILER_TREE3_INC_LINE_WIDTH ,  "Increase line width");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION, ID_SYS_PROFILER_TREE3_RESET_MIN_MAX ,  "Reset Min/Max");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nExportToExcell, ID_SYS_PROFILER_TREE3_EXPORT_TO_EXCELL ,  "Export to Excell...");
	 itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CMarvellDeviceSysProfilerHwModuleBasicDlg::OnNMRClickMarvellDeviceSpTree4(NMHDR *pNMHDR, LRESULT *pResult)
{
     HTREEITEM			  hItem = NULL;
	 CPoint				  point;
	 CNewMenu			  itemMenu;
	 int				  mMenuItem =0;
 	 CBitmap			  bmpImage;
	 UINT32				  nExportToExcell = MF_GRAYED, nAltColorCheck = MF_CHECKED, nIncreaseLineWidth = MF_CHECKED;

	 if((hItem = GetMouseClickedItem(&m_eventsTreeView4,pNMHDR,pResult)) == NULL)
		 return;

     CSysProfilerEvent* pCurrGraphEvent = (CSysProfilerEvent*)m_eventsTreeView4.GetItemData(hItem);
	 if(pCurrGraphEvent == NULL)
	  	 return;

	 if(pCurrGraphEvent == m_eventsTreeView4.GetSelectedEvent())
	 {
		 nExportToExcell = MF_ENABLED;
	 }

	 if(pCurrGraphEvent->GetUseAlternateColor() == FALSE)
	 {
		 nAltColorCheck = MF_UNCHECKED;
	 }
	 if(pCurrGraphEvent->GetEnableTwoDotLine() == FALSE)
	 {
		 nIncreaseLineWidth = MF_UNCHECKED;
	 }

     GetCursorPos(&point);   
	 itemMenu.CreatePopupMenu();
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nAltColorCheck, ID_SYS_PROFILER_TREE4_USE_ALT_COLOR_ITEM ,  "Use alternate color");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nIncreaseLineWidth, ID_SYS_PROFILER_TREE4_INC_LINE_WIDTH ,  "Increase line width");
 	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION , ID_SYS_PROFILER_TREE4_RESET_MIN_MAX ,  "Reset Min/Max");
	 itemMenu.InsertMenu(mMenuItem++, MF_BYPOSITION | nExportToExcell, ID_SYS_PROFILER_TREE4_EXPORT_TO_EXCELL ,  "Export to Excell...");
	 itemMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}
