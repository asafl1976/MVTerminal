#pragma once
#include "frequencyctrlhwmoduleaccessstartegy.h"

class CHwModuleCtrlI2CUnitDrv;

class CClockETPHwModuleAccessStartegy : public CFrequencyCtrlHwModuleAccessStartegy
{
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;

public:
	CClockETPHwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CClockETPHwModuleAccessStartegy(void);

	virtual BOOL	SetFrequency(long nFreq, long nRef, long nRange);

};
