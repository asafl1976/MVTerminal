#pragma once
#include "sysprofilerbasicevent.h"

class CSysProfilerBeforeEvents : public CSysProfilerBasicEvent
{
public:
	CSysProfilerBeforeEvents(void);
	~CSysProfilerBeforeEvents(void);

	virtual   BOOL DeSerialize(CMarkup& xmlReader);
	virtual   BOOL Run();

};
