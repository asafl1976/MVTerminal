#pragma once
#include "hwmoduleaccessstartegy.h"

class CODPGHwModuleAccessStartegy : public CHwModuleAccessStartegy
{
public:

	virtual BOOL  WriteRegister32(UINT nRegBase, UINT nRegOffset, UINT nRegValue, BOOL bReadModifyWrite = FALSE) = 0;
	virtual BOOL  ReadRegister32(UINT nRegBase, UINT nRegOffset,UINT *pRegValue)                                 = 0;

	CODPGHwModuleAccessStartegy(CHwModule *pHwModule);
	virtual ~CODPGHwModuleAccessStartegy(void);
};
