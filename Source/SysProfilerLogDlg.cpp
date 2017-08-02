// SysProfilerLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "SysProfilerLogDlg.h"
#include "ProfilerEventManager.h"


// CSysProfilerLogDlg dialog

CSysProfilerLogDlg::CSysProfilerLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysProfilerLogDlg::IDD, pParent)
{
	m_bCaptureToLogFile = FALSE;
}

CSysProfilerLogDlg::~CSysProfilerLogDlg()
{
}

void CSysProfilerLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_richEdit);
}


BEGIN_MESSAGE_MAP(CSysProfilerLogDlg, CDialog)
		ON_WM_DESTROY()	
		ON_BN_CLICKED(IDCANCEL, &CSysProfilerLogDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void CSysProfilerLogDlg::PostNcDestroy()
{
	delete this;
	::SendMessage(GetOwner()->m_hWnd,WM_SYS_PROFILER_MODELESS_DLG_DESTROYED,NULL,NULL);
}

void CSysProfilerLogDlg::OnDestroy()
{
	//Notify owner that dlg is destroyed
	CDialog::OnDestroy();
}

void CSysProfilerLogDlg::OnCancel()
{
	DestroyWindow();
}

BOOL  CSysProfilerLogDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(CSysProfilerLogDlg::IDD,pParentWnd);
}

void CSysProfilerLogDlg::AddLogLine(CString sLogLine)
{
	CString sCurLog;

	if(m_bCaptureToLogFile == TRUE)
		m_logCaptureFile.WriteString(sLogLine + "\n");

	m_richEdit.GetWindowText(sCurLog);
	if(sCurLog.GetLength() >= (10*4096))
	{	
		// Cut the tail..
		sCurLog = sCurLog.Right(sCurLog.GetLength() - 2048);		
	}
	sCurLog += sLogLine;
	sCurLog += "\n";
	m_richEdit.SetWindowText(sCurLog);
	for(int i = 0; i < m_richEdit.GetLineCount(); i++)
	{
		m_richEdit.SendMessage(EM_SCROLL,SB_LINEDOWN);
	}
}


void CSysProfilerLogDlg::OnBnClickedCancel()
{
	CProfilerEventManager::GetInstance().SetEnableLogging(FALSE);
	OnCancel();
}
