#pragma once
#include "sysprofilerbasicevent.h"

class CSysProfilerEventInitPhase;
class CSysProfilerEventDataPhase;

class CSysProfilerTimeStampEvent : public CSysProfilerBasicEvent
{
	double						m_fLastTimeStamp;
	UINT32						m_nFreq;
	CSysProfilerEventInitPhase* m_pInitPhase;
	CSysProfilerEventDataPhase* m_pDataPhase;
public:
	CSysProfilerTimeStampEvent(void);
	virtual ~CSysProfilerTimeStampEvent(void);
	virtual double	   GetData();
	virtual  BOOL      ExecuteInitPhase();
	virtual  BOOL      ExecuteDataPhase();
	virtual  double    GetTimeStamp();
	virtual  BOOL      DeSerialize(CMarkup& xmlReader);
	
};
