
// MVTerminalView.cpp : implementation of the CMVTerminalView class
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "TFTPServerPane.h"
#include "SendScriptPane.h"
#include "DataAnalyzerPane.h"
#include "CmdExePane.h"
#include "IPMMessage.h"
#include "MVTerminalDoc.h"
#include "CntrItem.h"
#include "MVTerminalView.h"
#include ".\mvterminalview.h"
#include "NewMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW 
#endif

#define  VK_CTRL_C						TCHAR(3)
#define  VK_CTRL_V						TCHAR(22)
#define  VK_CTRL_R						TCHAR(18)
#define  CHAR_CR  						TCHAR(13)
#define  CHAR_LF  						TCHAR(10)
#define  CHAR_TAB_ECHO  				TCHAR(7)
#define  MAX_CHAR_COUNT					550000
#define  MAX_LINE_COUNT					500
#define  MOUSEWHEEL_EVENT				433
#define  MOUSESELECTION_EVENT			432
#define  MOUSEMIDBUTTON_EVENT			435


// CMVTerminalView

IMPLEMENT_DYNCREATE(CMVTerminalView, CRichEditView)
 
BEGIN_MESSAGE_MAP(CMVTerminalView, CRichEditView)
	ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
	
	ON_COMMAND(WM_CLEAR_HISTORY,OnClearHistory)

	ON_COMMAND(ID_NEW_FONT,OnNewFont)
	ON_COMMAND(ID_EDIT_FIND, SearchText)
	ON_COMMAND(ID_VIEW_CLEARBUFFER,OnClearBuffer)
	ON_COMMAND(ID_VIEW_WIN_CONTENT,OnViewWindowContent)
	ON_COMMAND(ID_PORT_DISCONNECT,OnPortDisconnect)
	ON_UPDATE_COMMAND_UI(ID_PORT_DISCONNECT, OnPortDisconnectUpdate)
	ON_COMMAND(ID_PORT_CONNECT,OnPortConnect)
	ON_UPDATE_COMMAND_UI(ID_PORT_CONNECT, OnPortConnectUpdate)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnTimeChange)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_CAPTURETOFILE, OnCaptureToFile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTURETOFILE,OnCaptureToFileUpdate  )
	ON_COMMAND(ID_VIEW_WRAPTEXT,OnWrapText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WRAPTEXT,OnWrapTextUpdate)
	ON_WM_CHAR()
	ON_MESSAGE(WM_THREAD_DONE,OnThreadDone)
	ON_COMMAND(ID_PORT_PROPERTIES, OnViewProperties)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_LOAD_TFTP_FILE, OnLoadTftpFile)
        ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
        ON_WM_CLOSE()
        ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CMVTerminalView, CRichEditView)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 1, OnRxCharSportctrl1, VTS_I4)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 2, OnCTSSportctrl1, VTS_BOOL)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 3, OnDSRSportctrl1, VTS_BOOL)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 4, OnRingSportctrl1, VTS_BOOL)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 5, OnBreakSportctrl1, VTS_NONE)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 6, OnDCDSportctrl1, VTS_BOOL)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 7, OnTxEmptySportctrl1, VTS_NONE)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 8, OnRxFlagSportctrl1, VTS_NONE)
	ON_EVENT(CMVTerminalView, IDC_SPORTAX, 10, OnCommErrorSportctrl1, VTS_I4)
END_EVENTSINK_MAP()

void CMVTerminalView::OnKillFocus(CWnd* pNewWnd)
{
	//This function override fixes the bug that the caret is missing after losing focus
}

void CMVTerminalView::OnRxCharSportctrl1(long Count)
{
	if(GetThreadStatus() == MVT_THREAD_IS_NOT_ACTIVE)
	{
		// Stop sending events 
		SetThreadStatus(MVT_THREAD_IS_ACTIVE);
		AfxBeginThread(ReadPortDataThread,this); 
	}
}

void CMVTerminalView::OnCTSSportctrl1(BOOL On)
{
	//CString sTmp;
	//sTmp.Format("CTS changed to \"%s\"", On ? "ON":"OFF");
	//AddEventsLogLine(sTmp);
	//AfxMessageBox(sTmp);
}

void CMVTerminalView::OnDSRSportctrl1(BOOL On)
{
	//CString sTmp;
	//sTmp.Format("DSR changed to \"%s\"", On ? "ON":"OFF");
	//AddEventsLogLine(sTmp);
	//AfxMessageBox(sTmp);

}

void CMVTerminalView::OnRingSportctrl1(BOOL On)
{
	//CString sTmp;
	//sTmp.Format("RING changed to \"%s\"", On ? "ON":"OFF");
    //AddEventsLogLine(sTmp);
}

void CMVTerminalView::OnBreakSportctrl1()
{
	//CString sTmp;
	//sTmp.Format("OnBreak event occured");
	//AddEventsLogLine(sTmp);
	//AfxMessageBox(sTmp);

}

void CMVTerminalView::OnDCDSportctrl1(BOOL On)
{
	//CString sTmp;
	//sTmp.Format("DCD changed to \"%s\"", On ? "ON":"OFF");
	//AddEventsLogLine(sTmp);
	//AfxMessageBox(sTmp);

}

void CMVTerminalView::OnTxEmptySportctrl1()
{
	//CString sTmp;
	//sTmp.Format("OnTxEmpty event occured");
	//AddEventsLogLine(sTmp);
}

void CMVTerminalView::OnRxFlagSportctrl1()
{
	//CString sTmp;
	//sTmp.Format("OnRxFlag event occured");
	//AddEventsLogLine(sTmp);
}

void CMVTerminalView::OnCommErrorSportctrl1(long err)
{
	//CString sTmp;
	//sTmp.Format("OnCommError event occured - %d", err);
	//AddEventsLogLine(sTmp);
	if(err != 0)
	{
		//AfxMessageBox(sTmp);
		ResetConnection();
	}

}


// CMVTerminalView construction/destruction

/*Debug functions */
void 	ShowText(CString tmp)
{
	char	buf[10];
	CString st;
	int     i =0 ;	
	for(i = 0; i < tmp.GetLength();i++)
	{
		sprintf(buf,"%d %c",tmp[i],tmp[i]);
		st += buf;
		st += " ";
	}
	AfxMessageBox(st);
}
void tostring(UINT num)
{
	char buf[32];
	sprintf(buf,"%d",num);
	AfxMessageBox(buf);
}

CMVTerminalView::CMVTerminalView()
{
	m_pMainFrame   = reinterpret_cast<CMainFrame *>(AfxGetApp()->m_pMainWnd);

	EnableConnectionTimer(TRUE);
	SetCaptureToFileStatus(FALSE);
	m_threadStatus       = MVT_THREAD_IS_NOT_ACTIVE;
	m_pViewPropertiesDlg = NULL;
	m_nSecondCount       = 0;
	m_bEnableViewMsg     = TRUE;
	m_bPortConnected     = false;

	m_nCursorIndex       = 0;
	m_bInEditMode     = false;

}

CMVTerminalView::~CMVTerminalView()
{
	KillTimer(m_pIDTimer);
	/* Close file capture incase window was closed */
	m_captureFile.Close();
	// Stop showing time
	EnableConnectionTimer(FALSE);
	// Update the view count
	// Clear the time count if its the last view
    if(m_pMainFrame->GetViewCount() == 0)
	{
		m_pMainFrame->GetStatusBar().SetPaneText(
				   m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_TIME), LPCSTR("Time Elapsed:"));
	    m_pMainFrame->GetStatusBar().SetPaneText(
             m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_STATUS), LPCSTR(m_sConnectionStatus));
	}
	if(m_pViewPropertiesDlg != NULL)
	{
		delete m_pViewPropertiesDlg;
	}

	m_pMainFrame->RemoveView(this);

}



BOOL CMVTerminalView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CRichEditView::PreCreateWindow(cs);
}

void CMVTerminalView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));

	CClientDC dc(this);
	m_viewFont.CreateFont(-MulDiv(12, dc.GetDeviceCaps(LOGPIXELSX), 72), 
		0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Courier New"));
	SetFont(&m_viewFont);
	m_viewFont.GetLogFont(&m_lf);

	// Set read only
	GetRichEditCtrl().SetReadOnly();

	/* Start timer */
	m_pIDTimer = SetTimer(WM_TIMER, 1000, NULL); 
	m_sConnectionStatus = "Connected";

	// Turn on the horizontal scroll bar by disabling wrap text
    //m_nWordWrap = WrapNone;
    //WrapChanged();

	// Turn on wrap text
	OnWrapText();
	// Disable horizental  ECO_AUTOHSCROLL
	SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_AND, (LPARAM)(~ECO_AUTOHSCROLL));
	/* Shoe the scrollbars */
	ShowScrollBars();

	// Set default monitor parameters
	m_bootLoaderParams.SetTFTPLoadAddress("0x81000000");
	m_bootLoaderParams.SetRunCommand("go");
	m_bootLoaderParams.SetTFTPLoadCommand("tftp");
	m_bootLoaderParams.SetTFTPBurnAsIsCommand("FStftp");
	m_bootLoaderParams.SetTFTPBurnExeCommand("FStftpe");

	/* Create thin */
    ModifyStyle (0, WS_BORDER);
    ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
    GetParentFrame()->RecalcLayout();

}

