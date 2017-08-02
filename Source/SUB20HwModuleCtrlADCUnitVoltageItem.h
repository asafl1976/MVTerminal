#pragma once
#include "HwModuleCtrlADCUnitVoltageItem.h"

class CSUB20HwModuleCtrlADCUnit;

class CSUB20HwModuleCtrlADCUnitVoltageItem : public CHwModuleCtrlADCUnitVoltageItem
{
	CSUB20HwModuleCtrlADCUnit* m_pUnit;

	int		m_nIndex;
	int		m_nMin;
	int		m_nMax;
public:

	CSUB20HwModuleCtrlADCUnitVoltageItem(int nIndex, CSUB20HwModuleCtrlADCUnit *pUnit);
	virtual ~CSUB20HwModuleCtrlADCUnitVoltageItem(void);
	virtual void    SetDisplayName(CString sName);
	virtual int     GetIndex() {return m_nIndex;}
	virtual BOOL    GetVoltage(int *pVoltage);
	virtual CString	Serialize();
	virtual void    SetMin(int nMin) {m_nMin = nMin;}
	virtual void    SetMax(int nMax) {m_nMax = nMax;}
	virtual int     GetMin()		 {return m_nMin;}
	virtual int     GetMax()         {return m_nMax;}
};
