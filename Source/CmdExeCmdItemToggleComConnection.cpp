#include "StdAfx.h"
#include "CmdExeCmdItemToggleComConnection.h"
#include "MVTerminalView.h"
#include "MVTerminal.h"


CCmdExeCmdItemToggleComConnection::CCmdExeCmdItemToggleComConnection(void)
{
	m_sName        = "Toggle COM Connection";
	m_sDisplayName = "Toggle COM Connection";
}

CCmdExeCmdItemToggleComConnection::~CCmdExeCmdItemToggleComConnection(void)
{
}

CMVTerminalView*  CCmdExeCmdItemToggleComConnection::GetView()
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

BOOL    CCmdExeCmdItemToggleComConnection::Execute()
{
	CMVTerminalView	 *pView;

	if((pView = GetView()) == NULL)
		return FALSE;

	Log(m_sDisplayName);
	if(m_params.m_bDisconnect == TRUE)
		pView->OnPortDisconnect();
	
	int  nDelay;
	sscanf(m_params.m_sDelay,"%d",&nDelay);
	Sleep(nDelay);

	if(m_params.m_bConnect==TRUE)
		pView->OnPortConnect();

	return TRUE;
}

BOOL  CCmdExeCmdItemToggleComConnection::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemToggleComConnection");
	CCmdExeCmdItem::Serialize(xmlWriter);

	xmlWriter.AddAttrib( "Connect",m_params.m_bConnect);
	xmlWriter.AddAttrib( "Disconnect",m_params.m_bDisconnect);
	xmlWriter.AddAttrib( "ComPort",m_params.m_sComPort);
	xmlWriter.AddAttrib( "Delay",m_params.m_sDelay);

	return TRUE;
}

BOOL  CCmdExeCmdItemToggleComConnection::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	m_params.m_bConnect    = (xmlReader.GetAttrib("Connect") == "0")? FALSE:TRUE;
	m_params.m_bDisconnect = (xmlReader.GetAttrib("Disconnect") == "0")? FALSE:TRUE;
	m_params.m_sComPort = xmlReader.GetAttrib("ComPort");
	m_params.m_sDelay = xmlReader.GetAttrib("Delay");

	return TRUE;
}

CCmdExeItem*   CCmdExeCmdItemToggleComConnection::Clone()
{
	CCmdExeCmdItemToggleComConnection *pClone;
	pClone = new CCmdExeCmdItemToggleComConnection();
	pClone->SetParams(GetParams());
	return pClone;
}


