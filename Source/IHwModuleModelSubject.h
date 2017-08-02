#pragma once
#include "IHwModuleModelObserver.h"
#include <vector>
using namespace std;

class CHwModule;
class CHwModuleCtrl;
class CHwModuleCtrlUnit;

class IHwModuleModelSubject
{	
	vector<IHwModuleModelObserver*> 	m_Observers;

public:
	IHwModuleModelSubject(void);
	virtual ~IHwModuleModelSubject(void);

	virtual void RegisterObserver(IHwModuleModelObserver *pObserver);
	virtual void UnregisterObserver(IHwModuleModelObserver *pObserver);	

	virtual void NotifyCreateHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj);
	virtual void NotifyCreateHwModuleItem(CHwModule *pObj        , CHwModuleItem *pParentObj);
	virtual void NotifyCreateHwModuleItem(CHwModuleCtrl *pObj    , CHwModuleItem *pParentObj);
	virtual void NotifyCreateHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj);
	virtual BOOL NotifyHwModuleDisplayNameChange(CHwModule *pObj);
	virtual void NotifyHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem *pObj);
	virtual BOOL NotifyRemoveHwModule(CHwModule *pObj);
	virtual BOOL NotifyRemoveHwModuleCtrl(CHwModuleCtrl *pObj);
	virtual BOOL NotifySelectHwModule(CHwModule *pObj);
	virtual BOOL NotifyDeSelectHwModule(CHwModule *pObj);
	virtual BOOL NotifyReplaceHwModule(CHwModule *pObj,CHwModule *pNewObj);

};
