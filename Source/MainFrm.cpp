// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MVTerminal.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include "HwModulePane.h"
#include "TFTPServerPane.h"
#include "SendScriptPane.h"
#include "DataAnalyzerPane.h"
#include "PlatformInfoPane.h"
#include "CmdExePane.h"
#include "MVTerminalView.h"


#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

#define  ID_POPUP_CLOSE		       (-1)
#define  ID_POPUP_NOTIFICATION_MSG (-2)


TCHAR szAppVersion[]    = _T("8.6");
TCHAR szAppTitlString[] = _T("MVTerminal");   
  
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnTabbarMouseMsg)
	ON_MESSAGE(XTPWM_POPUPCONTROL_NOTIFY, OnPopUpNotify)
	ON_MESSAGE(WM_UPDATE_TAB_COLOR,OnUpdateTabColor)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

    //}}AFX_MSG_MAP
	ON_WM_CHILDACTIVATE()
	ON_WM_MDIACTIVATE()
	ON_WM_COPYDATA()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_STATUS,
	ID_INDICATOR_TIME,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pSendScriptFormFrame    = NULL;
	m_pTFTPFormFrame		  = NULL;
	m_pDataAnalyzerFrame      = NULL;  
	m_pHwModulesFrame         = NULL;  
	m_pCommandExecuterFrame   = NULL;
	m_pPlatformInfoFrame      = NULL;

	m_nColorIndex             = 0;

	m_nAnimation = 2;
	m_uAnimDelay = 500;
	m_uShowDelay = 3000;
	m_bAllowMove = TRUE;
	m_pActivePopup = NULL;
	m_ptPopup = CPoint(-1, -1);

	XTPImageManager()->m_bAutoResample = TRUE;

	m_sAppDataPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	// Load settings from file
	if(LoadSettings(m_sAppDataPath + "\\mainframeSettings.xml") == FALSE) 
	{		
		m_bHideHwModulesPane = TRUE;
	}
}



CMainFrame::~CMainFrame()
{
	SaveSettings(m_sAppDataPath + "\\mainframeSettings.xml");

	while (!m_lstPopupControl.IsEmpty()) 
	{
		delete m_lstPopupControl.RemoveTail();
	}
}

void CMainFrame::OnIPMMessage(CIPMMessage  *pMsg) 
{
	if(m_IPMMessageManager.HandleMessage(pMsg) == FALSE)
	{
		MessageBox("Error while processing message - Message not sent!",NULL,MB_ICONERROR);
	}
}



void CMainFrame::AddLogo()
{
#if 0
	if (!m_wndLogoPane.Create(_T("                  Marvell Semiconductor Inc.                  "), &m_wndStatusBar))
	{
		TRACE0("Failed to create logo control.\n");
		return;
	}

	int nWidth = m_wndLogoPane.GetTextSize().cx;

	// add the indicator to the status bar.
	m_wndStatusBar.AddIndicator(ID_INDICATOR_LOGO, 0);
	
	// Initialize the pane info and add the control.
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOGO);
	ASSERT (nIndex != -1);
	
	m_wndStatusBar.SetPaneWidth(nIndex, nWidth);
	m_wndStatusBar.SetPaneStyle(nIndex, m_wndStatusBar.GetPaneStyle(nIndex) | SBPS_POPOUT);
	m_wndStatusBar.AddControl(&m_wndLogoPane, ID_INDICATOR_LOGO, FALSE);
#endif
}





LRESULT	CMainFrame::OnUpdateTabColor(WPARAM a, LPARAM b)
{
	UpdateTabColor();
	return 0;
}


void  CMainFrame::UpdateTabColor()
{
	if(m_MTIClientWnd.GetWorkspaceCount() == 0)
		return;
	
	if(m_MTIClientWnd.GetWorkspace(0)->GetItemCount() == 1)
	{
		SetTabColor(0,MVT_ACTIVE_TAB_COLOR);
		return;
	}
	SetActiveTabColor(MVT_ACTIVE_TAB_COLOR);
	SetInactiveTabColor(MVT_INACTIVE_TAB_COLOR);
}


