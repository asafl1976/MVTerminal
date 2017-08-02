#pragma once
#include "SysProfilerEvent.h"

class ISysProfilerObserver
{
public:
	ISysProfilerObserver(void);
	virtual ~ISysProfilerObserver(void);

	virtual void OnSysProfilerModelAddedEvent(CSysProfilerEvent *pEvent) = 0;
	virtual void OnSysProfilerModelAddedLogLine(CString sLogLine) = 0;
	virtual void OnSysProfilerModelRemoveAllEvents() = 0;
	
};
