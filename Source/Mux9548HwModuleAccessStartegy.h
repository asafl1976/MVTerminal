#pragma once
#include "muxhwmoduleaccessstartegy.h"
#include "HwModuleCtrlI2CUnitDrv.h"
#include "HwModule.h"

class CMux9548HwModuleAccessStartegy : public CMuxHwModuleAccessStartegy
{
	CHwModuleCtrlI2CUnitDrv* m_pI2CUnitDrv;
public:
	virtual BOOL	SelectChannel(int nChannel);
	virtual BOOL	DeSelectChannels();

	CMux9548HwModuleAccessStartegy(CHwModule *pHWModule, CHwModuleCtrlI2CUnitDrv *pI2CUnitDrv);
	virtual ~CMux9548HwModuleAccessStartegy(void);
};