void CMainFrame::SetTabColor(int nTabIndex, XTPTabOneNoteColor xtpTabColor)
{
	COLORREF  selItemColor;
	
	if(m_MTIClientWnd.GetWorkspaceCount() == 0)
		return;

	selItemColor = m_MTIClientWnd.GetPaintManager()->GetOneNoteColor(xtpTabColor);
	m_MTIClientWnd.GetWorkspace(0)->GetItem(nTabIndex)->SetColor(selItemColor);
}

void CMainFrame::SetActiveTabColor(XTPTabOneNoteColor xtpTabColor)
{
	COLORREF	 selItemColor;
	int			 nActiveTab;

	if(m_MTIClientWnd.GetWorkspaceCount() == 0)
		return;

	if((nActiveTab = GetActiveTabIndex()) != -1)
	{
		selItemColor = m_MTIClientWnd.GetPaintManager()->GetOneNoteColor(xtpTabColor);
		m_MTIClientWnd.GetWorkspace(0)->GetItem(GetActiveTabIndex())->SetColor(selItemColor);
	}

}
int CMainFrame::GetActiveTabIndex()
{
	if(m_MTIClientWnd.GetWorkspaceCount() == 0)
		return -1;

	return m_MTIClientWnd.GetWorkspace(0)->GetSelectedItem()->GetIndex();
}

