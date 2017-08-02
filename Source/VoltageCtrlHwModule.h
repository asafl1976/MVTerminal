#pragma once
#include "hwmodule.h"

typedef enum _voltageCtrlChannel 
{
	VOLTAGE_CTRL_CHANNEL_A,
	VOLTAGE_CTRL_CHANNEL_B,
	VOLTAGE_CTRL_CHANNEL_UNKNOWN
} VOLTAGE_CTRL_CHANNEL;

class CVoltageCtrlHwModule : 	public CHwModule
{
public:
	CVoltageCtrlHwModule(void);
	CVoltageCtrlHwModule(CHwModuleInfo* pInfo);

	virtual ~CVoltageCtrlHwModule(void);

	virtual void  				 SetActiveChannel(VOLTAGE_CTRL_CHANNEL eChannel)  = 0;
	virtual VOLTAGE_CTRL_CHANNEL GetActiveChannel()  = 0;
	virtual BOOL  			     SetVoltage(int nVoltage)  = 0;
	virtual BOOL				 GetVoltage(int* pVoltage) = 0;

};