void CMVTerminalView::ShowScrollBars()
{
	SendMessage(EM_SHOWSCROLLBAR,SB_VERT,TRUE);
	SendMessage(EM_SHOWSCROLLBAR,SB_HORZ,TRUE);
}

// CMVTerminalView printing

BOOL CMVTerminalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CMVTerminalView::OnDestroy()
{
	OnPortDisconnect();


	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}



// CMVTerminalView diagnostics

#ifdef _DEBUG
void CMVTerminalView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CMVTerminalView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CMVTerminalDoc* CMVTerminalView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMVTerminalDoc)));
	return (CMVTerminalDoc*)m_pDocument;
}
#endif //_DEBUG

void  CMVTerminalView::SetPropertiesDlg(CViewPropertiesDlg *pDlg)
{
	m_pViewPropertiesDlg = pDlg;
}


void CMVTerminalView::OnNewFont()
{
	CFontDialog     fontDlg(&m_lf);

	if(fontDlg.DoModal() != IDOK)
		return;

	fontDlg.GetCurrentFont(&m_lf);
	m_lf.lfCharSet = DEFAULT_CHARSET;
    m_viewFont.CreateFontIndirect(&m_lf);
	CClientDC dc(this);
	SetFont(&m_viewFont);
}

void CMVTerminalView::SearchText() 
{
	OnEditFindReplace(true);
}

void	CMVTerminalView::OnClearBuffer()
{
	SetWindowText("");
}
void	CMVTerminalView::OnPortConnect()
{
	if(m_SPCommCtrl.get_IsOpened() == TRUE)
		return;
	m_SPCommCtrl.Open(GetConnectionParams().GetComPort));
	EnableConnectionTimer(TRUE);
	m_bPortConnected = true;
}
void	CMVTerminalView::OnPortConnectUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bPortConnected);
}




void	CMVTerminalView::EnableConnectionTimer(BOOL bEnable)
{
    // Reset time and enable update
	m_showTimeCount     = (bEnable == TRUE)? true:false;
	m_nHour             = 0;	
	m_nMinute           = 0;
	m_nSecond           = 0; 
	m_sConnectionStatus = (bEnable == TRUE)?  "Connected":"Disconnected";
}
void	CMVTerminalView::OnPortDisconnect()
{
	if(m_SPCommCtrl.get_IsOpened() == TRUE)
		m_SPCommCtrl.Close();
	EnableConnectionTimer(FALSE);
	m_bPortConnected = false;
}

void	CMVTerminalView::OnPortDisconnectUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bPortConnected);
}

void CMVTerminalView::OnTimer(UINT nIDEvent)
{
	// Mouse wheel generates events that reach this function.
	if((nIDEvent != MOUSEWHEEL_EVENT) && 
	   (nIDEvent != MOUSESELECTION_EVENT) && 
	   (nIDEvent != MOUSEMIDBUTTON_EVENT))
	{
		m_nSecond++;
		if(m_nSecond == 60)
		{
			m_nSecond = 0;
			m_nMinute++;
		}
		if(m_nMinute == 60)
		{
			m_nMinute = 0;
			m_nHour++;
		}
		if(m_nHour == 100)
		{
			m_nHour = 0;
		}

		/* Send status e-mail */
		if(GetSendMailEn() == TRUE)
		{
			m_nSecondCount++;
			if(CheckSendDuration() == TRUE)
			{
				SendMail();
				m_nSecondCount = 0;
			}
		}
		else
		{
			m_nSecondCount = 0;
		}
	}

	CRichEditView::OnTimer(nIDEvent);
}

void CMVTerminalView::OnTimeChange(CCmdUI *pCmdUI)
{
    char	szTime[32];

	if(m_showTimeCount == false)
	{
		m_pMainFrame->GetStatusBar().SetPaneText(
				   m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_TIME), LPCSTR("Time Elapsed:"));
	    m_pMainFrame->GetStatusBar().SetPaneText(
             m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_STATUS), LPCSTR(m_sConnectionStatus));

		return;
	}
	wsprintf(szTime, "Time Elapsed: %i:%02i:%02i", m_nHour, m_nMinute,m_nSecond);
    // Now set the text of the pane.
    m_pMainFrame->GetStatusBar().SetPaneText(
               m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_TIME), LPCSTR(szTime));

    m_pMainFrame->GetStatusBar().SetPaneText(
               m_pMainFrame->GetStatusBar().CommandToIndex(ID_INDICATOR_STATUS), LPCSTR(m_sConnectionStatus));
    pCmdUI->Enable(TRUE);
}

BOOL    CMVTerminalView::GetSendMailEn()
{
	return m_pViewPropertiesDlg->GetSendMailTab().GetSendMailEn();
}

BOOL    CMVTerminalView::CheckSendDuration()
{
	unsigned int nSendDuration;
	nSendDuration = m_pViewPropertiesDlg->GetSendMailTab().GetSendDurationInSeconds();
	if(nSendDuration == m_nSecondCount)
        return TRUE;
	return FALSE;
}

BOOL    CMVTerminalView::SendMail()
{		
	CString sPath;
	DumpScreenBufferToFile(sPath);

	ShowSendMailNotification();

	return m_pViewPropertiesDlg->GetSendMailTab().SendMail(sPath);
}

void CMVTerminalView::ShowSendMailNotification()
{
	CString  sMsg;

	sMsg = "Sending e-mail with screen\ncapture to:\n\n";
	sMsg += m_pViewPropertiesDlg->GetSendMailTab().GetMailRecipient();
	m_pMainFrame->Notify(sMsg);
}


void CMVTerminalView::OnCaptureToFileUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bCaptureToFile);
}
void CMVTerminalView::OnCaptureToFile()
{
	if(GetCaptureToFileStatus() == FALSE)
	{
		static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
		CString sFName;

		sFName = GetConnectionParams().GetComPort();
		sFName += "_Log";
 		CFileDialog		dlg(TRUE,"",sFName,NULL,szFilter);

		if(dlg.DoModal() == IDOK)
		{
			CString path = dlg.GetPathName();
			if(!m_captureFile.Open(path, CFile::modeCreate
				   | CFile::modeWrite | CFile::typeText ) ) 
			{
				MessageBox("Can't open " + path + " file for writing...",NULL,MB_ICONEXCLAMATION);
				return;
			}		
			SetCaptureToFileStatus(TRUE);
		}
	}
	else
	{
		SetCaptureToFileStatus(FALSE);
		m_captureFile.Close();
	}
}
void CMVTerminalView::SetCaptureToFileStatus(BOOL bStatus)
{
	m_bCaptureToFile = bStatus;
}

BOOL CMVTerminalView::GetCaptureToFileStatus()
{
	return m_bCaptureToFile;
}




BOOL   CMVTerminalView::CreateCommCtrl()
{
	return m_SPCommCtrl.Create(NULL, WS_VISIBLE, CRect(0,0,0,0),
							   this,IDC_SPORTAX, NULL, FALSE);
}


BOOL	CMVTerminalView::ActivateComPort(CConnectionParams connectionParams)
{
	OnPortDisconnect();

	/* Set the COM properties */
	SetConnectionParams(connectionParams);
	CString	 sComSettings;
	sComSettings  = GetConnectionParams().GetBaudRate();
	// Parity
	sComSettings += "," + GetConnectionParams().GetParity().Left(1);
	// Data bits
	sComSettings += "," + GetConnectionParams().GetDataBits();
	// Stop bits
	sComSettings += "," + GetConnectionParams().GetStopBits() ;

	m_SPCommCtrl.put_InBufferSize(0);
	m_SPCommCtrl.put_OutBufferSize(0);
	m_SPCommCtrl.InitString(sComSettings);

	/* Connect to COM port */
	OnPortConnect();
	return TRUE;
}

