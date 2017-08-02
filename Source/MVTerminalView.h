// MVTerminalView.h : interface of the CMVTerminalView class
//
#pragma once

#include ".\textline.h"
#include "MVTerminalDoc.h"
#include "sportax1.h"
#include ".\BootLoaderParams.h"
#include ".\viewpropertiesdlg.h"
#include "CmdHistoryLogger.h"
#include ".\connectionparams.h"
#include "NewMenu.h"
#include "ViewPropertiesParams.h"
#include <vector>
using namespace std;


#define  WM_THREAD_DONE					(WM_USER + 1)
#define  WM_LOAD_SCRIPT_THREAD_ENDED	(WM_USER + 2)
#define  IDC_SPORTAX                    (WM_USER + 3)
#define  WM_CLEAR_HISTORY               (WM_USER + 4)

// Thread status enumaration
typedef enum  _mvtThreadStatus { MVT_THREAD_IS_NOT_ACTIVE,
			  	                 MVT_THREAD_IS_ACTIVE,
						         MVT_END_THREAD
					           } MVT_THREAD_STATUS;

class CMVTerminalCntrItem;

class CMVTerminalView : public CRichEditView
{
public:


	// Load script class params
	class CLoadScriptParams
	{		
		CMVTerminalView  *m_pView;
		CTextFile   	 *m_pScript;
	public:
		CLoadScriptParams() {}
		void SetParams(CMVTerminalView *pView, CTextFile *pScript)
		{
			m_pView   = pView;
			m_pScript = pScript;
		}
		CTextFile*        GetScript() {return m_pScript;}
		CMVTerminalView*  GetView() {return m_pView;}
	};
		
	// Load text file class params
	class CLoadTextFileParams
	{		
		CMVTerminalView  *m_pView;
	public:
		CLoadTextFileParams() {}
		void SetView(CMVTerminalView *pView)
		{
			m_pView     = pView;
		}
		CTextFile&          GetFile() {return m_textFile;}
		CMVTerminalView*    GetView() {return m_pView;}
		CTextFile		    m_textFile;

	};

	CLoadScriptParams	    m_loadScriptParams;
	CLoadTextFileParams 	m_loadTextFileParams;
	HDROP					m_hDropInfo;

	bool				    m_bDisableViewUpdate;
	CString				    m_sEditModeData;
	int					    m_nCursorIndex;
	int					    m_nStringLen;
	bool					m_bInEditMode;

private:

	afx_msg void OnDropFiles(HDROP hDropInfo);

	CViewPropertiesParams   m_viewPropertiesParams;

	/* Messaging */
	CString				    m_sIPMMsg;
	BOOL					m_bEnableViewMsg;
	BOOL	                HandleIPMMessage(CString& sStr);
	BOOL					CreateIPMMessage(CString sMsg, CIPMMessage* pMsg);

	CMVTerminalView*        GetViewByWinName(CString sWinName);
	void					StripMessage(CString & sStr);

	CViewPropertiesDlg*	    m_pViewPropertiesDlg;


	CCmdHistoryLogger       m_cmdHistoryLogger;
	CNewMenu			    m_cmdHistoryMenu;
	BOOL	                m_bUseTerminalHistory;
	vector<CString>	    	m_historyMenuItems;

	/* Mail */
	unsigned int			m_nSecondCount;
	BOOL                    GetSendMailEn();
	BOOL                    SendMail();
	BOOL					CheckSendDuration();
	void					ShowSendMailNotification();


	/* BootLoader parameters */
	CBootLoaderParams    m_bootLoaderParams;

	/* Font */
	CFont		      m_viewFont;
    LOGFONT			  m_lf; 
	CMainFrame       *m_pMainFrame;

	/* Text wrap */
	BOOL	          m_bWrapText;
	void			  SetWrapTextStatus(BOOL bStatus);
	BOOL			  GetWrapTextStatus();
	void			  OnWrapTextUpdate(CCmdUI* pCmdUI);
	void			  OnWrapText();