void CMainFrame::SetInactiveTabColor(COLORREF color)
{	
	int		   nItemCount, nItem, nSelItemIndex;
	COLORREF   bkItemColor;


	if(m_MTIClientWnd.GetWorkspaceCount() == 0)
		return;

	nSelItemIndex = GetActiveTabIndex();
	nItemCount = m_MTIClientWnd.GetWorkspace(0)->GetItemCount();
	bkItemColor = color;
	for(nItem = 0; nItem < nItemCount; nItem++)
	{
		if(nItem != nSelItemIndex)
		{
			m_MTIClientWnd.GetWorkspace(0)->GetItem(nItem)->SetColor(bkItemColor);
		}
	}
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
        
        if (!pPane->IsValid())
        {
            switch (pPane->GetID())
            {
            case IDR_PANE_TFTP_SERVER:
 				pPane->Attach(m_pTFTPFormFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
				break;
			case IDR_PANE_SEND_SCRIPT:
 				pPane->Attach(m_pSendScriptFormFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
                break;
			case IDR_PANE_DATA_ANALYZER:
				pPane->Attach(m_pDataAnalyzerFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
                break;
 			case IDR_PANE_CMD_EXECUTER: 
				pPane->Attach(m_pCommandExecuterFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
                break;
 			case IDR_PANE_HARDWARE_MODULES:
				pPane->Attach(m_pHwModulesFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
                break;
 			case IDR_PANE_PLATFORM_INFO:
				pPane->Attach(m_pPlatformInfoFrame);
				pPane->SetOptions(xtpPaneNoCloseable);
                break;
			default:
				break;
            }
        }	
		return TRUE;
    }
    return FALSE;
}
void CMainFrame::SetAppTitle(CString sTitle)
{
	SetTitle(sTitle);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create Status bar. 
    // Important: All control bars including the Status Bar 
    // must be created before CommandBars....

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }

    // Add the menu bar
    CXTPToolBar* pMenuBar = pCommandBars->SetMenu( _T("Menu Bar"), IDR_MVTerminalTYPE);       
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }

    // Create ToolBar
    CXTPToolBar* pStandardBar = (CXTPToolBar*) pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pStandardBar || !pStandardBar->LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

	// Create ToolBar
	CXTPToolBar* pPowerCtrlToolBar = (CXTPToolBar*) pCommandBars->Add(_T("Power Control"), xtpBarTop);
    if (!pPowerCtrlToolBar || !pPowerCtrlToolBar->LoadToolBar(IDR_POWER_CTRL_TOOLBAR))
    {
        TRACE0("Failed to create power ctrl toolbar\n");
        return -1;
    }

	//Dock pPowerCtrlToolBar right of pStandardBar
	DockRightOf(pPowerCtrlToolBar,pStandardBar);

	/* Remove the expand toolbar arrow */
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = false;
	/* Remove customization option from menu when clicking right mouse button */
	pCommandBars->EnableCustomization(FALSE);

  	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	// Initialize the docking pane manager and set the
	// initial theme for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(m_nPanesTheme);

		// Create docking panes.
	CXTPDockingPane* pTftpServerPane = m_paneManager.CreatePane(
		IDR_PANE_TFTP_SERVER, CRect(0, 0,180, 225), xtpPaneDockBottom);

	CXTPDockingPane* pSendScriptPane = m_paneManager.CreatePane(
		IDR_PANE_SEND_SCRIPT, CRect(0, 0,180, 225), xtpPaneDockBottom);

	CXTPDockingPane* pDataAnalyzerPane = m_paneManager.CreatePane(
		IDR_PANE_DATA_ANALYZER, CRect(0, 0,180, 225), xtpPaneDockBottom);
	
	CXTPDockingPane* pCmdExePane = m_paneManager.CreatePane(
		IDR_PANE_CMD_EXECUTER, CRect(0, 0,180, 225), xtpPaneDockBottom);

	CXTPDockingPane* pHWModulesPane = m_paneManager.CreatePane(
		IDR_PANE_HARDWARE_MODULES, CRect(0, 0,298, 225), xtpPaneDockLeft);

	CXTPDockingPane* pPlatformInfoPane = m_paneManager.CreatePane(
		IDR_PANE_PLATFORM_INFO, CRect(0, 0,274, 225), xtpPaneDockLeft);

	//Init pane mapping... for easy access to pane control
	m_paneMap[IDR_PANE_TFTP_SERVER] = pTftpServerPane;
	m_paneMap[IDR_PANE_SEND_SCRIPT] = pSendScriptPane;
	m_paneMap[IDR_PANE_DATA_ANALYZER] = pDataAnalyzerPane;
	m_paneMap[IDR_PANE_CMD_EXECUTER] = pCmdExePane;
	m_paneMap[IDR_PANE_HARDWARE_MODULES] = pHWModulesPane;
	m_paneMap[IDR_PANE_PLATFORM_INFO] = pPlatformInfoPane;

    m_paneManager.AttachPane(pSendScriptPane, pTftpServerPane);	
	m_paneManager.AttachPane(pDataAnalyzerPane, pTftpServerPane);
	m_paneManager.AttachPane(pCmdExePane, pTftpServerPane);
	m_paneManager.AttachPane(pPlatformInfoPane, pTftpServerPane);
	m_paneManager.AttachPane(pHWModulesPane, pHWModulesPane);
	m_paneManager.SetThemedFloatingFrames(TRUE);

	// Select the TFTP server 
	pTftpServerPane->Select();
	m_paneManager.SetAnimationDelay(0.007);
	// Set pane icons
	int nIDIcons[] = {IDR_PANE_TFTP_SERVER, IDR_PANE_SEND_SCRIPT, IDR_PANE_DATA_ANALYZER,IDR_PANE_CMD_EXECUTER,IDR_PANE_HARDWARE_MODULES,IDR_PANE_PLATFORM_INFO};
	m_paneManager.SetIcons(IDB_PANES_BITMAP, nIDIcons,
		sizeof(nIDIcons)/sizeof(nIDIcons[0]), RGB(192, 192, 192)/*Use this mask to remove icon background (like transperent background) */);

	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetOneNoteColors(TRUE);
	m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bBoldSelected = TRUE;	
	m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bShowIcons    = TRUE;
	m_paneManager.GetPaintManager()->GetTabPaintManager()->SetAppearance(xtpTabAppearanceVisualStudio2005) ;

	//Hide pane by default
	if(m_bHideHwModulesPane == TRUE)
	{
		pHWModulesPane->Hide();
	}
	
	/* Add tab support */
	VERIFY(m_MTIClientWnd.Attach(this));

	CXTPTabPaintManager* pTabPaintManager = new CXTPTabPaintManager();
	pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio2005);

	pTabPaintManager->RefreshMetrics();
	pTabPaintManager->m_bOneNoteColors  = TRUE;
	pTabPaintManager->m_bHotTracking    = TRUE;
	pTabPaintManager->m_bShowIcons      = TRUE;
	pTabPaintManager->m_bBoldSelected   = TRUE;

	m_MTIClientWnd.SetPaintManager(pTabPaintManager);

	// Sets the session window line thickness of the tab 
	pTabPaintManager->m_rcClientMargin.SetRect(1, 1, 1, 1);

	// Create panes here so they can be loaded with configuration using startup file
	CreatePanes();

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(m_nAppTheme);

	if(m_IPMMessageManager.Init(this) == FALSE)
	{
		return 0; 
	}
	return 0; 
}
void CMainFrame::ShowPane(int nID,BOOL bShow)
{
	if(bShow == TRUE)
	{
		if(m_paneManager.IsPaneHidden(GetPane(nID)) == TRUE)
		{
			m_paneManager.ToggleAutoHide(GetPane(nID));
		}
	}
	else
	{
		if(m_paneManager.IsPaneHidden(GetPane(nID)) == FALSE)
		{
			m_paneManager.ToggleAutoHide(GetPane(nID));
		}
	}
}

CXTPDockingPane*    CMainFrame::GetPane(int nID)
{
	if(m_paneMap.find(nID) != m_paneMap.end())
	{
		return m_paneMap[nID];
	}
	return NULL;
}


void CMainFrame::CreatePanes()
{
	m_pTFTPFormFrame = new CFrameWnd;
	CCreateContext TFTPServerContext;
	TFTPServerContext.m_pNewViewClass = RUNTIME_CLASS(CTFTPServerPane);
	TFTPServerContext.m_pCurrentDoc = NULL;
	if(m_pTFTPFormFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &TFTPServerContext) == FALSE)
	{
		AfxMessageBox("Failed to create TFTP Server Pane",NULL,MB_ICONERROR);
	}
	
	m_pSendScriptFormFrame = new CFrameWnd;
	CCreateContext SendScriptContext;
	SendScriptContext.m_pNewViewClass = RUNTIME_CLASS(CSendScriptPane);
	SendScriptContext.m_pCurrentDoc = NULL;
	if(m_pSendScriptFormFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &SendScriptContext) == FALSE)
	{
		AfxMessageBox("Failed to create Send Script Pane",NULL,MB_ICONERROR);
	}
	
	m_pDataAnalyzerFrame = new CFrameWnd;
	CCreateContext DataAnalyzerContext;
	DataAnalyzerContext.m_pNewViewClass = RUNTIME_CLASS(CDataAnalyzerPane);
	DataAnalyzerContext.m_pCurrentDoc = NULL;
	if(m_pDataAnalyzerFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &DataAnalyzerContext) == FALSE)
	{
		AfxMessageBox("Failed to create Data Analyzer Pane",NULL,MB_ICONERROR);
	}

	m_pCommandExecuterFrame = new CFrameWnd;
	CCreateContext CommandExecuterContext;
	CommandExecuterContext.m_pNewViewClass = RUNTIME_CLASS(CCmdExePane);
	CommandExecuterContext.m_pCurrentDoc = NULL;
	if(m_pCommandExecuterFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &CommandExecuterContext) == FALSE)
	{
		AfxMessageBox("Failed to create Command Executer Pane",NULL,MB_ICONERROR);
	}

	m_pHwModulesFrame = new CFrameWnd;
	CCreateContext HWModulesContext;
	HWModulesContext.m_pNewViewClass = RUNTIME_CLASS(CHwModulesPane);
	HWModulesContext.m_pCurrentDoc = NULL;
	if(m_pHwModulesFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &HWModulesContext) == FALSE)
	{
		AfxMessageBox("Failed to create Hardware Modules Pane",NULL,MB_ICONERROR);
	}

	m_pPlatformInfoFrame = new CFrameWnd;
	CCreateContext PlatformInfoContext;
	PlatformInfoContext.m_pNewViewClass = RUNTIME_CLASS(CPlatformInfoPane);
	PlatformInfoContext.m_pCurrentDoc = NULL;
	if(m_pPlatformInfoFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, NULL, 0, &PlatformInfoContext) == FALSE)
	{
		AfxMessageBox("Failed to create Platform Info Pane",NULL,MB_ICONERROR);
	}

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnClose()
{
	((CMVTerminalApp*)AfxGetApp())->CreateRestoreFile();

	((CMVTerminalApp*)AfxGetApp())->SetSaveConfigDlgCloseBtnText("Close Application");
	switch( ((CMVTerminalApp*)AfxGetApp())->SaveConfiguration() )
	{
	case IDYES:
	case IDCANCEL:			
		break;
	case IDCLOSE:
		return;
	default:
		break;
	}
	CMDIFrameWnd::OnClose();
}

