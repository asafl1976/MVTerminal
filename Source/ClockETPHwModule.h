#pragma once
#include "FrequencyCtrlHwModule.h"

class CClockETPParams
{
public:
	int  	m_nFrequency;
	int  	m_nRef;
	int  	m_nRange;

	CClockETPParams()
	{
		m_nFrequency = 25000;
		m_nRef       = 25000;
		m_nRange     = 2;
	}
};

class CClockETPHwModuleAccessStartegy;

class CClockETPHwModule :
	public CFrequencyCtrlHwModule
{

	CClockETPHwModuleAccessStartegy* m_pAccessStrategy;

public:
	CClockETPHwModule(void);
	CClockETPHwModule(CHwModuleInfo* pInfo);
	virtual ~CClockETPHwModule(void);
	virtual BOOL		 SetFrequancy(long nFreq, long nRef, long nRange);
	virtual CHwModule*   Clone(CHwModuleInfo* pInfo);

};