long	CMVTerminalView::ReadScreenBuffer(char *pBuf, int nLen)
{
	CString sWinText;
	int     nWinTextLen,nReadLen;

	if(nLen == 0)
		return 0;

	GetRichEditCtrl().GetWindowText(sWinText);
	nWinTextLen = sWinText.GetLength();
	if(nWinTextLen <= nLen)
	{
		nReadLen = nWinTextLen;
	}
	else
	{
		sWinText = sWinText.Right(nLen);
		nReadLen = nLen;
	}
	strncpy(pBuf,(LPCSTR)sWinText,nReadLen);
	pBuf[nReadLen] = NULL;
	return nReadLen;
}

void CMVTerminalView::AppendEditModeData(CString sCh)
{
	switch(sCh.GetAt(0))
	{
	case '\b':
		if(m_sEditModeData.IsEmpty() == FALSE)
		{
			m_sEditModeData = m_sEditModeData.Left(m_sEditModeData.GetLength() - 1);
		}
		break;
	case VK_CTRL_C:
	case VK_CTRL_V:
		break;
	default:
		m_sEditModeData += sCh;
		break;
	}
}
bool CMVTerminalView::IsEditModeEnabled() 
{
	return m_bInEditMode;
}

void CMVTerminalView::ResetEditModeData()
{
	m_sEditModeData.Empty();
}

void CMVTerminalView::ExitEditMode() 
{
	//MessageBox("in exit edit mode "+ m_sEditModeData );
	m_bInEditMode  = false;
	m_nCursorIndex = 0;
	m_nStringLen = 0;
	ResetEditModeData();
	GetRichEditCtrl().SetReadOnly(TRUE);
}

void CMVTerminalView::IncEditModeDataCursorIndex() 
{
	//if(m_nCursorIndex < m_sEditModeData.GetLength())
	{
		// Allow cursor to move beyond string's first char
		m_nCursorIndex++;
	}
}

void CMVTerminalView::DecEditModeDataCursorIndex() 
{
	if(m_nCursorIndex > 0)
	{
		m_nCursorIndex--;
	}
}
void CMVTerminalView::EnterEditMode() 
{
	m_bInEditMode = true;
	m_nStringLen  = m_sEditModeData.GetLength();
	GetRichEditCtrl().SetReadOnly(FALSE);
}

void CMVTerminalView::BackspaceEditModeData()
{
	if(m_sEditModeData.IsEmpty() == FALSE)
	{
		int nIndex = m_sEditModeData.GetLength() - m_nCursorIndex - 1;
		// Allow delete only in string boudries
		if((nIndex >= 0) && (nIndex < m_sEditModeData.GetLength()))
			m_sEditModeData.Delete(nIndex ,1);
	}


}
 
void CMVTerminalView::DeleteEditModeData()
{
	if(m_sEditModeData.IsEmpty() == FALSE)
	{
		int nIndex = m_sEditModeData.GetLength() - m_nCursorIndex;
		// Allow delete only in string boudries
		if((nIndex >= 0) && (nIndex < m_sEditModeData.GetLength()))
			m_sEditModeData.Delete(nIndex ,1);

		DecEditModeDataCursorIndex();
	}

}

void CMVTerminalView::SetEditModeData(TCHAR ch)
{
	switch(ch)
	{
	case VK_DELETE:
		DeleteEditModeData();
		break;
	case VK_BACK:
		BackspaceEditModeData();
		break;
	case VK_CTRL_C:
	case VK_CTRL_V:
		break;
	default:
		m_sEditModeData.Insert(m_sEditModeData.GetLength() - m_nCursorIndex,ch);
		break;
	}
}

void CMVTerminalView::SetEditModeData(CString sStr)
{
	m_sEditModeData = sStr;
}


void CMVTerminalView::UpdateEditModeDataHistory()
{

}


void CMVTerminalView::SendEditModeData()
{
	// Move caret to end of line
	OnChar(VK_END,1,0);

	/* Incase string was updated with more chars the before 
	   since u-boot is updated only with m_nStringLen, we need 
	   to compensate with terminal delete */
	if(m_nStringLen < m_sEditModeData.GetLength())
	{
		DeleteText(GetRichEditCtrl().GetTextLength(), 
			m_sEditModeData.GetLength() - m_nStringLen);
	}

	// Move caret to end of line
	OnChar(VK_END,1,0);

	m_cmdHistoryLogger.SetEnable(FALSE);
	m_cmdHistoryLogger.AddCommand(m_sEditModeData);

	CString sVKBack((TCHAR)VK_BACK);
	for(int nBackspace = 0; nBackspace < m_nStringLen; nBackspace++)
	{
		//m_sEditModeData.Insert(0,VK_BACK);
		m_SPCommCtrl.WriteStr(sVKBack);
	}


	PasteString(m_sEditModeData);
	m_cmdHistoryLogger.Release();
	m_cmdHistoryLogger.SetEnable(TRUE);
}

BOOL CMVTerminalView::PreTranslateMessage(MSG* pMsg)
{
	
    if(pMsg->message == WM_COMMAND)
	{			
		//The high-order word specifies the notification code if the message is from a control. 
		//If the message is from an accelerator, this value is 1. If the message is from a menu, 
		//this value is zero. 
		//The low-order word specifies the identifier of the menu item, control, or accelerator.
		if(((((UINT)pMsg->wParam)>>16) & 0xFFFF) == 0)
		{
			SendHistoryMenuString((UINT)pMsg->wParam & 0xFFFF);
		}
	}
	
	// Handle meesages related to key down 
	if( pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			if(IsEditModeEnabled() == true)
			{
				SendEditModeData();
				ExitEditMode();
			}
			ResetEditModeData();
			OnChar(VK_RETURN,1,0);
			return TRUE;
			/* Handle backspace */
		case VK_BACK:
			if(IsEditModeEnabled() == true)
			{
				BackspaceEditModeData();
				/* Allow backspace to erase data from view */
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				if(m_sEditModeData.IsEmpty() == TRUE)
				{
					ExitEditMode();
					m_cmdHistoryLogger.SetEnable(TRUE);

				}
				return TRUE;
			}
			OnChar(VK_BACK,1,0);
			return TRUE;
		case VK_END:   
			m_nCursorIndex = 0;
			break;
		case VK_HOME:			
			break;
		case VK_UP:   
			OnChar(VK_UP,1,0);
			return TRUE;
		case VK_DOWN: 
			OnChar(VK_DOWN,1,0);
			return TRUE;
		case VK_LEFT:
			if(IsEditModeEnabled() == false)
			{
				EnterEditMode();
			}
			if(IsEditModeEnabled() == true)
			{
				IncEditModeDataCursorIndex();
			}
			break;
		case VK_RIGHT:
			if(IsEditModeEnabled() == true)
			{
				DecEditModeDataCursorIndex();
			}
			break;
		case VK_SHIFT:
			// Do nothing
			return TRUE;
			break;
		case VK_DELETE:
			if(IsEditModeEnabled() == true)
			{
				DeleteEditModeData();
				/* Allow backspace to erase data from view */
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				if(m_sEditModeData.IsEmpty() == TRUE)
				{
					ExitEditMode();
				}
				return TRUE;
			}
			break;

		case VK_ESCAPE: 
		case VK_CONTROL:
		    ::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		default:
			break;
		}
	}
	return CRichEditView::PreTranslateMessage(pMsg);
}


int  CMVTerminalView::GetSelTextAbsoluteIndex()
{
	// This function returns the
    // character position of the caret relative to the
    // first character in the entire window

	CPoint  charPoint;
	int		nLineFromChar;
	int		nSel, ai;

	// Send the EM_LINEFROMCHAR message with the value of
    // -1 in wParam.
    // The return value is the zero-based line number
    // of the line containing the caret.
	nLineFromChar = (int)SendMessage(EM_EXLINEFROMCHAR, (WPARAM)-1, (LPARAM)0);

	// Send the EM_GETSEL message to the ToolBox control.
    // The low-order word of the return value is the
    // character position of the caret relative to the
    // first character in the ToolBox control,
    // i.e. the absolute character index.
	nSel          = (int)SendMessage(EM_GETSEL, (WPARAM)0, (LPARAM)0);
	// get the low-order word from sel
	ai            = nSel & 0xffff;
	return ai;
}

int  CMVTerminalView::GetSelTextStartIndex()
{
	int		nCharIndex, nLineIndex;
	int		ai,nLastCharIndex;
	char	buf[512];
	
	// This function returns the
    // character position of the caret relative to the
    // first character in the entire window
	ai = GetSelTextAbsoluteIndex();

	// Send the EM_LINEINDEX message with the value of -1
    // in wParam.
    // The return value is the number of characters that
    // precede the first character in the line containing
    // the caret.
	nLineIndex    = (int)SendMessage(EM_LINEINDEX, (WPARAM)-1, (LPARAM)0);

	// Subtract the li (line index) from the ai
    // (absolute character index),
    // The result is the column number of the caret position
    // in the line containing the caret.
	nCharIndex = ai - nLineIndex;

	nLastCharIndex = GetRichEditCtrl().GetLine(GetRichEditCtrl().GetLineCount()-1,buf,512) - 1;
	return  nLastCharIndex - nCharIndex;
    
}