CPlatformInfoPane*   CMainFrame::GetPlatformInfoPane() 
{
	if(m_pPlatformInfoFrame != NULL)
		return static_cast<CPlatformInfoPane *>(m_pPlatformInfoFrame->GetTopWindow());
	else
		return NULL;
}

CDataAnalyzerPane*   CMainFrame::GetDataAnalyzerPane() 
{
	if(m_pDataAnalyzerFrame != NULL)
		return static_cast<CDataAnalyzerPane *>(m_pDataAnalyzerFrame->GetTopWindow());
	else
		return NULL;
}
CSendScriptPane*    CMainFrame::GetSendScriptPane()   
{
	if(m_pSendScriptFormFrame != NULL)
		return static_cast<CSendScriptPane *>(m_pSendScriptFormFrame->GetTopWindow());
	else
		return NULL;
}

CTFTPServerPane*   CMainFrame::GetTFTPServerPane()   
{
	if(m_pTFTPFormFrame != NULL)
		return static_cast<CTFTPServerPane *>(m_pTFTPFormFrame->GetTopWindow());
	else
		return NULL;
}

CCmdExePane*   CMainFrame::GetCommandExecuterPane()
{
	if(m_pCommandExecuterFrame != NULL)
		return static_cast<CCmdExePane *>(m_pCommandExecuterFrame->GetTopWindow());
	else
		return NULL;
}
CHwModulesPane*     CMainFrame::GetHardwareModulesPane()
{
	if(m_pHwModulesFrame != NULL)
		return static_cast<CHwModulesPane *>(m_pHwModulesFrame->GetTopWindow());
	else
		return NULL;
}

