#pragma once
#include <vector>
using namespace std;

#include "IHwModuleViewObserver.h"



class IHwModuleViewSubject
{
	vector<IHwModuleViewObserver*> 	m_Observers;

public:
	IHwModuleViewSubject(void);
	virtual ~IHwModuleViewSubject(void);

	virtual void RegisterObserver(IHwModuleViewObserver *pObserver);
	virtual void UnregisterObserver(IHwModuleViewObserver *pObserver);

	virtual BOOL NotifySetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName);
	virtual BOOL NotifyRefreshHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL NotifyResetHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL NotifySetHwModuleDisplayName(CHwModule *pObj, CString sDisplayName);
	virtual BOOL NotifyReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName);
    virtual BOOL NotifyRemoveHwModule(CHwModule *pObj);
    virtual BOOL NotifySelectHwModule(CHwModule *pObj);
    virtual BOOL NotifyDeSelectHwModule(CHwModule *pObj);
	virtual BOOL NotifyAddUsbDeviceMsg(CString sDevId);
	virtual BOOL NotifyRemoveUsbDeviceMsg(CString sDevId);
	virtual BOOL NotifySaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	virtual BOOL NotifyLoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
};
