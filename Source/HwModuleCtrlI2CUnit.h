#pragma once
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "IHwModuleContainer.h"

class CUnknownI2CHwModule;
class CHwModuleCtrl;

class CHwModuleCtrlI2CUnit : public CHwModuleCtrlUnit, public IHwModuleContainer
{
protected:
	CHwModuleCtrlI2CUnitDrv*          m_pI2CUnitDrv;

	virtual void   					  NotifyCreateHwModule(CHwModule* pHwModule);
public:
	CHwModuleCtrlI2CUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlI2CUnitDrv* pI2CUnitDrv);
	virtual ~CHwModuleCtrlI2CUnit(void);
	virtual CHwModuleItem*			  GetSubHwModuleItem(list<CHwModuleItem>& ownerList);
	virtual CHwModuleCtrlUnitDrv*     GetDriver(){return m_pI2CUnitDrv;}
	virtual BOOL				      SetFrequency(int nFreq);
	virtual int 				      GetFrequency();
	virtual BOOL				      DetectHwModules();
	virtual BOOL				      DetectHwModules(vector<CHwModuleInfo*>& HWMInfo,CHwModuleItem* pOwner);
	virtual CString					  Serialize();
	virtual BOOL					  DeSerialize(CMarkup& xmlReader);


};
