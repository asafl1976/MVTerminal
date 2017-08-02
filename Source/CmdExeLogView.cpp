#include "StdAfx.h"
#include ".\cmdexelogview.h"
#include "MVTerminal.h"

CCmdExeLogView::CCmdExeLogView(void)
{
}

CCmdExeLogView::~CCmdExeLogView(void)
{
}

void CCmdExeLogView::OpenLogWinBrowseDlg()
{
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog		dlg(TRUE,"","CmdExeLog",NULL,szFilter);

	if(dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		if(!m_logCaptureFile.Open(path, CFile::modeCreate
			   | CFile::modeWrite | CFile::typeText ) ) 
		{
			MessageBox("Can't open " + path + " file for writing...",NULL,MB_ICONEXCLAMATION);
			return;
		}		
		SetCaptureToLogFile(TRUE);
	}
}

void  CCmdExeLogView::SetCaptureToLogFile(BOOL bCapture) 
{
	if(bCapture == FALSE)
		m_logCaptureFile.Close();

	m_bCaptureToLogFile = bCapture;
}

BOOL  CCmdExeLogView::GetCaptureToLogFile()             
{
	return m_bCaptureToLogFile;
}

BOOL CCmdExeLogView::OnLogLine(CString sLine)
{
	CString sCurLog;

	if(GetCaptureToLogFile() == TRUE)
		m_logCaptureFile.WriteString(sLine + "\n");

	GetWindowText(sCurLog);
	if(sCurLog.GetLength() >= (10*1024))
	{	
		// Cut the tail..
		sCurLog = sCurLog.Right(sCurLog.GetLength() - 2048);		
	}
	sCurLog += sLine;
	sCurLog += "\n";
	SetWindowText(sCurLog);
	for(int i = 0; i < GetLineCount(); i++)
	{
		SendMessage(EM_SCROLL,SB_LINEDOWN);
	}
	return TRUE;
}

void CCmdExeLogView::ClearLogWindow()
{
	SetWindowText("");
}

void CCmdExeLogView::OpenLogWinNotepad()
{
	CString  sScreenBufferContent,sPath,sFilePath;

	sPath = ((CMVTerminalApp*)AfxGetApp())->GetAppDataPath();
	GetWindowText(sScreenBufferContent);
	CStdioFile file;
	if( !file.Open(sPath +  "\\CELogWinContent.txt", CFile::modeCreate
		   | CFile::modeWrite | CFile::typeText ) ) 
	{
		MessageBox("Error in opening file " + sPath +  "\\CELogWinContent.txt",NULL,MB_ICONERROR);
		return ;
	}
	sFilePath = sPath +  "\\CELogWinContent.txt";
	file.WriteString(sScreenBufferContent);
	file.Close();

	ShellExecute(NULL, "open", "notepad.exe",sFilePath ,"", SW_SHOWDEFAULT);

}
