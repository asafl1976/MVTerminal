#include "StdAfx.h"
#include "CmdExeCmdItemExecuteCommand.h"
#include "MVTerminalView.h"
#include "TextLine.h"
#include "MVTerminal.h"


CCmdExeCmdItemExecuteCommand::CCmdExeCmdItemExecuteCommand()
{
	m_params.m_sCmd = "";
	m_sName        = "Execute Command";
	m_sDisplayName = "Execute Command";
}

CCmdExeCmdItemExecuteCommand::~CCmdExeCmdItemExecuteCommand(void)
{

}

CMVTerminalView*  CCmdExeCmdItemExecuteCommand::GetView()
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

BOOL   CCmdExeCmdItemExecuteCommand::Execute()
{
	CTextLine		  cmd(m_params.m_sCmd,0,0,TRUE);
	CMVTerminalView	 *pView;

	Log(m_sDisplayName);

	if((pView = GetView()) == NULL)
		return FALSE;
	pView->SendTextLine(cmd);

	return TRUE;
}
BOOL  CCmdExeCmdItemExecuteCommand::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemExecuteCommand");
	CCmdExeCmdItem::Serialize(xmlWriter);

	xmlWriter.AddAttrib( "Cmd",m_params.m_sCmd);
	xmlWriter.AddAttrib( "ComPort",m_params.m_sComPort);

	return TRUE;
}

BOOL  CCmdExeCmdItemExecuteCommand::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	m_params.m_sCmd     = xmlReader.GetAttrib("Cmd");
	m_params.m_sComPort = xmlReader.GetAttrib("ComPort");

	return TRUE;
}

CCmdExeItem*   CCmdExeCmdItemExecuteCommand::Clone()
{
	CCmdExeCmdItemExecuteCommand *pClone;
	pClone = new CCmdExeCmdItemExecuteCommand();
	pClone->SetParams(GetParams());
	return pClone;
}



