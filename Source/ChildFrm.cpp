// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "MVTerminal.h"

#include "ChildFrm.h"
#include ".\childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers



void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	//CMainFrame  *pMainFrame;	
    //pMainFrame = (CMainFrame*)GetParentOwner();
	//::PostMessage(pMainFrame->m_hWnd,WM_UPDATE_TAB_COLOR,0,0); 

}

BOOL CChildFrame::ValidateSessionFileName(CString sName)
{
	vector<CString>  comPorts;
	vector<CString>::iterator  iter,endIter;
	
	((CMVTerminalApp*)AfxGetApp())->EnumerateSerialPorts(comPorts);
	iter = comPorts.begin();
	endIter = comPorts.end();
	for(;iter != endIter; iter++)
	{
		if((*iter) == sName)
			return FALSE;
	}
	return TRUE;
}

void CChildFrame::CloseWindow()
{
	CMDIChildWnd::OnClose();
}

void CChildFrame::OnClose()
{
	CMDIChildWnd::OnClose();
}

void CChildFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
    CMDIChildWnd::OnUpdateFrameMenu (bActive, pActiveWnd, hMenuAlt);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}