BOOL  CMVTerminalView::IsCursorInEditMode()
{
	int		nSelTextStartIndex;

	nSelTextStartIndex = GetSelTextStartIndex();
	if(nSelTextStartIndex < m_nCursorIndex)
		return TRUE;
	return FALSE;	
}

int CMVTerminalView::GetSelLineIndex()
{
    int ai = GetSelTextAbsoluteIndex();
	return (int)SendMessage(EM_LINEFROMCHAR, (WPARAM)ai, (LPARAM)0);
}

BOOL CMVTerminalView::OnCharHandleSelectedText(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	CString    sData,ch((TCHAR)nChar),sTmp, sHistoryCmd ;
	int        nLen;		
			
	/* Get selected text */
	sTmp = GetRichEditCtrl().GetSelText();
	if((nLen = sTmp.GetLength()) != 0)
	{
		// Allow selection edit in last line only
		if(GetSelLineIndex() != (GetRichEditCtrl().GetLineCount() - 1))
		{
			if(IsEditModeEnabled() == true)
				ExitEditMode();

			return FALSE;
		}

		/* Set edit mode cursor index to selection position */ 
		m_nCursorIndex = GetSelTextStartIndex();

		if(IsEditModeEnabled() == false)
			EnterEditMode();

		// Delete selected text from edit mode data
		SendMessage(WM_KEYDOWN,VK_DELETE,0);
		for(int i = 0; i < nLen; i++)
		{
			DeleteEditModeData();
		}
		OnChar((TCHAR)nChar,1,0);

		return TRUE;


	}
	return FALSE;
}

BOOL CMVTerminalView::OnCharHandleEditMode(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString    ch((TCHAR)nChar);

	if(IsEditModeEnabled() == true)
	{
		if((ch == VK_BACK) || (ch == VK_DELETE) || 
		   (ch == VK_END)  || (ch == VK_HOME)   ||
		   (ch == VK_UP)   || (ch == VK_DOWN)   ||
		   (ch == VK_LEFT) || (ch == VK_RIGHT))
			return TRUE;

		SetEditModeData((TCHAR)nChar);
		CRichEditView::OnChar(nChar,nRepCnt,nFlags);
		return TRUE;
	}
	else
	{
		// Log to edit mode data
		if(ch != VK_RETURN)
		{
			AppendEditModeData(ch);
		}
	}
	return FALSE;
}

void CMVTerminalView::OnCharHandleVKUPChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	CString    sHistoryCmd ;

	if(GetUseTerminalHistory() == TRUE)
	{
		sHistoryCmd = m_cmdHistoryLogger.GetCommandByDirection(NEXT_COMMAND);
		if(sHistoryCmd.IsEmpty() == FALSE)
		{
			PasteString(sHistoryCmd);
			ExitEditMode();
			// Remove VK_BACK inserted by GetCommandByDirection
			sHistoryCmd.Remove(VK_BACK);
			SetEditModeData(sHistoryCmd);
		}
	}
	else
	{
		CString sUpArrow;
		sUpArrow += TCHAR(0x1b);
		sUpArrow += TCHAR(0x5b);
		sUpArrow += TCHAR(0x41);
		m_SPCommCtrl.WriteStr((LPCTSTR)sUpArrow);
	}
}
void CMVTerminalView::OnCharHandleVKDOWNChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString    sHistoryCmd ;

	if(nFlags == 0) // If flags = 0xa than the key is SHIFT+9 AND NOT VK_DOWN
	{
		if(GetUseTerminalHistory() == TRUE)
		{
			sHistoryCmd = m_cmdHistoryLogger.GetCommandByDirection(PREVIOUS_COMMAND);
			if(sHistoryCmd.IsEmpty() == FALSE)
			{
				PasteString(sHistoryCmd);
				ExitEditMode();
				// Remove VK_BACK inserted by GetCommandByDirection
				sHistoryCmd.Remove(VK_BACK);
				SetEditModeData(sHistoryCmd);
			}
		}
		else
		{
			CString sDownArrow;
			sDownArrow += TCHAR(0x1b);
			sDownArrow += TCHAR(0x5b);
			sDownArrow += TCHAR(0x42);
			m_SPCommCtrl.WriteStr((LPCTSTR)sDownArrow);
		}
	}
}
void CMVTerminalView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString    sData,ch((TCHAR)nChar),sTmp, sHistoryCmd ;

	// Set auto vscroll incase its disabled
	SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_OR, (LPARAM)ECO_AUTOVSCROLL);
	switch(nChar)
	{
	case VK_CTRL_C:
		sTmp = GetRichEditCtrl().GetSelText();
		if(sTmp.GetLength() > 0)
		{
			//GetRichEditCtrl().Copy();
			//SendMessage(WM_COPY,0,0);
			CRichEditView::OnEditCopy();
			return;
		}
		break;
	case VK_CTRL_V:
		OnEditPaste();
		return;
	case VK_LEFT:
	case VK_RIGHT:
		break;
	case VK_UP:
		OnCharHandleVKUPChar(nChar, nRepCnt, nFlags);
		return;
	case VK_DOWN:
		OnCharHandleVKDOWNChar(nChar, nRepCnt, nFlags);
		return;
	default:
		break;
	}
	
	// Incase text is selected 
	if(OnCharHandleSelectedText(nChar, nRepCnt, nFlags) == TRUE)
		return;

	// Log the char 
	m_cmdHistoryLogger.GetInput(ch);

	////////////////////////////////
	// Dont send chars in edit mode
	if(OnCharHandleEditMode(nChar, nRepCnt, nFlags) == TRUE)
		return;
	//////////////////////////////

	// Send the char...
	m_SPCommCtrl.WriteStr((LPCTSTR)(ch));

}


void CMVTerminalView::SendHistoryMenuString(UINT nMenuIndex)
{
	if((nMenuIndex - WM_CLEAR_HISTORY - 1) >= 0)
	{
		//PasteString(m_cmdHistoryLogger.GetCommandByIndex(nMenuIndex - WM_CLEAR_HISTORY - 1));
		PasteString(GetHistoryMenuItem(nMenuIndex - WM_CLEAR_HISTORY - 1));
	}
}

void CMVTerminalView::PasteString(CString txt)
{
	int	 len = txt.GetLength();

	for(int i = 0; i < len; i++)
	{
		CString    ch((TCHAR)txt[i]);
		if(m_SPCommCtrl.get_IsOpened() == TRUE)
		{
			m_SPCommCtrl.WriteStr((LPCTSTR)ch);
			// Log the char 
			m_cmdHistoryLogger.GetInput(ch);
		}
	}
}

void  CMVTerminalView::SetWindowTitle(CString  sWndTitle)
{
	CString sComPort;

	sComPort = GetConnectionParams().GetComPort();

	if(sWndTitle.IsEmpty())
		sWndTitle = sComPort;
	else
	{
		sWndTitle += " (";
		sWndTitle += sComPort;
		sWndTitle += ")";
	}
	GetDocument()->SetTitle(sWndTitle);
}

CString  CMVTerminalView::GetWindowTitle()
{
	return GetDocument()->GetTitle();
}

