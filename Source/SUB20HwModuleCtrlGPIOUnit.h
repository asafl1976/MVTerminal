#pragma once
#include "HwModuleCtrlGPIOUnit.h"
#include "HwModuleCtrl.h"
#include "SUB20HwModuleCtrlGPIOUnitIOItem.h"
#include <vector>
#include <map>
using namespace std;


class CHwModuleCtrlGPIOUnitDrv;


class CSUB20HwModuleCtrlGPIOUnit : public CHwModuleCtrlGPIOUnit
{
	vector<CSUB20HwModuleCtrlGPIOUnitIOItem*> m_GPIOs;
	map<int,CGPIOConfig*>                     m_defaultGPIOConfigMap;

public:

	CSUB20HwModuleCtrlGPIOUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlGPIOUnitDrv* pGPIOUnitDrv);
	virtual ~CSUB20HwModuleCtrlGPIOUnit(void);

	virtual CHwModuleItem*					  GetSubHwModuleItem(list<CHwModuleItem>& ownerList) ;
	virtual int								  GetGPIOCount();
	virtual CHwModuleCtrlUnitItem*			  GetItem(CString sName);
	virtual CSUB20HwModuleCtrlGPIOUnitIOItem* GetGPIO(int nIndex);
	virtual BOOL						      SetDefaultGPIOMapping();
	virtual BOOL						      Init();

	virtual CString							  Serialize();
	virtual BOOL							  DeSerialize(CMarkup& xmlReader);

};
