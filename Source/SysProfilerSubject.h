#pragma once
#include "SysProfilerEvent.h"

class ISysProfilerSubject
{
public:
	ISysProfilerSubject(void);
	virtual ~ISysProfilerSubject(void);

	virtual void NotifySysProfilerModelAddedEvent(CSysProfilerEvent *pEvent) = 0;
	virtual void NotifySysProfilerModelAddedLogLine(CString sLogLine) = 0;
	virtual void NotifySysProfilerModelRemoveAllEvents() = 0;

};
