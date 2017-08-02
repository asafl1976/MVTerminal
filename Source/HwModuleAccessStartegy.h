#pragma once

class CHwModule;

class CHwModuleAccessStartegy
{
protected:
	CHwModule        *m_pHwModule;
	int			      m_nHwModuleID;
public:
	CHwModuleAccessStartegy(CHwModule *pHWModule);
	virtual ~CHwModuleAccessStartegy(void);
};