void CMVTerminalView::OnViewProperties()
{
	//CViewPropertiesDlg  viewPropertiesDlg;

	if(m_pViewPropertiesDlg == NULL)
	{
        MessageBox("Failed to open properties dialog!",NULL,MB_ICONERROR);
		return;
	}
    
	m_pViewPropertiesDlg->SetDlgType(PROPERTIES_DLG_TYPE);
	m_pViewPropertiesDlg->CenterWindow(this);

	//viewPropertiesDlg.SetDlgType(PROPERTIES_DLG_TYPE);
	//viewPropertiesDlg.GetConnectionSettingsTab().SetDlgType(PROPERTIES_DLG_TYPE);
	//viewPropertiesDlg.CenterWindow(this);

	if(m_pViewPropertiesDlg->DoModal() != IDOK)
		return;

	//if(viewPropertiesDlg.DoModal() != IDOK)
	//	return;

	CConnectionParams  connectionParams = m_pViewPropertiesDlg->GetConnectionSettingsTab().GetConnectionParams();
	//CConnectionParams  connectionParams = viewPropertiesDlg.GetConnectionSettingsTab().GetConnectionParams();
	if(connectionParams == GetConnectionParams())
	{
		//Nothing changed...
	}
	else
	{
		/* Re-activate the COM port with new settings */
		if(ActivateComPort(connectionParams) == FALSE)
		{
			MessageBox("Failed to re-activate " + m_pViewPropertiesDlg->GetConnectionSettingsTab().GetConnectionParams().GetComPort(),NULL,MB_ICONERROR);
			//MessageBox("Failed to re-activate " + viewPropertiesDlg.GetConnectionSettingsTab().GetConnectionParams().GetComPort(),NULL,MB_ICONERROR);
			return;
		}
		// Update wnd name
		CString sWndName = m_pViewPropertiesDlg->GetConnectionSettingsTab().GetConnectionParams().GetWndName();
		//CString sWndName = viewPropertiesDlg.GetConnectionSettingsTab().GetConnectionParams().GetWndName();
		GetConnectionParams().SetWndName(sWndName);
		SetWindowTitle(sWndName);
	}

	SetBootLoaderParams(m_pViewPropertiesDlg->GetBootLoaderParams());
	//SetBootLoaderParams(viewPropertiesDlg.GetBootLoaderParams());

	// Update History settings
	SetMaxHistoryCmdCount(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetMaxHistoryCount());
	SetCmdMinLenThreshold(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetCmdMinLenThreshold());
	SetCmdMaxLenThreshold(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetCmdMaxLenThreshold());
	SetUseTerminalHistory(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetUseTerminalHistory());
	SetCmdHistoryFilter(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetFilter());

	//SetMaxHistoryCmdCount(viewPropertiesDlg.GetCmdHistorySettingsTab().GetMaxHistoryCount());
	//SetCmdMinLenThreshold(viewPropertiesDlg.GetCmdHistorySettingsTab().GetCmdMinLenThreshold());
	//SetCmdMaxLenThreshold(viewPropertiesDlg.GetCmdHistorySettingsTab().GetCmdMaxLenThreshold());
	//SetUseTerminalHistory(viewPropertiesDlg.GetCmdHistorySettingsTab().GetUseTerminalHistory());
	//SetCmdHistoryFilter(viewPropertiesDlg.GetCmdHistorySettingsTab().GetFilter());

	// Update BootLoader changes on file list display
	if(m_pMainFrame != NULL)
	{
		(m_pMainFrame->GetTFTPServerPane())->UpdateChanges();
	}
}


void CMVTerminalView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UINT   scrollLimit, scrollPos;

	switch(nSBCode)
	{
	case SB_BOTTOM://   Scroll to bottom.
		break;
	case SB_ENDSCROLL: //   End scroll.
		break;
	case SB_LINEDOWN: //   Scroll one line down.
		break;
	case SB_LINEUP://   Scroll one line up.
		break;
	case SB_PAGEDOWN: //   Scroll one page down.
		break;
	case SB_PAGEUP: //   Scroll one page up.
		break;
	case SB_THUMBPOSITION://   Scroll to the absolute position. The current position is provided in nPos.
		break;
	case SB_THUMBTRACK:   //Drag scroll box to specified position. The current position is provided in nPos.
		scrollLimit = GetScrollLimit(SB_VERT);
		scrollPos = GetScrollPos(SB_VERT);
		if(nPos < (scrollLimit-(scrollLimit/10)))
		{
			SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_AND, (LPARAM)(~(ECO_AUTOVSCROLL)));
		}
		else
		{
			SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_OR, (LPARAM)(ECO_AUTOVSCROLL));
		}
		   
		break;
	case SB_TOP: //   Scroll to top
		break;
	}

	CRichEditView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void	CMVTerminalView::OnClearHistory()
{
	m_cmdHistoryLogger.ClearHistoryList();
}

void	CMVTerminalView::OnViewWindowContent()
{
	CString   sFilePath;
	DumpScreenBufferToFile(sFilePath);
	ShellExecute(NULL, "open", "notepad.exe",sFilePath ,"", SW_SHOWDEFAULT);
}

void CMVTerminalView::CaptureToFile(CString str)
{
    str.Replace("\r","\n");
	m_captureFile.WriteString(str);
}

void	CMVTerminalView::DumpScreenBufferToFile(CString& sFilePath)
{
	CString  sScreenBufferContent,sPath;

	sPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	GetWindowText(sScreenBufferContent);
	CStdioFile file;
	if( !file.Open(sPath +  "\\ScreenBufferContent.txt", CFile::modeCreate
		   | CFile::modeWrite | CFile::typeText ) ) 
	{
		MessageBox("Error in opening file " + sPath +  "\\ScreenBufferContent.txt",NULL,MB_ICONERROR);
		return ;
	}
	sFilePath = sPath +  "\\ScreenBufferContent.txt";
	file.WriteString(sScreenBufferContent);
	file.Close();
}


BOOL CMVTerminalView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT   scrollLimit, scrollPos;

	scrollLimit = GetScrollLimit(SB_VERT);
	scrollPos = GetScrollPos(SB_VERT);
	if(scrollPos < (scrollLimit-(scrollLimit/10)))
	{
		SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_AND, (LPARAM)(~(ECO_AUTOVSCROLL)));
	}
	else
	{
		SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_OR, (LPARAM)(ECO_AUTOVSCROLL));
	}


	return CRichEditView::OnMouseWheel(nFlags, zDelta, pt);
}


UINT ReadPortDataThread(LPVOID pParam)
{
	CMVTerminalView  *pMTView= (CMVTerminalView *)pParam;
	CString     str;

	while(pMTView->GetSPCommCtrl().get_InCount() > 0)
	{		
		if(pMTView->GetThreadStatus() == MVT_END_THREAD)
		{
			AfxEndThread(0);
		}

		Sleep(15); 

		if(pMTView->GetSPCommCtrl().get_IsOpened() == TRUE)
		{
			str = pMTView->GetSPCommCtrl().ReadStr();
		}
	
		if( str.IsEmpty() == FALSE ) 
		{
			pMTView->UpdateView(str);
		}
		
		Sleep(15); 
	}
	
	::PostMessage(pMTView->m_hWnd,WM_THREAD_DONE,0,0); 
	return 0;
}

LRESULT	CMVTerminalView::OnThreadDone(WPARAM a, LPARAM b)
{
	SetThreadStatus(MVT_THREAD_IS_NOT_ACTIVE);
	return 0;
}

void CMVTerminalView::UpdateView(CString pchData)
{
	CString          str;
	int              nCharIndex, nBackcount = 0,nLineCount, nCurrLineCount, nReduceCount ;

	int len =  GetRichEditCtrl().GetWindowTextLength();
	if ((len >= MAX_CHAR_COUNT) || ((nCurrLineCount = GetRichEditCtrl().GetLineCount()) > MAX_LINE_COUNT))
	{
		nReduceCount = len%MAX_CHAR_COUNT;
		if(nCurrLineCount > MAX_LINE_COUNT)
		{
			nReduceCount = (nReduceCount > 1024)?  1024:nReduceCount;
		}
		else
		{
			nReduceCount = (nReduceCount < 6000)?  6000:nReduceCount;
		}
		RemoveBufferTail(nReduceCount);
		len -= nReduceCount;
	}
	str = pchData;
	str.Replace("\r\r","\r");
	// CR is enough in Richdit 2.0 for line break
	str.Remove(CHAR_LF);
	// Handle backspace characters.
	while ((nCharIndex = str.Find(TCHAR(VK_BACK))) != -1)
	{
		if (nCharIndex == 0)
		{
			nBackcount++;
			str = str.Mid(nCharIndex+1);
		}
		else
		{
			if(str[nCharIndex-1] != CHAR_CR) 
			{
				str = str.Left(nCharIndex-1) + str.Mid(nCharIndex+1);
			}
			else
			{
				// Do not erase CR+LF
				str = str.Left(nCharIndex) + str.Mid(nCharIndex+1);
			}
		}
	}
	// Set text insertion point according to number of backspaces
	// at the begining of str.
	if (nBackcount>len)
		nBackcount = len;

	// Fix nBackCount so it wont erase previous lines
	if((nLineCount = GetRichEditCtrl().GetLineCount()) > 0)
	{
		char    lastLine[2048];
		CString lastLineStr;
		int     lastLineLen;
		 
		GetRichEditCtrl().GetLine(nLineCount - 1,lastLine,2048);
		lastLineStr = lastLine;
		lastLineStr.Remove(CHAR_LF);
		lastLineStr.Remove(CHAR_CR);
		if((lastLineLen = lastLineStr.GetLength()) < nBackcount)
			nBackcount = lastLineLen;
	}
	 
	// Delete text...
	if(nBackcount > 0)
		DeleteText(len,nBackcount);
	 
	if(m_pViewPropertiesDlg->GetViewMessagingDlg().GetMessagingEn() == TRUE)
	{
		int nIndex = str.Find(VK_ESCAPE,0);
		// Handle half message (such as ",1,2,3,4,aa,ss,dd,ff]")
		if((m_sIPMMsg.IsEmpty() == FALSE) && (nIndex == -1))
		{
			HandleIPMMessage(str);
		}
		// Handle MVT messages
		while( str.Find(VK_ESCAPE,0) != -1 )
		{
			HandleIPMMessage(str);
		}
	}

	str.Remove(CHAR_TAB_ECHO);
	// Look for ESC char that starts VT100 control sequance
	// Remove VT100 control sequance - currently no support for VT100 
	if(str.Find(VK_ESCAPE,0) != -1)
		str = RemoveVT100ControlSeq(str); 

	// Append new text
	AppendText(len,str);
	 
	// Capture text to file
	if(m_bCaptureToFile == TRUE)
		CaptureToFile(str);
}


