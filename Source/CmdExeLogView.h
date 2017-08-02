#pragma once

class CCmdExeLogView: public CRichEditCtrl
{
	BOOL           m_bCaptureToLogFile;
	CStdioFile	   m_logCaptureFile;

public:
	CCmdExeLogView(void);
	virtual ~CCmdExeLogView(void);

	virtual void SetCaptureToLogFile(BOOL bCapture);
	virtual BOOL GetCaptureToLogFile();
	virtual void OpenLogWinBrowseDlg();
	virtual void ClearLogWindow();
	virtual void OpenLogWinNotepad();


	virtual BOOL OnLogLine(CString sLine); 
};
