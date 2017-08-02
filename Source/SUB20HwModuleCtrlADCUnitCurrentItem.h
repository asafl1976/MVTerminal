#pragma once
#include "HwModuleCtrlADCUnitCurrentItem.h"

class CSUB20HwModuleCtrlADCUnit;

class CSUB20HwModuleCtrlADCUnitCurrentItem : public CHwModuleCtrlADCUnitCurrentItem
{
	CSUB20HwModuleCtrlADCUnit* m_pUnit;

	int		m_nIndex;
	int		m_nMin;
	int		m_nMax;
	float   m_nResistorValue;

public:
	CSUB20HwModuleCtrlADCUnitCurrentItem(int nIndex, CSUB20HwModuleCtrlADCUnit *pUnit);
	virtual ~CSUB20HwModuleCtrlADCUnitCurrentItem(void);
	virtual void    SetDisplayName(CString sName);
	virtual int     GetIndex() {return m_nIndex;}
	virtual BOOL    GetCurrent(float nResistorVal, int* pCurrent);
	virtual CString	Serialize();
	virtual void    SetResistorValue(float nResistorValue) {m_nResistorValue = nResistorValue;}
	virtual void    SetMin(int nMin) {m_nMin = nMin;}
	virtual void    SetMax(int nMax) {m_nMax = nMax;}
	virtual float   GetResistorValue()		 {return m_nResistorValue;}
	virtual int     GetMin()				 {return m_nMin;}
	virtual int     GetMax()				 {return m_nMax;}

};
