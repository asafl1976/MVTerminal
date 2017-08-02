#pragma once
#include "afxcmn.h"

#define WM_SYS_PROFILER_MODELESS_DLG_DESTROYED (WM_USER + 300)
// CSysProfilerLogDlg dialog

class CSysProfilerLogDlg : public CDialog
{
	BOOL                 m_bCaptureToLogFile;
	CStdioFile	         m_logCaptureFile;
	CRichEditCtrl		 m_richEdit;

public:
	CSysProfilerLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSysProfilerLogDlg();
	virtual BOOL			Create(CWnd* pParentWnd);
// Dialog Data
	enum { IDD = IDD_SYS_PROFILER_LOG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//Modeless 
	virtual void			PostNcDestroy();
	afx_msg void			OnDestroy();
	virtual void			OnCancel();

	DECLARE_MESSAGE_MAP()
public:

	virtual void AddLogLine(CString sLogLine);
	afx_msg void OnBnClickedCancel();
};
