#pragma once
#include "HwModuleInfo.h"
#include "HwModuleItem.h"
#include <list>
using namespace std;

class CHwModuleInfo;
class CHwModuleCtrl;
class CHwModuleCtrlUnitItem;

class CHwModuleCtrlUnit : public CHwModuleItem
{
protected:
	CHwModuleCtrl		       *m_pParent;

public:
	CHwModuleCtrlUnit(CHwModuleCtrl* pParent);
	virtual					       ~CHwModuleCtrlUnit(void);
	virtual CHwModuleItem*			GetSubHwModuleItem(list<CHwModuleItem>& ownerList) {return NULL;}
	virtual BOOL				    SetItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName) {return TRUE;}
	virtual CHwModuleItem*          Clone(CHwModuleInfo* pInfo)	 {return NULL;}
	virtual BOOL			        IsUnitItemExist(CHwModuleCtrlUnitItem* pItem){return FALSE;}
	virtual CHwModuleCtrlUnitItem*  GetItem(CString sName){return NULL;}
	virtual CHwModuleCtrlUnitDrv*   GetDriver()	{return NULL;}
	virtual BOOL Init()				{return TRUE;}
	virtual CString					Serialize();
	virtual BOOL					DeSerialize(CMarkup& xmlReader);


};