LRESULT CMainFrame::OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam)
{	
	if (wParam != WM_RBUTTONDOWN)
		return FALSE;

	CPoint point = CPoint((DWORD)lParam);

	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);
	if (pItem)
	{
		CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pItem->GetHandle()));
		if (!pFrame)
			return FALSE;

		MDIActivate(pFrame);
		pFrame->GetActiveView()->SetFocus();

		m_MTIClientWnd.Refresh();

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_TABS));
		CMenu* pPopup = menu.GetSubMenu(0);

		m_MTIClientWnd.WorkspaceToScreen(&point);

		CXTPPopupBar* pPopupBar = (CXTPPopupBar*) CXTPPopupBar::CreatePopupBar(GetCommandBars());
		VERIFY(pPopupBar->LoadMenu(pPopup));

		VERIFY(pPopupBar->Popup(point.x, point.y, NULL));

		while ( pPopupBar->IsTrackingMode() )
		{
			AfxGetApp()->PumpMessage();
		}
		pPopupBar->InternalRelease();

		m_MTIClientWnd.Refresh();
		return TRUE;
	}
	return FALSE;
}



void CMainFrame::SetNotificationTheme()
{
	m_pActivePopup->RemoveAllItems();

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_pActivePopup->AddItem(
		new CXTPPopupItem(CRect(25, 6, 170, 19), _T("MVTerminal")));
	pItemText->SetTextAlignment(DT_LEFT);
	pItemText->CalculateHeight();
	pItemText->CalculateWidth();
	pItemText->SetHyperLink(FALSE);

		pItemText = (CXTPPopupItem*)m_pActivePopup->AddItem(
			new CXTPPopupItem(CRect(0, 50, 170, 100), 
			/*_T("Sending E-Mail notification")*/m_pActivePopup->GetMessage()));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->CalculateHeight();
	pItemText->SetID(ID_POPUP_NOTIFICATION_MSG);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_pActivePopup->AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSE, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	pItemIcon = (CXTPPopupItem*)m_pActivePopup->AddItem(
		new CXTPPopupItem(CRect(7, 6, 20, 19)));
	pItemIcon->SetIcons(IDB_NOTIFICATION_CAPTION, 0, xtpPopupItemIconNormal);


	m_pActivePopup->SetTheme(xtpPopupThemeMSN);
	m_pActivePopup->RedrawControl();
}

