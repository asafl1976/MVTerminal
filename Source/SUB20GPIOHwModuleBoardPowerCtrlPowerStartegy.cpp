#include "StdAfx.h"
#include ".\sub20gpiohwmoduleboardpowerctrlpowerstartegy.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"

CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy::CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy()
{
}

CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy::~CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy(void)
{
}

BOOL CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy::PowerOn()
{
	CSUB20HwModuleCtrlGPIOUnit *pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(m_pHwModuleItem);
	if(pGPIOUnit == NULL)
		return FALSE;

	CSUB20HwModuleCtrlGPIOUnitIOItem *pIO  = pGPIOUnit->GetGPIO(m_nIO);
	if(pIO == NULL)
		return FALSE;

	if(pIO->SetType(GPIO_TYPE_OUTPUT) == FALSE)
		return FALSE;

	return pIO->SetState(GPIO_STATE_LOW);
}

BOOL CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy::PowerOff()
{
	CSUB20HwModuleCtrlGPIOUnit *pGPIOUnit = dynamic_cast<CSUB20HwModuleCtrlGPIOUnit*>(m_pHwModuleItem);
	if(pGPIOUnit == NULL)
		return FALSE;

	CSUB20HwModuleCtrlGPIOUnitIOItem *pIO  = pGPIOUnit->GetGPIO(m_nIO);
	if(pIO == NULL)
		return FALSE;

	return pIO->SetType(GPIO_TYPE_INPUT);
}

CHwModuleBoardPowerCtrlStartegy* CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy::Clone()
{
	return new CSUB20GPIOHwModuleBoardPowerCtrlPowerStartegy();
}

