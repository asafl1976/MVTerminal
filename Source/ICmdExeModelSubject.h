#pragma once
#include <vector>
using namespace std;
#include "ICmdExeModelObserver.h"
#include "CmdExeCmd.h"


class ICmdExeModelSubject
{
	vector<ICmdExeModelObserver*> 	m_Observers;

public:
	ICmdExeModelSubject(void);
	virtual ~ICmdExeModelSubject(void);

	virtual void RegisterObserver(ICmdExeModelObserver *pObserver);
	virtual void UnregisterObserver(ICmdExeModelObserver *pObserver);	

	virtual BOOL NotifyLoopCountChange(int nLoopCount);
	virtual BOOL NotifyLogLine(CString sLogLine);
	virtual BOOL NotifyAddCommand(CCmdExeCmd *pCommand);
	virtual BOOL NotifyRemoveCommand(CCmdExeCmd *pCommand);
	virtual BOOL NotifyUpdateCommand(CCmdExeCmd *pCommand);
	

};
