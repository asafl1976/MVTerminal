#pragma once

class CHwModuleItem;
class CHwModule;
class CHwModuleCtrl;
class CHwModuleCtrlUnit;
class CHwModuleCtrlUnitItem;

class IHwModuleModelObserver 
{
public:
	IHwModuleModelObserver(void);
	virtual ~IHwModuleModelObserver(void);

	virtual BOOL OnModelDeSelectHwModule(CHwModule *pObj)                                     {return TRUE;}   
	virtual BOOL OnModelSelectHwModule(CHwModule *pObj)										  {return TRUE;}   
	virtual BOOL OnModelRemoveHwModule(CHwModule *pObj)                                       {return TRUE;} 
	virtual BOOL OnModelHwModuleDisplayNameChange(CHwModule *pObj)                            {return TRUE;}
	virtual BOOL OnModelHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem *pObj)    {return TRUE;}
	virtual BOOL OnModelRemoveHwModuleCtrl(CHwModuleCtrl *pObj)                               {return TRUE;}

	
	virtual void OnModelCreateHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj){}
	virtual void OnModelCreateHwModuleItem(CHwModule *pObj            , CHwModuleItem *pParentObj){}
	virtual void OnModelCreateHwModuleItem(CHwModuleCtrl *pObj        , CHwModuleItem *pParentObj){}
	virtual void OnModelCreateHwModuleItem(CHwModuleCtrlUnit *pObj    , CHwModuleItem *pParentObj){}
	virtual BOOL OnModelReplaceHwModule(CHwModule *pObj, CHwModule *pNewObj)                  {return TRUE;}   

};