CString CMVTerminalView::RemoveVT100ControlSeq(CString sStr)
{
	CString sNewStr;
	static  bool bAppend = true, bEscFound = false;
	int		i,nLen = sStr.GetLength(),nTimeOutCount = 0;

	for(i = 0; i < nLen; i++)
	{
		switch(sStr[i])
		{
		case VK_ESCAPE:
			bAppend   = false;
			bEscFound = true;
			nTimeOutCount = 0;
			break;
		// The following chars represent closure of the sequance
		case ')':
		case '(':
		case 'c':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'l':
		case 'm':
		case 'n':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 'u':
		case 'y':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'H':
		case 'J':
		case 'K':
		case 'M':
		case 'N':
		case 'O':
		case 'R':
			if(bEscFound == true)
			{
				bEscFound = false;
				bAppend = true;
			}
			else
			{
				if(bAppend == true)
					sNewStr += sStr[i];
			}
			break;
		default:
			if(bAppend == true) 
			{
				sNewStr += sStr[i];
			}
			else
			{
				// Timeout incase of en error...
				nTimeOutCount++;
				if(nTimeOutCount == 20)
				{
					bEscFound = false;
					bAppend = true;
					nTimeOutCount = 0;
				}
			}
			break;
		}
	}

	return sNewStr;

}

BOOL CMVTerminalView::HandleIPMMessage(CString& sStr)
{
	int				nIndex = 0;
	CString			sStrPrefix, sStrPostfix, sCh;

	if(m_sIPMMsg.IsEmpty() == TRUE)
	{
		if((nIndex = sStr.Find(VK_ESCAPE)) == -1)
			return FALSE;

		sStrPrefix = sStr.Left(nIndex);
	}

	for(; nIndex < sStr.GetLength(); nIndex++)
	{
		m_sIPMMsg += sStr[nIndex];
		if(m_sIPMMsg.GetLength() >= 5)
		{
			//if(m_sIPMMsg.Find("[MVT") == -1)
			//{
			//	sStrPostfix = sStr.Right(sStr.GetLength() - nIndex - 1);
			//	sStr  = sStrPrefix;
			//	sStr += sStrPostfix;
			//	m_sIPMMsg.Empty();
			//	return TRUE;
			//}
		}
		if(sStr[nIndex] == ']')
			break;
	}
	
	sStrPostfix = sStr.Right(sStr.GetLength() - nIndex - 1);

	sStr  = sStrPrefix;
	sStr += sStrPostfix;
	if(m_sIPMMsg.Find("]") != -1)
	{
		CIPMMessage *pMsg = new CIPMMessage();
		if( CreateIPMMessage(m_sIPMMsg,pMsg) == TRUE)
		{
			m_pMainFrame->OnIPMMessage(pMsg);
		}
		delete pMsg;
		m_sIPMMsg.Empty();
	}

	// Incase message postfix never come and string gets big...
	if(m_sIPMMsg.GetLength() >= 256)
	{
		m_sIPMMsg.Empty();
	}
	return TRUE;
}

BOOL CMVTerminalView::CreateIPMMessage(CString sMsg, CIPMMessage* pMsg)
{
	int		  nIndex,nParam;
	UINT	  nMsgType;
	CString	  sProcessName,sMessageType;
	CString	  sIntParam[4];
	CString	  sStrParam[4];

	if(pMsg == NULL)
	{
		MessageBox("pMsg == NULL");
		return FALSE;
	}

	pMsg->SetSourceComPort(GetConnectionParams().GetComPort());

	// Remove msg prefix "ESC[MVT,"
	nIndex  = sMsg.Find(',');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sMsg);
		return FALSE;
	}
	sMsg = sMsg.Right(sMsg.GetLength() - nIndex - 1);

	// Extract ProcessName
	nIndex  = sMsg.Find(',');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sMsg);
		return FALSE;
	}

	sProcessName = sMsg.Left(nIndex);
	sProcessName.Remove(',');
	sMsg      = sMsg.Right(sMsg.GetLength() - nIndex - 1);

	// Extract MessageType
	nIndex  = sMsg.Find(',');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sMsg);
		return FALSE;
	}
	sMessageType = sMsg.Left(nIndex + 1);
	sMessageType.Remove(',');
	sMsg         = sMsg.Right(sMsg.GetLength() - nIndex - 1);

	// Extract sIntParam[x]
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		nIndex = sMsg.Find(',');		
		if(nIndex == -1)
		{
			MessageBox("Error in parsing message " + sMsg);
			return FALSE;
		}
		sIntParam[nParam] = sMsg.Left(nIndex);
		sIntParam[nParam].Remove(',');
		sMsg    = sMsg.Right(sMsg.GetLength() - nIndex - 1);
	}
	// Extract sStrParam[x]
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT - 1 /* Last one ends with ']' */; nParam++)
	{
		nIndex = sMsg.Find(',');		
		if(nIndex == -1)
		{
			MessageBox("Error in parsing message " + sMsg);
			return FALSE;
		}
		sStrParam[nParam] = sMsg.Left(nIndex);
		sStrParam[nParam].Remove(',');
		sMsg    = sMsg.Right(sMsg.GetLength() - nIndex - 1);
	}

	// Extract last sStrParam
	nIndex  = sMsg.Find(']');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sMsg);
		return FALSE;
	}
	sStrParam[IPM_MAX_PARAMS_COUNT-1] = sMsg.Left(nIndex);
	sStrParam[IPM_MAX_PARAMS_COUNT-1].Remove(']');

	/* Set pMsg with values */
	sscanf(sMessageType,"%x",&nMsgType);
	pMsg->SetProcessName((LPTSTR)(LPCTSTR)sProcessName);
	pMsg->SetMessage((IPM_MESSAGE_TYPE)nMsgType);
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		UINT nIntParam;
		sscanf(sIntParam[nParam],"%x",&nIntParam);
		pMsg->SetIntParam(nParam,nIntParam);
	}
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		pMsg->SetStringParam(nParam,(LPTSTR)(LPCTSTR)sStrParam[nParam]);
	}
	return TRUE;
}