	/* File capture */
	CStdioFile        m_captureFile;
	BOOL	          m_bCaptureToFile;
	void			  OnCaptureToFile();
	void			  OnCaptureToFileUpdate(CCmdUI* pCmdUI);
	void			  CaptureToFile(CString str);
	void			  DumpScreenBufferToFile(CString& sFilePath);

	/* Timer */
	UINT_PTR		  m_pIDTimer;
	void			  OnTimeChange(CCmdUI *pCmdUI);
	int				  m_nHour;	
    int				  m_nMinute;
    int				  m_nSecond;
	CString           m_sConnectionStatus;
	bool			  m_showTimeCount;
	void			  EnableConnectionTimer(BOOL bEnable);

	/* COM comunication */
	bool              m_bPortConnected;
	CSportax1         m_SPCommCtrl;

	CConnectionParams m_connectionParams;

	void	          OnPortDisconnectUpdate(CCmdUI* pCmdUI);
	void	          OnPortConnectUpdate(CCmdUI* pCmdUI);
	void			  OnViewProperties();
	volatile		  MVT_THREAD_STATUS  m_threadStatus;
	void			  EraseTextRegion(int len,int nStart,int nEnd);

	DECLARE_EVENTSINK_MAP()
	void OnRxCharSportctrl1(long Count);
	void OnCTSSportctrl1(BOOL On);
	void OnDSRSportctrl1(BOOL On);
	void OnRingSportctrl1(BOOL On);
	void OnBreakSportctrl1();
	void OnDCDSportctrl1(BOOL On);
	void OnTxEmptySportctrl1();
	void OnRxFlagSportctrl1();
	void OnCommErrorSportctrl1(long err);


	LRESULT			  OnThreadDone(WPARAM a, LPARAM b);
	void              SetThreadStatus(MVT_THREAD_STATUS  status);


	/* View update/edit */
	void		      OnViewWindowContent();
	void			  ShowScrollBars();
	void			  OnNewFont();
	void			  SearchText();
	void			  OnClearBuffer();


	void 			  AppendEditModeData(CString sCh);
	bool 			  IsEditModeEnabled() ;
	void 			  ResetEditModeData();
	void 			  ExitEditMode();
	void 			  IncEditModeDataCursorIndex();
	void 			  DecEditModeDataCursorIndex(); 
	void 			  EnterEditMode(); 
	void 			  DeleteEditModeData();
	void 			  BackspaceEditModeData();	
	void 			  SetEditModeData(TCHAR ch);
	void 			  SetEditModeData(CString sStr);
	void 			  SendEditModeData();
	BOOL			  IsCursorInEditMode();
	int				  GetSelTextStartIndex();
	int				  GetSelLineIndex();
	int				  GetSelTextAbsoluteIndex();
	void			  UpdateEditModeDataHistory();


protected: // create from serialization only
	CMVTerminalView();
	DECLARE_DYNCREATE(CMVTerminalView)

public:
	void              SetPropertiesDlg(CViewPropertiesDlg *pDlg);

	CMVTerminalDoc*   GetDocument() const;
	CSportax1&		  GetSPCommCtrl() {return m_SPCommCtrl;}

	MVT_THREAD_STATUS GetThreadStatus();
	void			  UpdateView(CString pchData);
	CString			  RemoveVT100ControlSeq(CString sStr);
	void			  PasteString(CString txt);
	void			  LoadTextFile(CString sFile);
	long			  ReadScreenBuffer(char *pBuf, int nLen);


// Operations
public:
	void			   ResetConnection();
	void			   OnPortConnect();
	void			   OnPortDisconnect();

	CConnectionParams& GetConnectionParams() {return m_connectionParams;}
	void               SetConnectionParams(CConnectionParams connectionParams) {m_connectionParams = connectionParams;}

	void		       SetWindowTitle(CString  wndTitleStr);
	CString			   GetWindowTitle();
	CMainFrame      *  GetMainFrame() {	return	m_pMainFrame;}
	BOOL			   CreateCommCtrl();
	BOOL	           ActivateComPort(CConnectionParams connectionParams);
	CString			   Serialize();
	BOOL			   DeSerialize(CMarkup& xmlReader);
	CViewPropertiesDlg* GetViewPropertiesDlg() {return m_pViewPropertiesDlg;}

