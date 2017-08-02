#pragma once
#include "IHwModuleModelObserver.h"
#include "IHwModuleViewObserver.h"
#include "IHwModulePresenter.h"

class CHwModuleTreeView;
class CHwModuleModel;
class CHwModuleItem;

class CHwModulePresenter : public IHwModuleModelObserver, public IHwModuleViewObserver, public IHwModulePresenter
{
	CHwModuleTreeView  *m_pHWMView;
	CHwModuleModel     *m_pHWMModel;

public:
	CHwModulePresenter(CHwModuleModel *pModel, CHwModuleTreeView* pView);
	virtual ~CHwModulePresenter(void);

	//View observer
	virtual BOOL       OnViewSetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName);
	virtual BOOL       OnViewRefreshHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL       OnViewResetHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL       OnViewSetHwModuleDisplayName(CHwModule *pHwModule, CString sDisplayName);
	virtual BOOL       OnViewReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName);
    virtual BOOL       OnViewRemoveHwModule(CHwModule *pObj);   
    virtual BOOL       OnViewSelectHwModule(CHwModule *pObj);   
    virtual BOOL       OnViewDeSelectHwModule(CHwModule *pObj);   
	virtual BOOL       OnViewAddUsbDeviceMsg(CString sDevID);
	virtual BOOL       OnViewRemoveUsbDeviceMsg(CString sDevID);
	virtual BOOL	   OnViewSaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	virtual BOOL	   OnViewLoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	
	//Model observer
	virtual BOOL       OnModelDeSelectHwModule(CHwModule *pObj);  
	virtual BOOL	   OnModelSelectHwModule(CHwModule *pObj);   
	virtual BOOL       OnModelRemoveHwModule(CHwModule *pObj) ;  
	virtual BOOL	   OnModelHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem *pObj);
	virtual BOOL       OnModelHwModuleDisplayNameChange(CHwModule *pObj);
	virtual BOOL	   OnModelRemoveHwModuleCtrl(CHwModuleCtrl *pObj);
	virtual void       OnModelCreateHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj);
	virtual void       OnModelCreateHwModuleItem(CHwModule *pObj        , CHwModuleItem *pParentObj);
	virtual void       OnModelCreateHwModuleItem(CHwModuleCtrl *pObj    , CHwModuleItem *pParentObj);
	virtual void       OnModelCreateHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj);
	virtual BOOL	   OnModelReplaceHwModule(CHwModule *pObj, CHwModule *pNewObj);

};
