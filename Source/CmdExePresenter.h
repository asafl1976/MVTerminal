#pragma once
#include "icmdexepresenter.h"
#include "ICmdExeModelObserver.h"
#include "ICmdExeViewObserver.h"
#include "CmdExeTreeView.h"
#include "CmdExeLogView.h"
#include "CmdExeModel.h"

class CCmdExePresenter : 	public ICmdExePresenter , public ICmdExeModelObserver, public ICmdExeViewObserver
{
	CCmdExePane       *m_pCmdExePaneView;
	CCmdExeLogView    *m_pCmdExeLogView;
	CCmdExeTreeView   *m_pCmdExeView;
	CCmdExeModel      *m_pCmdExeModel;

public:
	CCmdExePresenter(CCmdExeModel *pModel, CCmdExeTreeView* pCmdExeView, CCmdExeLogView* pLogView, CCmdExePane* pCmdExePaneView);
	virtual ~CCmdExePresenter(void);

	//View observer
	virtual BOOL     OnViewUpdateCommand(CCmdExeCmd *pCommand); 
	virtual BOOL     OnViewRemoveAllCommands(); 
	virtual BOOL     OnViewRemoveCommand(CCmdExeCmd *pCommand); 
	virtual BOOL     OnViewAddCommand(CCmdExeCmd *pCommand);
	virtual BOOL     OnViewLoadCommands(CMarkup& xmlReader);
	virtual BOOL	 OnViewSaveCommands(CMarkup& xmlWriter);

	//Model observer
	virtual BOOL	 OnModelLoopCountChange(int nloopCount);
	virtual BOOL	 OnModelLogLine(CString sLogLine);
	virtual BOOL     OnModelRemoveCommad(CCmdExeCmd *pCommand);
	virtual BOOL     OnModelUpdateCommand(CCmdExeCmd *pCommand);
	virtual BOOL     OnModelAddCommand(CCmdExeCmd *pCommand);

};