	/* View edit/update */
	void			   DeleteText(int nStartIndex,int nCount);
	void			   AppendText(int lastCharIndex, CString txt);
	void			   RemoveBufferTail(int nReduceCount);
	void			   SetCaptureToFileStatus(BOOL bStatus);
	BOOL			   GetCaptureToFileStatus();

	void			   SendTextLine(CTextLine  txtLine);
	void		       LoadScript(CTextFile  *pScript);
	CBootLoaderParams& GetBootLoaderParams() {return m_bootLoaderParams;}
	CBootLoaderParams  GetBootLoaderParamsContent() {return m_bootLoaderParams;}
	
	void               SetBootLoaderParams(CBootLoaderParams bootLoaderParams);

	// History
	CMenu&		       GetCmdHistoryMenu() {return m_cmdHistoryMenu;}
	CCmdHistoryLogger& GetCmdHistoryLogger() {return m_cmdHistoryLogger;}

	void               AddHistoryMenuItem(CString sItem);
	CString			   GetHistoryMenuItem(int nIndex);
	void               ClearHistoryMenuItems();


	
	void		       SetCmdHistoryEnable(BOOL bEnable) {m_cmdHistoryLogger.SetEnable(bEnable);}
	void		       SetMaxHistoryCmdCount(int nMaxCmdCount) {m_cmdHistoryLogger.SetMaxCmdCount(nMaxCmdCount);}
	int			       GetMaxHistoryCmdCount()				   {return m_cmdHistoryLogger.GetMaxCmdCount();}

	void		       SetUseTerminalHistory(BOOL bUse) {m_bUseTerminalHistory = bUse;}
	BOOL		       GetUseTerminalHistory()			{return m_bUseTerminalHistory;}

	void		       SetCmdHistoryFilter(CString sCmdHistoryFilter) {m_cmdHistoryLogger.SetFilter(sCmdHistoryFilter);}
	CString		       GetCmdHistoryFilter()  {return m_cmdHistoryLogger.GetFilter();}
	int		           GetCmdMinLenThreshold()  {return m_cmdHistoryLogger.GetCommandMinLenThreshold();}
	void		       SetCmdMinLenThreshold(int nCmdLen)  {m_cmdHistoryLogger.SetCommandMinLenThreshold(nCmdLen);}
	int		           GetCmdMaxLenThreshold()  {return m_cmdHistoryLogger.GetCommandMaxLenThreshold();}
	void		       SetCmdMaxLenThreshold(int nCmdLen)  {m_cmdHistoryLogger.SetCommandMaxLenThreshold(nCmdLen);}
	void		       SendHistoryMenuString(UINT nMenuIndex);

	void               OnCharHandleVKDOWNChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void			   OnCharHandleVKUPChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL			   OnCharHandleSelectedText(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL			   OnCharHandleEditMode(UINT nChar, UINT nRepCnt, UINT nFlags);
	void               OnClearHistory();
	

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMVTerminalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void    OnTimer(UINT nIDEvent);
	virtual BOOL    PreTranslateMessage(MSG* pMsg);
	afx_msg void    OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void    OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL    OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void    OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);

protected:
	virtual HMENU GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg);

public:
	afx_msg void OnEditPaste();
	afx_msg void OnLoadTftpFile();
	//afx_msg void OnCEExecuteCommands();
	afx_msg void OnFileClose();
	afx_msg void OnClose();
        afx_msg void OnKillFocus(CWnd* pNewWnd);
};

// Threads declerations
UINT ReadPortDataThread(LPVOID pParam);
UINT LoadScriptThread(LPVOID pParam);
UINT LoadTextFileThread(LPVOID pParam);

#ifndef _DEBUG  // debug version in MVTerminalView.cpp
inline CMVTerminalDoc* CMVTerminalView::GetDocument() const
   { return reinterpret_cast<CMVTerminalDoc*>(m_pDocument); }
#endif

