#include "StdAfx.h"
#include ".\cmdexepresenter.h"
#include "CmdExePane.h"

CCmdExePresenter::CCmdExePresenter(CCmdExeModel *pModel, CCmdExeTreeView* pCmdExeView, CCmdExeLogView* pLogView, CCmdExePane* pCmdExePaneView)
{
	m_pCmdExeLogView   = pLogView;
	m_pCmdExeView      = pCmdExeView;
	m_pCmdExeModel     = pModel;
	m_pCmdExePaneView  = pCmdExePaneView;

	m_pCmdExeModel->RegisterObserver(this);
	m_pCmdExeView->RegisterObserver(this);
}

CCmdExePresenter::~CCmdExePresenter(void)
{
}

//View observer
BOOL CCmdExePresenter::OnViewLoadCommands(CMarkup& xmlReader)
{
	return m_pCmdExeModel->LoadCommands(xmlReader);
}

BOOL CCmdExePresenter::OnViewSaveCommands(CMarkup& xmlWriter)
{
	return m_pCmdExeModel->SaveCommands(xmlWriter);
}

BOOL CCmdExePresenter::OnViewRemoveAllCommands()
{
	return m_pCmdExeModel->RemoveAllCommands();
}

BOOL CCmdExePresenter::OnViewRemoveCommand(CCmdExeCmd *pCommand)
{
	return m_pCmdExeModel->RemoveCommand(pCommand);
}

BOOL CCmdExePresenter::OnViewUpdateCommand(CCmdExeCmd *pCommand)
{
	return m_pCmdExeModel->UpdateCommand(pCommand);
}
BOOL CCmdExePresenter::OnViewAddCommand(CCmdExeCmd *pCommand)
{
	return m_pCmdExeModel->AddCommand(pCommand);
}

//Model observer
BOOL CCmdExePresenter::OnModelLoopCountChange(int nloopCount)
{
	m_pCmdExePaneView->SetLoopIndex(nloopCount);
	return TRUE;
} 

BOOL CCmdExePresenter::OnModelRemoveCommad(CCmdExeCmd *pCommand)
{
	return m_pCmdExeView->OnRemoveCommand(pCommand);
}

BOOL CCmdExePresenter::OnModelAddCommand(CCmdExeCmd *pCommand)
{
	return m_pCmdExeView->OnAddCommand(pCommand);
}

BOOL CCmdExePresenter::OnModelLogLine(CString sLogLine)
{
	return m_pCmdExeLogView->OnLogLine(sLogLine);
}

BOOL CCmdExePresenter::OnModelUpdateCommand(CCmdExeCmd *pCommand)
{
	return m_pCmdExeView->OnUpdateCommand(pCommand);
}