LRESULT CMainFrame::OnPopUpNotify(WPARAM wParam, LPARAM lParam)
{
// process notify from popup wnd

	if(wParam == XTP_PCN_ITEMCLICK)
	{
		CXTPPopupItem* pItem = (CXTPPopupItem*)lParam;
		ASSERT(pItem);

		if(pItem->GetID() == ID_POPUP_NOTIFICATION_MSG)
		{
		}

		pItem->GetPopupControl()->Close();
	}
	else if(wParam == XTP_PCN_STATECHANGED)
	{
		CXTPPopupControlEx* pControl = (CXTPPopupControlEx*)lParam;
		ASSERT(pControl);

		if(pControl->GetPopupState() == xtpPopupStateClosed)
		{
			m_lstPopupControl.RemoveAt(m_lstPopupControl.Find(pControl));
			m_pActivePopup = NULL;
			delete pControl;
		}
	}
	else if (wParam == XTP_PCN_POSCHANGED)
	{
		CXTPPopupControlEx* pControl = (CXTPPopupControlEx*)lParam;
		ASSERT(pControl);
		
		m_ptPopup = pControl->GetPopupPos();
	}
		
	return TRUE;
}


void CMainFrame::Notify(CString sMsg)
{
	m_pActivePopup = new CXTPPopupControlEx(sMsg);
	
	CSize szPopup (170, 130);
	if(m_nTheme == 8)
		szPopup = CSize(220, 90);
	SetNotificationTheme();
	m_pActivePopup->SetPopupAnimation((XTPPopupAnimation)m_nAnimation);
	m_pActivePopup->SetShowDelay(m_uShowDelay);
	m_pActivePopup->SetAnimateDelay(m_uAnimDelay);
	m_pActivePopup->SetPopupSize(szPopup);
	FindBestPosition(szPopup);
	m_pActivePopup->AllowMove(m_bAllowMove);
	m_pActivePopup->Show(this);
	m_lstPopupControl.AddTail(m_pActivePopup);
}



void CMainFrame::FindBestPosition(CSize szPopup)
{
	if (m_ptPopup != CPoint(-1, -1)) 
		m_pActivePopup->SetPopupPos(m_ptPopup);

	CPoint ptPopup = m_pActivePopup->GetPopupPos();

	CRect rcActivePopup(CPoint(ptPopup.x - szPopup.cx, ptPopup.y - szPopup.cy), szPopup);

	BOOL bIntersect = FALSE;
	do 
	{
		bIntersect = FALSE;
		POSITION pos = m_lstPopupControl.GetHeadPosition();

		while(pos)
		{
			CXTPPopupControlEx* pPopup = m_lstPopupControl.GetNext(pos);
			
			CRect rcPopup(CPoint(pPopup->GetPopupPos().x - pPopup->GetPopupSize().cx, 
				pPopup->GetPopupPos().y - pPopup->GetPopupSize().cy), pPopup->GetPopupSize());
			
			if (CRect().IntersectRect(rcPopup, rcActivePopup))
			{
				ptPopup.y =  rcPopup.top;
				rcActivePopup = CRect(CPoint(ptPopup.x - szPopup.cx, ptPopup.y - szPopup.cy), szPopup);
				bIntersect = TRUE;
			}
		}
	} while (bIntersect);

	m_pActivePopup->SetPopupPos(ptPopup);
}

void    CMainFrame::AddView(CMVTerminalView* pView)
{
	m_viewList.push_back(pView);
	GetTFTPServerPane()->AllowLoadControlls(TRUE);
}

void    CMainFrame::CloseAllViews()
{
	list<CMVTerminalView*>::iterator  iter, endIter;
	
	while(GetViewCount() > 0)
	{
		for(iter = m_viewList.begin(); iter != m_viewList.end(); iter++)
		{
			((CChildFrame*)((*iter)->GetParentFrame()))->CloseWindow();
			break;
		}
	}
	m_viewList.clear();
	GetTFTPServerPane()->EnableAllControlls(FALSE);
}