#if 0
BOOL CMVTerminalView::GetIPMMessage(CString& sStr, CIPMMessage* pMsg)
{
	int		  nIndex,nParam;
	UINT	  nMsgType;
	CString   sRetStr, sMsg;
	CString	  sMVTMessagePrefix = "[MVT,";
	CString	  sProcessName,sMessageType;
	CString	  sIntParam[4];
	CString	  sStrParam[4];

	sMVTMessagePrefix.Insert(0,VK_ESCAPE);
	// MVT Message format = ESC[MVT,ProcessName.exe,MessageType,LParam,WParam]
	if((((nIndex = sStr.Find(sMVTMessagePrefix)) == -1)) || ( sStr.Find("]") == -1  ))
		return FALSE;

	sRetStr = sStr.Left(nIndex);
	sRetStr = sRetStr + sStr.Right(sStr.GetLength() - sStr.ReverseFind(']') - 1);

	// Found a message - remove prefix
	sStr = sStr.Right(sStr.GetLength() - nIndex - sMVTMessagePrefix.GetLength());	

	// Extract ProcessName
	nIndex  = sStr.Find(',');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sStr);
		sStr = sRetStr;
		return FALSE;
	}

	sProcessName = sStr.Left(nIndex);
	sProcessName.Remove(',');
	sStr      = sStr.Right(sStr.GetLength() - nIndex - 1);

	// Extract MessageType
	nIndex  = sStr.Find(',');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sStr);
		sStr = sRetStr;
		return FALSE;
	}
	sMessageType = sStr.Left(nIndex + 1);
	sMessageType.Remove(',');
	sStr         = sStr.Right(sStr.GetLength() - nIndex - 1);

	// Extract sIntParam[x]
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		nIndex = sStr.Find(',');		
		if(nIndex == -1)
		{
			MessageBox("Error in parsing message " + sStr);
			sStr = sRetStr;
			return FALSE;
		}
		sIntParam[nParam] = sStr.Left(nIndex);
		sIntParam[nParam].Remove(',');
		sStr    = sStr.Right(sStr.GetLength() - nIndex - 1);
	}
	// Extract sStrParam[x]
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT - 1 /* Last one ends with ']' */; nParam++)
	{
		nIndex = sStr.Find(',');		
		if(nIndex == -1)
		{
			MessageBox("Error in parsing message " + sStr);
			sStr = sRetStr;
			return FALSE;
		}
		sStrParam[nParam] = sStr.Left(nIndex);
		sStrParam[nParam].Remove(',');
		sStr    = sStr.Right(sStr.GetLength() - nIndex - 1);
	}

	// Extract last sStrParam
	nIndex  = sStr.Find(']');
	if(nIndex == -1)
	{
		MessageBox("Error in parsing message " + sStr);
		sStr = sRetStr;
		return FALSE;
	}
	sStrParam[IPM_MAX_PARAMS_COUNT-1] = sStr.Left(nIndex);
	sStrParam[IPM_MAX_PARAMS_COUNT-1].Remove(']');
	sStr = sRetStr;

	/* Set pMsg with values */
	sscanf(sMessageType,"%x",&nMsgType);
	pMsg->SetProcessName((LPTSTR)(LPCTSTR)sProcessName);
	pMsg->SetMessage((IPM_MESSAGE_TYPE)nMsgType);
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		UINT nIntParam;
		sscanf(sIntParam[nParam],"%x",&nIntParam);
		pMsg->SetIntParam(nParam,nIntParam);
	}
	for(nParam = 0; nParam < IPM_MAX_PARAMS_COUNT; nParam++)
	{
		pMsg->SetStringParam(nParam,(LPTSTR)(LPCTSTR)sStrParam[nParam]);
	}
	return TRUE;
}

#endif


void CMVTerminalView::AppendText(int lastCharIndex, CString txt)
{	
	GetRichEditCtrl().SetSel(lastCharIndex,lastCharIndex);
	GetRichEditCtrl().ReplaceSel(txt);
}

void CMVTerminalView::DeleteText(int nStartIndex, int nCount)
{
	// Set back to R/W mode	
	SendMessage(EM_SETREADONLY, FALSE, 0);
	
	// Move caret to end
	GetRichEditCtrl().SetSel(nStartIndex,nStartIndex);
	for(int i = 0; i < nCount; i++)
		SendMessage(WM_KEYDOWN,VK_BACK,0);

	// Move back to Read Only	
    SendMessage(EM_SETREADONLY, TRUE, 0L);

}

void CMVTerminalView::RemoveBufferTail(int nReduceCount)
{
	// Disable auto vscroll
	SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_AND, (LPARAM)(~(ECO_AUTOVSCROLL)));

	// Delete text
	GetRichEditCtrl().SetSel(0,nReduceCount);
	GetRichEditCtrl().ReplaceSel("");
	
	// Enable back auto vscroll
	SendMessage(EM_SETOPTIONS, (WPARAM)ECOOP_OR, (LPARAM)(ECO_AUTOVSCROLL));

	// Align the text to the buttom of window
	SendMessage(EM_SCROLL,SB_PAGEDOWN,0);
	

}

MVT_THREAD_STATUS CMVTerminalView::GetThreadStatus()
{
	return m_threadStatus;
}

void CMVTerminalView::SetThreadStatus(MVT_THREAD_STATUS status)
{
	m_threadStatus = status;
}

void  CMVTerminalView::SendTextLine(CTextLine  txtLine)
{
	Sleep(txtLine.GetDelayBefore());
	PasteString(txtLine.GetLine());
	if(txtLine.HasNewLine() == TRUE)
		PasteString(CString(CHAR_CR));
	Sleep(txtLine.GetDelayAfter());
}


void CMVTerminalView::LoadScript(CTextFile  *pScript)
{
	if(pScript == NULL)
		return;

	m_loadScriptParams.SetParams(this,pScript);
	AfxBeginThread(LoadScriptThread,&m_loadScriptParams);
}

UINT LoadScriptThread(LPVOID pParam)
{
	CMVTerminalView::CLoadScriptParams  *pLoadScriptParams = (CMVTerminalView::CLoadScriptParams*)pParam;
	int  								 nLine;		
	CTextLine                           *pTextLine;
	
	pLoadScriptParams->GetView()->GetCmdHistoryLogger().SetEnable(FALSE);
	for(nLine = 0 ; nLine < pLoadScriptParams->GetScript()->GetLineCount(); nLine++)
	{
		// Set default delay
		Sleep(50);
		pTextLine = pLoadScriptParams->GetScript()->GetTextLine(nLine);
		if(pTextLine != NULL)
		{
			CString sLine = pTextLine->GetStripLine();
			if(sLine.IsEmpty() == FALSE)
			{
				pLoadScriptParams->GetView()->PasteString(sLine);
				
				if(pTextLine->HasNewLine() == TRUE)
					pLoadScriptParams->GetView()->PasteString(CString(CHAR_CR));

				Sleep(pLoadScriptParams->GetScript()->GetDelayVal());
			}
		}
		else
		{
			AfxMessageBox("Invalid script " + pLoadScriptParams->GetScript()->GetName() +". Aborting loading",MB_ICONERROR);
			AfxEndThread(0,TRUE);
		}
	}
	::PostMessage((pLoadScriptParams->GetScript()->GetWnd())->m_hWnd,WM_LOAD_SCRIPT_THREAD_ENDED,0,0);
	pLoadScriptParams->GetView()->GetCmdHistoryLogger().SetEnable(TRUE);
	return 0;
}

void  CMVTerminalView::SetBootLoaderParams(CBootLoaderParams bootLoaderParams)
{
	m_bootLoaderParams = bootLoaderParams;
}

void CMVTerminalView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString     sSelection;
	
	CRichEditView::OnLButtonDblClk(nFlags, point);
	sSelection = GetRichEditCtrl().GetSelText();
	if(sSelection.IsEmpty() == TRUE)
		return;

	CDataAnalyzerPane *pDataAnalyzerPane = m_pMainFrame->GetDataAnalyzerPane();
	if(pDataAnalyzerPane == NULL)
		return;
	
	pDataAnalyzerPane->Analyze(sSelection);
}

void CMVTerminalView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString     sSelection;

	CRichEditView::OnLButtonUp(nFlags, point);
	sSelection = GetRichEditCtrl().GetSelText();
	if(sSelection.IsEmpty() == TRUE)
		return;

	CDataAnalyzerPane *pDataAnalyzerPane = m_pMainFrame->GetDataAnalyzerPane();
	if(pDataAnalyzerPane == NULL)
		return;
	
	pDataAnalyzerPane->Analyze(sSelection);
}

void CMVTerminalView::LoadTextFile(CString sFile)
{
	CString		 sLine;
	FILE	    *pTextFile;
    TCHAR   	 cLine[1024];

	// Use this method of open file since we need newline char on read
    if((pTextFile = fopen( sFile, "rt" )) == NULL )
    {		
		MessageBox("Can't open file "+sFile,NULL,MB_ICONERROR);
		return;
	}

	// Clear previous loads
	m_loadTextFileParams.m_textFile.Clear();
	// Read lines INCLUDING newline char
    while(fgets( cLine, 1024, pTextFile ) != NULL)
	{
		sLine = cLine;
		// Remove newline char - indication for newline
		m_loadTextFileParams.m_textFile.AddLine(sLine,sLine.Remove(CHAR_LF));
	}
	fclose( pTextFile );
	m_loadTextFileParams.m_textFile.SetDelay("0");
	m_loadTextFileParams.SetView(this);
	AfxBeginThread(LoadTextFileThread,&m_loadTextFileParams);
}

