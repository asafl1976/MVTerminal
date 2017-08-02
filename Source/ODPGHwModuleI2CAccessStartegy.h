#pragma once
#include "ODPGHwModuleAccessStartegy.h"

class CHwModuleCtrlI2CUnitDrv;

class CODPGHwModuleI2CAccessStartegy : public CODPGHwModuleAccessStartegy
{	
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;

public:
	CODPGHwModuleI2CAccessStartegy(CHwModule *pHwModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CODPGHwModuleI2CAccessStartegy(void);

	virtual BOOL  WriteRegister32(UINT nRegBase, UINT nRegOffset, UINT nRegValue, BOOL bReadModifyWrite = FALSE);
	virtual BOOL  ReadRegister32(UINT nRegBase, UINT nRegOffset,UINT *pRegValue);

};
