#include "StdAfx.h"
#include ".\sub20gpiohwmoduleboardpowerctrlresetstartegy.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"

CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy::CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy(void)
{
}

CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy::~CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy(void)
{
}

BOOL CSUB20GPIOHwModuleBoardPowerCtrlResetStartegy::Reset(int nPulseLength)
{
	CSUB20HwModuleCtrlGPIOUnit *pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(m_pHwModuleItem);
	if(pGPIOUnit == NULL)
		return FALSE;

	CSUB20HwModuleCtrlGPIOUnitIOItem *pIO  = pGPIOUnit->GetGPIO(m_nIO);
	if(pIO == NULL)
		return FALSE;

	if(pIO->SetType(GPIO_TYPE_OUTPUT) == FALSE)
	{
		return FALSE;
	}

	if(pIO->SetState(GPIO_STATE_LOW) == FALSE)
	{
		return FALSE;
	}

	Sleep(nPulseLength);

	if(pIO->SetType(GPIO_TYPE_INPUT) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

