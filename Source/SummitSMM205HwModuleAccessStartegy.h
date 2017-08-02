#pragma once
#include "voltagectrlhwmoduleaccessstartegy.h"
#include ".\voltagectrlhwmodule.h"

class CHwModuleCtrlI2CUnitDrv;
class CHwModule;



class CSummitSMM205HwModuleAccessStartegy : public CVoltageCtrlHwModuleAccessStartegy
{
	VOLTAGE_CTRL_CHANNEL  m_eChannel;
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;
	virtual BOOL	Unlock();

public:
	virtual void			     SetActiveChannel(VOLTAGE_CTRL_CHANNEL eChannel) {m_eChannel = eChannel;}
	virtual VOLTAGE_CTRL_CHANNEL GetActiveChannel() {return m_eChannel;}
	virtual BOOL 				 SetVoltage(int nVoltage);
	virtual BOOL 				 GetVoltage(int* pVoltage);
	CSummitSMM205HwModuleAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CSummitSMM205HwModuleAccessStartegy(void);
};
