#pragma once
#include "ICmdExeViewObserver.h"
#include <vector>
using namespace std;

class ICmdExeViewObserver;

class ICmdExeViewSubject
{
	vector<ICmdExeViewObserver*> 	m_Observers;

public:
	ICmdExeViewSubject(void);
	virtual ~ICmdExeViewSubject(void);

	virtual BOOL NotifyLoadCommands(CMarkup& xmlReader);
	virtual BOOL NotifySaveCommands(CMarkup& xmlWriter);
	virtual void RegisterObserver(ICmdExeViewObserver *pObserver);
	virtual void UnregisterObserver(ICmdExeViewObserver *pObserver);
	virtual BOOL NotifyAddCommand(CCmdExeCmd* pCommand);
    virtual BOOL NotifyRemoveCommand(CCmdExeCmd* pCommand);
    virtual BOOL NotifyRemoveAllCommands();
    virtual BOOL NotifyUpdateCommand(CCmdExeCmd* pCommand);
	

};
