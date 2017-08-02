#include "StdAfx.h"
#include "CmdExeCmdItemCheckResult.h"
#include "MVTerminal.h"

CCmdExeCmdItemCheckResult::CCmdExeCmdItemCheckResult()
{
	CMVTerminalApp* pApp;

	pApp               = (CMVTerminalApp*)AfxGetApp();
	m_sAppDataPath     = pApp->GetAppDataPath();

	m_params.m_bFailIfFound = FALSE;
	m_params.m_sStr         = "";
	m_params.m_type         = CE_CHECK_RESULT_COMPARE_STRING_TYPE;
	m_params.m_sComPort     = "";

	m_sName        = "Check Result";
	m_sDisplayName = "Check Result";
}

CCmdExeCmdItemCheckResult::~CCmdExeCmdItemCheckResult(void)
{
}

CMVTerminalView*  CCmdExeCmdItemCheckResult::GetView()
{
	CMVTerminalApp		  *pApp;
	CMVTerminalView		  *pView;

	pApp  = (CMVTerminalApp*)AfxGetApp();
	pView = ((CMainFrame*)(pApp->GetMainFrame()))->GetViewByCOM(m_params.m_sComPort);
	if(pView == NULL)
	{
		AfxMessageBox("Can't find " + m_params.m_sComPort + " session window!",NULL,MB_ICONERROR);
		return NULL;
	}
	return pView;
}

BOOL    CCmdExeCmdItemCheckResult::Execute()
{
	CString			  str, sLine1, sLine2;
	CStdioFile		  compareFile, tempFile;
	CMVTerminalView	 *pView;

	if((pView = GetView()) == NULL)
		return FALSE;

	pView->GetWindowText(str);
	switch(m_params.m_type)
	{
	case CE_CHECK_RESULT_COMPARE_STRING_TYPE:
		/* Compare string */
		int		nRes;
		nRes = str.Find(m_params.m_sStr,0);
		if(nRes != -1) 
		{
			if(m_params.m_bFailIfFound == TRUE)
			{
				Log(m_sDisplayName + " - String found --> ERROR!!!");
				return FALSE;
			}
			else
			{
				Log(m_sDisplayName + " - String found --> OK!");
				return TRUE;
			}
		}
		if(nRes == -1) 
		{
			if(m_params.m_bFailIfFound == TRUE)
			{
				Log(m_sDisplayName + " - String not found --> OK!");
				return TRUE;
			}
			else
			{
				Log(m_sDisplayName + " - String not found!");
				return FALSE;
			}
		}
		break;
	case CE_CHECK_RESULT_COMPARE_FILE_TYPE:
		/* Compare text file */
		if(compareFile.Open(m_params.m_sStr,CFile::modeRead | CFile::typeText) == FALSE)
		{
			Log(m_sDisplayName + " - Error while opening file!");
			return FALSE;
		}
		if(tempFile.Open(m_sAppDataPath + "\\_CompareFile.txt",CFile::modeWrite |CFile::modeCreate | CFile::typeText) == FALSE)
		{
			Log(m_sDisplayName + " - Error while opening file!");
			return FALSE;
		}
		str.Remove(13);
		tempFile.WriteString(str);
		tempFile.Close();
		if(tempFile.Open(m_sAppDataPath + "\\_CompareFile.txt",CFile::modeRead | CFile::typeText) == FALSE)
		{
			Log(m_sDisplayName + " - Error while opening file!");
			return FALSE;
		}
		if(tempFile.GetLength() != compareFile.GetLength())
		{
			Log(m_sDisplayName + " - Not equal - ERROR!!!");
			return FALSE;
		}
		while((tempFile.ReadString(sLine1) != FALSE) &&
			  (compareFile.ReadString(sLine2) != FALSE))
		{
			if(sLine1 != sLine2)
			{
				Log(m_sDisplayName + " - Not equal --> ERROR!!!");
				return FALSE;
			}
		}
		Log(m_sDisplayName + " - Equal --> OK!");
		tempFile.Close();
		compareFile.Close();
		break;
	default:
		break;
	}
	return TRUE;
}


CCmdExeItem*   CCmdExeCmdItemCheckResult::Clone()
{
	CCmdExeCmdItemCheckResult *pClone;
	pClone = new CCmdExeCmdItemCheckResult();
	pClone->SetParams(GetParams());
	return pClone;
}
BOOL  CCmdExeCmdItemCheckResult::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemCheckResult");
	CCmdExeCmdItem::Serialize(xmlWriter);

	xmlWriter.AddAttrib( "FailIfFound",m_params.m_bFailIfFound);
	xmlWriter.AddAttrib( "ComPort",m_params.m_sComPort);
	xmlWriter.AddAttrib( "Str",m_params.m_sStr);
	xmlWriter.AddAttrib( "type",m_params.m_type);

	return TRUE;
}

BOOL  CCmdExeCmdItemCheckResult::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	CString sType;
	m_params.m_bFailIfFound = (xmlReader.GetAttrib("FailIfFound") == "0")? FALSE:TRUE;
	m_params.m_sComPort     = xmlReader.GetAttrib("ComPort");
	m_params.m_sStr         = xmlReader.GetAttrib("Str");
	sType = xmlReader.GetAttrib("type");
	sscanf(sType,"%d",&m_params.m_type);

	return TRUE;
}

