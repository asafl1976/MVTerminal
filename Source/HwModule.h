#pragma once

#include "HwModuleCtrl.h"
#include "HwModuleItem.h"

#include <vector>
using namespace std;

class CHwModuleCtrlUnitDrv;
class CMuxHwModule;

class CHwModule : public CHwModuleItem 
{
protected:
	HWM_IF_TYPE			  m_eIFType; 
	CHwModuleCtrlUnit*    m_pParentUnit;

public:
	CHwModule();
	CHwModule(CHwModuleInfo *pInfo);
	virtual ~CHwModule(void);

	virtual  void               SetDisplayName(CString sDisplayName);
	virtual  CHwModuleCtrlUnit* GetParentUnit()						 {return m_pParentUnit;}
	virtual  CString	        GetInterfaceTypeName();
	virtual  HWM_IF_TYPE        GetInterfaceType()					 {return m_eIFType;}
	virtual  CString			Serialize();
	virtual  BOOL   			DeSerialize(CMarkup& xmlReader);
	virtual  CHwModule*         Clone(CHwModuleInfo* pInfo) = 0;
};
