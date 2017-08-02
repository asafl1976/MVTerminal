#pragma once
#include "sysprofilerbasicevent.h"

class CSysProfilerAfterEvents : public CSysProfilerBasicEvent
{
public:
	CSysProfilerAfterEvents(void);
	~CSysProfilerAfterEvents(void);
	virtual   BOOL DeSerialize(CMarkup& xmlReader);
	virtual   BOOL Run();

};
