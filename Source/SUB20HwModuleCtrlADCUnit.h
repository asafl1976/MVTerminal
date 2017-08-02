#pragma once
#include "HwModuleCtrlUnit.h"
#include "HwModuleCtrl.h"
#include "HwModuleCtrlADCUnitDrv.h"
#include "HwModuleCtrlADCUnit.h"
#include <vector>
using namespace std;

class CHwModuleCtrlADCUnitItem;
class CSUB20HwModuleCtrlADCUnitCurrentItem;
class CSUB20HwModuleCtrlADCUnitVoltageItem;

class CSUB20HwModuleCtrlADCUnit : public CHwModuleCtrlADCUnit
{
	vector<CHwModuleCtrlADCUnitItem*> m_unitItems;

public:

	CSUB20HwModuleCtrlADCUnit(CHwModuleCtrl *pHwModuleCtrl,CHwModuleCtrlADCUnitDrv* pADCUnitDrv);
	virtual ~CSUB20HwModuleCtrlADCUnit(void);
	virtual BOOL        Init();
	virtual BOOL	    IsUnitItemExist(CHwModuleCtrlUnitItem* pItem);
	virtual CHwModuleCtrlUnitItem*  GetItem(CString sName);
	virtual BOOL		SetItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName);
	virtual CSUB20HwModuleCtrlADCUnitCurrentItem* GetCurrentItem(int nIndex);
	virtual CSUB20HwModuleCtrlADCUnitVoltageItem* GetVoltageItem(int nIndex);
	virtual CString		Serialize();
	virtual BOOL		DeSerialize(CMarkup& xmlReader);

};