void    CMainFrame::RemoveView(CMVTerminalView* pView)
{
	list<CMVTerminalView*>::iterator  iter, endIter;

	for(iter = m_viewList.begin(); iter != m_viewList.end(); iter++)
	{
		if((*iter) == pView)
		{
			m_viewList.erase(iter);
			break;
		}
	}
	if(GetViewCount() == 0)
	{
		GetTFTPServerPane()->EnableAllControlls(FALSE);
	}
}
CMVTerminalView*    CMainFrame::GetViewByName(CString sName)
{
	list<CMVTerminalView*>::iterator  iter, endIter;

	for(iter = m_viewList.begin(); iter != m_viewList.end(); iter++)
		if(((CMVTerminalView*)(*iter))->GetConnectionParams().GetWndName() == sName)
			return (*iter);
	return NULL;
}
CMVTerminalView*    CMainFrame::GetViewByCOM(CString sCom)
{
	list<CMVTerminalView*>::iterator  iter, endIter;

	for(iter = m_viewList.begin(); iter != m_viewList.end(); iter++)
	{
		if(((CMVTerminalView*)(*iter))->GetConnectionParams().GetComPort() == sCom)
			return (*iter);
	}
	return NULL;
}


void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MVTerminalTYPE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MVTerminalTYPE);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

void CMainFrame::OnChildActivate()
{
	CXTPMDIFrameWnd::OnChildActivate();
}

void CMainFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CXTPMDIFrameWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
} 

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	if(pCopyDataStruct->dwData == IPM_MESSAGE)
	{
		OnIPMMessage((CIPMMessage*)(pCopyDataStruct->lpData));
	}
	return CXTPMDIFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

BOOL	CMainFrame::SaveSettings(CString sFile)
{
	CStdioFile  file;
	CMarkup		xmlWriter;

	if(file.Open(sFile,CFile::typeText | CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		AfxMessageBox("Failed to open file " + sFile + "!",NULL,MB_ICONERROR);
		return FALSE;
	}

	xmlWriter.AddElem("MainFrame");
	m_bHideHwModulesPane = GetPane(IDR_PANE_HARDWARE_MODULES)->IsHidden();
	xmlWriter.AddAttrib("HideHwModulesPane",m_bHideHwModulesPane);

	file.WriteString(xmlWriter.GetDoc());
	file.Close();
	return TRUE;
}

BOOL	CMainFrame::LoadSettings(CString sFile)
{
	CMarkup	 xmlReader;

	if(xmlReader.Load(sFile) == false)
		return FALSE;

	if(xmlReader.FindElem("MainFrame") == false)
		return FALSE;

	m_bHideHwModulesPane = (xmlReader.GetAttrib("HideHwModulesPane") == "0")? FALSE:TRUE;

	return TRUE;

}


BOOL	CMainFrame::DeSerialize(CMarkup & xmlReader)
{	
	//First, close all views
	CloseAllViews();

	if(xmlReader.FindElem("MainFrame") == false)
		return FALSE;

	m_bHideHwModulesPane = (xmlReader.GetAttrib("HideHwModulesPane") == "TRUE")? TRUE:FALSE;
	ShowPane(IDR_PANE_HARDWARE_MODULES,!m_bHideHwModulesPane);
	xmlReader.IntoElem();

	if(xmlReader.FindElem("Sessions") == false)
		return FALSE;

	xmlReader.IntoElem(); 
	while(xmlReader.FindElem("Session") == true)
	{
		xmlReader.IntoElem();
		
		CViewPropertiesDlg*  pViewPropertiesDlg = new CViewPropertiesDlg();
		if(pViewPropertiesDlg->DeSerialize(xmlReader) == FALSE)
			return FALSE;

		if(((CMVTerminalApp*)AfxGetApp())->CreateNewSession(pViewPropertiesDlg,FALSE) == FALSE)
		{
			delete pViewPropertiesDlg;
		}

		xmlReader.OutOfElem(); 

	}
	xmlReader.OutOfElem();
	xmlReader.OutOfElem(); 
	return TRUE;
}

CString CMainFrame::Serialize()
{
	CMarkup xmlWriter;
	
	xmlWriter.AddElem("MainFrame");
	xmlWriter.AddAttrib("HideHwModulesPane",m_bHideHwModulesPane);
	xmlWriter.IntoElem();

	xmlWriter.AddElem("Sessions");
	xmlWriter.IntoElem();
	if(GetViewCount() > 0)
	{
		list<CMVTerminalView*> viewList = GetViewList();
		list<CMVTerminalView*>::iterator   iter = viewList.begin(), endIter = viewList.end();
		for(;iter != endIter; iter++)
		{
			xmlWriter.AddSubDoc( (*iter)->Serialize() );
		}
	}
	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}
