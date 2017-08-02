// MainFrm.h : interface of the CMainFrame class
//
#pragma once

#include "PathDialog.h"
#include "TextFile.h"
#include "IPMMessageManager.h"
#include <map>
using namespace std;

#define  MVT_ACTIVE_TAB_COLOR    xtpTabColorYellow
#define  MVT_INACTIVE_TAB_COLOR  RGB(200,200,200)
#define  WM_UPDATE_TAB_COLOR     (WM_USER + 5)
#define  WM_UPDATE_TFTP_FILE_LIST (WM_USER + 6)

class CPlatformInfoPane;
class CHwModulesPane;
class CDataAnalyzerPane;
class CSendScriptPane;
class CTFTPServerPane;
class CCmdExePane;
class CMVTerminalView;
class CIPMMessage;

class CXTPPopupControlEx: public CXTPPopupControl
{
	CString  m_sMsg;

public:
	CXTPPopupControlEx(CString sMsg)
	{
		m_sMsg = sMsg;
	}
	CString GetMessage() { return m_sMsg;}
};


class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

	map<int,CXTPDockingPane*> m_paneMap;
	CFrameWnd	             *m_pTFTPFormFrame;
	CFrameWnd	             *m_pSendScriptFormFrame;
	CFrameWnd	             *m_pDataAnalyzerFrame;
	CFrameWnd	             *m_pCommandExecuterFrame;
	CFrameWnd	             *m_pHwModulesFrame;
	CFrameWnd	             *m_pPlatformInfoFrame;

	CXTPReBar                m_wndReBar;
	BOOL			         m_bHideHwModulesPane;

	CXTPDockingPaneManager   m_paneManager;
	CXTPTabClientWnd         m_MTIClientWnd;

	CString					 m_sAppDataPath;
	int					     m_nColorIndex;

	XTPPaintTheme			 m_nAppTheme;
	XTPDockingPanePaintTheme m_nPanesTheme;

	CXTCaption				 m_wndCaption;
	list<CMVTerminalView*>   m_viewList;
	void                     AddView(CMVTerminalView* pView);
	void                     RemoveView(CMVTerminalView* pView);
	CMVTerminalView*    	 GetViewByName(CString sName);
	CMVTerminalView*    	 GetViewByCOM(CString sCom);
	void				     CreatePanes();

	CIPMMessageManager       m_IPMMessageManager;

public:
	CMainFrame();

	void				     OnIPMMessage(CIPMMessage  *pMsg);
	void				     SendIPMMessage(CIPMMessage  *pMsg);
	void					 SetAppTitle(CString sTitle);
	void					 ShowPane(int nID,BOOL bShow);
	CXTPDockingPane*         GetPane(int nID);
	void					 AddLogo();
	CDataAnalyzerPane*       GetDataAnalyzerPane();
	CSendScriptPane*         GetSendScriptPane();   
	CTFTPServerPane*         GetTFTPServerPane();
	CCmdExePane*             GetCommandExecuterPane();
	CHwModulesPane*          GetHardwareModulesPane();
	CPlatformInfoPane*   	 GetPlatformInfoPane();
	BOOL					 SaveConfiguration(CString sPath);
	void					 OpenSaveConfigDlg(BOOL bSkipCheck = FALSE);

	virtual BOOL		     SaveSettings(CString sFile);
	virtual BOOL		     LoadSettings(CString sFile);

	virtual	CString			 Serialize();
	virtual	BOOL			 DeSerialize(CMarkup & xmlReader);
	///////////////// Notification //////////////////////////
	int     m_nTheme;
	int     m_nAnimation;
	UINT    m_uAnimDelay;
	UINT    m_uShowDelay;
	BOOL    m_bAllowMove;
	CPoint  m_ptPopup;

	void    SetBlackTheme();
	void	SetNotificationTheme();
	void    SetDefaults();
	void	Notify(CString sMsg);
	//popup window object
	CList<CXTPPopupControlEx*, CXTPPopupControlEx*> m_lstPopupControl;
	CXTPPopupControlEx*        m_pActivePopup;
	void    FindBestPosition(CSize szPopup);
	/////////////////////////////////////////////////////////

public:

// Operations
public:
	LRESULT          OnTabbarMouseMsg(WPARAM wParam,LPARAM lParam);
	LRESULT          OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	CXTPStatusBar&   GetStatusBar() { return m_wndStatusBar;}
	int			     GetViewCount() {return (int)m_viewList.size();}
	list<CMVTerminalView*>&    GetViewList() {return m_viewList;}
	//void		     SetViewCount(int nViewCount) {m_nViewCount = nViewCount;}
	void		     SetAppTheme(XTPPaintTheme nAppTheme)  { m_nAppTheme   = nAppTheme;}
	void		     SetPaneTheme(XTPDockingPanePaintTheme nPaneTheme) { m_nPanesTheme = nPaneTheme;}

	void			 UpdateTabColor();
	void			 SetTabColor(int nTabIndex, XTPTabOneNoteColor xtpTabColor);
	int				 GetActiveTabIndex();
	void			 SetActiveTabColor(XTPTabOneNoteColor xtpTabColor);
	void			 SetInactiveTabColor(COLORREF color);
	LRESULT			 OnUpdateTabColor(WPARAM a, LPARAM b);
	LRESULT			 OnUpdateTFTPFileList(WPARAM a, LPARAM b);
	void			 CloseAllViews();



// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar   m_wndStatusBar;
	CToolBar        m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnCustomize();

	afx_msg LRESULT OnPopUpNotify(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnClose();

	afx_msg void OnChildActivate();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

	DECLARE_MESSAGE_MAP()

};


