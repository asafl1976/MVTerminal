#pragma once
#include "sysprofilerbasicevent.h"

class CSysProfilerInit : public CSysProfilerBasicEvent
{
public:
	CSysProfilerInit(void);
	~CSysProfilerInit(void);

	virtual   BOOL DeSerialize(CMarkup& xmlReader);
	virtual   BOOL Run();

};
