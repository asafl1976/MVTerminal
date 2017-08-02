#include "StdAfx.h"
#include "CmdExeCmdItemWaitForString.h"
#include "MVTerminal.h"

CCmdExeCmdItemWaitForString::CCmdExeCmdItemWaitForString()
{
	m_params.m_nTimeout = 10000;
	m_sName        = "Wait For String";
	m_sDisplayName = "Wait For String";
}

CCmdExeCmdItemWaitForString::~CCmdExeCmdItemWaitForString(void)
{

}
CMVTerminalView*  CCmdExeCmdItemWaitForString::GetView()
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

BOOL   CCmdExeCmdItemWaitForString::Execute()
{
	CString    sStr,sMsg;
	DWORD      nStart,nEnd;
	CMVTerminalView	 *pView;

	if((pView = GetView()) == NULL)
	{
		sMsg = "ERROR - ";
		sMsg += m_params.m_sComPort;
		sMsg += " Window can't be found!";
		Log( sMsg);
		return FALSE;
	}

	Log( m_sDisplayName);
	nStart = GetTickCount();
	while(sStr.Find(m_params.m_sStr) == -1)
	{
		nEnd = GetTickCount();
		if((nEnd - nStart) >= (DWORD)m_params.m_nTimeout)
		{
			Log( "Timout ERROR!!!" );
			return FALSE;
		}
		pView->GetWindowText(sStr);
	}
	//Log( "OK!" );
	return TRUE;
}

BOOL  CCmdExeCmdItemWaitForString::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemWaitForString");
	CCmdExeCmdItem::Serialize(xmlWriter);

	xmlWriter.AddAttrib( "Timeout",m_params.m_nTimeout);
	xmlWriter.AddAttrib( "ComPort",m_params.m_sComPort);
	xmlWriter.AddAttrib( "Str",m_params.m_sStr);

	return TRUE;
}

BOOL  CCmdExeCmdItemWaitForString::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	CString sTimeout;

	sTimeout   = xmlReader.GetAttrib("Timeout");
	sscanf(sTimeout,"%d",&m_params.m_nTimeout);
	m_params.m_sComPort = xmlReader.GetAttrib("ComPort");
	m_params.m_sStr     = xmlReader.GetAttrib("Str");

	return TRUE;
}

CCmdExeItem*   CCmdExeCmdItemWaitForString::Clone()
{
	CCmdExeCmdItemWaitForString *pClone = new CCmdExeCmdItemWaitForString();
	pClone->SetParams(GetParams());
	return pClone;
}






