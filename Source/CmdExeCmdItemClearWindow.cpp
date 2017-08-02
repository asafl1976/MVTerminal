#include "StdAfx.h"
#include "CmdExeCmdItemClearWindow.h"
#include "MVTerminalView.h"
#include "MVTerminal.h"

CCmdExeCmdItemClearWindow::CCmdExeCmdItemClearWindow()
{
	m_sName        = "Clear Window";
	m_sDisplayName = "Clear Window";
}

CCmdExeCmdItemClearWindow::~CCmdExeCmdItemClearWindow(void)
{
}

CMVTerminalView*  CCmdExeCmdItemClearWindow::GetView()
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

BOOL CCmdExeCmdItemClearWindow::Execute()
{	
	CMVTerminalView		  *pView;

	if((pView = GetView()) == NULL)
	{
		Log("COM Port window could not be found!");
		return FALSE;
	}
	Log(m_sDisplayName);
	pView->SetWindowText("");
	return TRUE;
}

BOOL  CCmdExeCmdItemClearWindow::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemClearWindow");
	CCmdExeCmdItem::Serialize(xmlWriter);
	xmlWriter.AddAttrib( "ComPort",m_params.m_sComPort);
	return TRUE;
}

BOOL  CCmdExeCmdItemClearWindow::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;
	m_params.m_sComPort = xmlReader.GetAttrib("ComPort");
	return TRUE;
}

CCmdExeItem*   CCmdExeCmdItemClearWindow::Clone()
{
	CCmdExeCmdItemClearWindow *pClone;
	pClone = new CCmdExeCmdItemClearWindow();
	pClone->SetParams(GetParams());
	return pClone;
}






