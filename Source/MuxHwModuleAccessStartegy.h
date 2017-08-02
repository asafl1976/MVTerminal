#pragma once
#include "hwmoduleaccessstartegy.h"

class CMuxHwModuleAccessStartegy : public CHwModuleAccessStartegy
{
public:
	virtual BOOL	SelectChannel(int nChannel) = 0;
	virtual BOOL	DeSelectChannels()          = 0;

	CMuxHwModuleAccessStartegy(CHwModule *pHWModule);
	virtual ~CMuxHwModuleAccessStartegy(void);
};
