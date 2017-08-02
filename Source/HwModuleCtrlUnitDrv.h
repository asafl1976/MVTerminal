#pragma once

class CHwModuleCtrlUnitDrv 
{
public:
	CHwModuleCtrlUnitDrv(void);
	virtual ~CHwModuleCtrlUnitDrv(void);
	virtual BOOL Init() {return TRUE;}
};