UINT LoadTextFileThread(LPVOID pParam)
{
	CMVTerminalView::CLoadTextFileParams  *pLoadTextFileParams = (CMVTerminalView::CLoadTextFileParams*)pParam;
	int  								 nLine,nLineCount;		
	CTextLine                           *pTextLine;
	UINT								 nDelay = pLoadTextFileParams->m_textFile.GetDelayVal();
	CMVTerminalView						*pView = pLoadTextFileParams->GetView();
	CTextFile							*pTextFile = &pLoadTextFileParams->GetFile();

	// Disable history
	pView->GetCmdHistoryLogger().SetEnable(FALSE);
	nLineCount = pTextFile->GetLineCount();

	for(nLine = 0 ; nLine < nLineCount; nLine++)
	{
		pTextLine = pTextFile->GetTextLine(nLine);
		if(pTextLine != NULL)
		{
			pView->PasteString(pTextLine->GetLine());
			if(pTextLine->HasNewLine() == TRUE)
				pView->PasteString(CString(CHAR_CR));
			Sleep(nDelay);
		}
		else
		{
			AfxMessageBox("Invalid text file " + pTextFile->GetName() +". Aborting loading",MB_ICONERROR);
			AfxEndThread(0,TRUE);
		}
	}
	// Enable history
	pView->GetCmdHistoryLogger().SetEnable(TRUE);
	return 0;
}



HMENU CMVTerminalView::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg)
{
    CPoint   point;
	int		 nMenuItemIndex = 0;
	

	int nCmdCount = m_cmdHistoryLogger.GetCommandCount(), nCmd;
		
	// Open Menu at Mouse position
	::GetCursorPos(&point);
	m_cmdHistoryMenu.CreatePopupMenu();
	
	/* Create the copy menu item */
	CBitmap bmpImage;
	bmpImage.LoadBitmap(IDB_MENU_COPY_BITMAP); 
	if(GetRichEditCtrl().GetSelText().IsEmpty() == TRUE)
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++,MF_BYPOSITION | MF_DISABLED| MF_GRAYED, ID_EDIT_COPY ,  "&Copy\tCtrl+C",&bmpImage);
	else
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++,MF_BYPOSITION  ,ID_EDIT_COPY,  "&Copy\tCtrl+C",&bmpImage);
	bmpImage.DeleteObject();

	/* Create the paste menu item */
	CString sFromClipboard;
	if ( OpenClipboard() ) 
	{
		HANDLE hData = ::GetClipboardData( CF_TEXT );
		char * pBuffer = (char*)GlobalLock( hData );
		sFromClipboard = pBuffer;
		GlobalUnlock( hData );
		CloseClipboard();
	}
	bmpImage.LoadBitmap(IDB_MENU_PASTE_BITMAP); 
	if(sFromClipboard.IsEmpty() == TRUE)
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++,MF_BYPOSITION | MF_DISABLED| MF_GRAYED,ID_EDIT_PASTE ,  "&Paste\tCtrl+V",&bmpImage);
	else
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++,MF_BYPOSITION  ,ID_EDIT_PASTE ,  "&Paste\tCtrl+V",&bmpImage);
	bmpImage.DeleteObject();

 
	/* Create the history menu item */
	if(nCmdCount > 0)
	{	
		// Initialize menu with history list
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++, MF_BYPOSITION | MF_SEPARATOR);
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++, MF_BYPOSITION | MF_STRING, WM_CLEAR_HISTORY , "Clear History");
		m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++, MF_BYPOSITION | MF_SEPARATOR);
		
		ClearHistoryMenuItems();
		for(nCmd = 0; nCmd < nCmdCount; nCmd++)
		{
			AddHistoryMenuItem( m_cmdHistoryLogger.GetCommandByIndex(nCmd) );
		}	
		
		vector<CString>::iterator iter, endIter;
		iter    = m_historyMenuItems.begin();
		endIter = m_historyMenuItems.end();
		nCmd = 0;
		for(; iter != endIter; iter++)
		{
			m_cmdHistoryMenu.InsertMenu(nMenuItemIndex++, MF_ENABLED | MF_BYPOSITION | MF_STRING, WM_CLEAR_HISTORY + nCmd + 1, (*iter));
			nCmd++;
		}
	}
	m_cmdHistoryMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

    return CRichEditView::GetContextMenu(seltyp, lpoleobj, lpchrg);

}
CString CMVTerminalView::GetHistoryMenuItem(int nIndex)
{
	vector<CString>::iterator iter, endIter;
	int						  i = 0;

	iter    = m_historyMenuItems.begin();
	endIter = m_historyMenuItems.end();

	for(; iter != endIter; iter++)
	{
		if(i == nIndex)
			return (*iter);
		i++;
	}
	return NULL;
}

void CMVTerminalView::AddHistoryMenuItem(CString sItem)
{
	vector<CString>::iterator iter, endIter;

	iter    = m_historyMenuItems.begin();
	endIter = m_historyMenuItems.end();

	if(m_pViewPropertiesDlg->GetCmdHistorySettingsTab().GetRemoveDuplicatesOnMenu() == TRUE)
	{
		for(; iter != endIter; iter++)
		{
			if((*iter) == sItem)
				return;
		}
	}

	CString sTemp = sItem;
	sTemp.Remove(' ');
	sTemp.Remove('\t');
	if(sTemp.GetLength() != 0)
	{
		m_historyMenuItems.push_back(sItem);
	}
}

void CMVTerminalView::ClearHistoryMenuItems()
{
	m_historyMenuItems.clear();
}


void CMVTerminalView::ResetConnection()
{
	if(m_SPCommCtrl.get_IsOpened() == TRUE)
	{
		m_SPCommCtrl.Close();
		Sleep(50);
		m_SPCommCtrl.Open(GetConnectionParams().GetComPort());
	}
}

void CMVTerminalView::OnEditPaste()
{
	char * pBuffer = NULL;

	CString fromClipboard;
	if ( OpenClipboard() ) 
	{
		HANDLE hData = ::GetClipboardData( CF_TEXT );
		char * pBuffer = (char*)GlobalLock( hData );
		fromClipboard = pBuffer;
		GlobalUnlock( hData );
		CloseClipboard();
	}
	if(fromClipboard.IsEmpty() == FALSE)
	{
		PasteString(fromClipboard);
	}
}

void CMVTerminalView::OnLoadTftpFile()
{
	CTFTPServerPane  *pTftpPane;

	pTftpPane = m_pMainFrame->GetTFTPServerPane();
	if(pTftpPane == NULL)
		return;
	pTftpPane->OnBnClickedLoadTftpFile();
}



void CMVTerminalView::OnFileClose()
{
	GetDocument()->OnCloseDocument();
}

void	CMVTerminalView::SetWrapTextStatus(BOOL bStatus)
{
	m_bWrapText = bStatus;
}

BOOL	CMVTerminalView::GetWrapTextStatus()
{
	return m_bWrapText;
}

void CMVTerminalView::OnWrapTextUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bWrapText);
}

void CMVTerminalView::OnWrapText()
{
	if(GetWrapTextStatus() == FALSE)
	{
	    m_nWordWrap = WrapToWindow;
	    WrapChanged();
		SetWrapTextStatus(TRUE);
	}
	else
	{
	    m_nWordWrap = WrapNone;
	    WrapChanged();
		SetWrapTextStatus(FALSE);
	}
}

void CMVTerminalView::OnDropFiles(HDROP hDropInfo)
{
    CString		      sFilename;
	CTFTPServerPane  *pTFTPServerPane;

	pTFTPServerPane = m_pMainFrame->GetTFTPServerPane();
	if(pTFTPServerPane == NULL)
		return;

    m_hDropInfo = hDropInfo;
    if (m_hDropInfo) 
	{
        int nFiles = DragQueryFile(m_hDropInfo, (UINT)-1, NULL, 0);
		for(int i = 0; i < nFiles ; i++) 
		{
            char* pFilename = sFilename.GetBuffer(_MAX_PATH);
            DragQueryFile(m_hDropInfo, i, pFilename, _MAX_PATH);

			if((GetFileAttributes(pFilename) & FILE_ATTRIBUTE_DIRECTORY) != 
				FILE_ATTRIBUTE_DIRECTORY)
			{
				pTFTPServerPane->AddAndLoadFile(pFilename);
			}
		}
    }    
    DragFinish(m_hDropInfo);
    m_hDropInfo = 0;
}


BOOL CMVTerminalView::DeSerialize(CMarkup& xmlReader)
{
	if(xmlReader.FindElem("Session") == false)
		return FALSE;
	xmlReader.IntoElem();

	if( GetViewPropertiesDlg()->DeSerialize(xmlReader)  == FALSE)
		return FALSE;

	xmlReader.OutOfElem();
	return TRUE;
}


CString  CMVTerminalView::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("Session");
	xmlWriter.IntoElem();

	xmlWriter.AddSubDoc( GetViewPropertiesDlg()->Serialize() );

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}



void CMVTerminalView::OnClose()
{
	CRichEditView::OnClose();
}
