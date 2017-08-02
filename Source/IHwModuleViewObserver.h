#pragma once

class CHwModule;
class CHwModuleCtrl;
class CHwModuleCtrlUnitItem;

class IHwModuleViewObserver
{
public:
	IHwModuleViewObserver(void); 
	virtual ~IHwModuleViewObserver(void);

	virtual BOOL     OnViewSetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName){return TRUE;}
	virtual BOOL     OnViewRefreshHwModuleCtrl(CHwModuleCtrl *pCtrl)					     {return TRUE;}
	virtual BOOL     OnViewResetHwModuleCtrl(CHwModuleCtrl *pCtrl)	    				     {return TRUE;}
	virtual BOOL     OnViewSetHwModuleDisplayName(CHwModule *pHwModule, CString sDisplayName){return TRUE;}
	virtual BOOL     OnViewReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName) {return TRUE;}
	virtual BOOL     OnViewRemoveHwModule(CHwModule *pObj)                       {return TRUE;} 
	virtual BOOL     OnViewSelectHwModule(CHwModule *pObj)                       {return TRUE;}
	virtual BOOL     OnViewDeSelectHwModule(CHwModule *pObj)                     {return TRUE;}
	virtual BOOL     OnViewAddUsbDeviceMsg(CString sDevID)                       {return TRUE;}
	virtual BOOL     OnViewRemoveUsbDeviceMsg(CString sDevID)                    {return TRUE;}
	virtual BOOL     OnViewSaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl) {return TRUE;}
	virtual BOOL     OnViewLoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl) {return TRUE;}
	

};
