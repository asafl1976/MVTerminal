#pragma once

class CHwModuleItem;
class CHwModule;
class CHwModuleCtrl;
class CHwModuleCtrlUnit;

class IHwModuleView
{
public:
	IHwModuleView(void);
	virtual ~IHwModuleView(void);

	//Triggered by presenter
	virtual void OnAddHwModuleItem(CHwModule *pObj        , CHwModuleItem *pParentObj) = 0;
	virtual void OnAddHwModuleItem(CHwModuleCtrl *pObj    , CHwModuleItem *pParentObj) = 0;
	virtual void OnAddHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj) = 0;
	virtual BOOL OnRemoveHwModule(CHwModule *pObj)  	    		                   = 0;
	virtual BOOL OnRemoveHwModuleCtrl(CHwModuleCtrl *pObj)						       = 0;
	virtual BOOL OnReplaceHwModule(CHwModule* pObj,CHwModule *pNewObj)                 = 0;

	//Triggered by user
	virtual BOOL ReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName)       = 0;
	virtual BOOL RemoveHwModule(CHwModule *pNewObj)                                    = 0;
};
