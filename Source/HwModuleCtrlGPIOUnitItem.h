#pragma once
#include "hwmodulectrlunititem.h"

#define GPIO_TYPE_INPUT  0
#define GPIO_TYPE_OUTPUT 1
#define GPIO_STATE_LOW   0
#define GPIO_STATE_HIGH  1


class CHwModuleCtrlGPIOUnitItem : public CHwModuleCtrlUnitItem
{
public:
	CHwModuleCtrlGPIOUnitItem(void);
	virtual ~CHwModuleCtrlGPIOUnitItem(void);

	virtual BOOL    GetState(int& nState) = 0;
	virtual BOOL    SetState(int  nState) = 0;

	virtual BOOL    GetType(int& nType) = 0;
	virtual BOOL    SetType(int  nType) = 0;

};
