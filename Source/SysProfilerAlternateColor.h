#pragma once
#include "SysProfilerEventPhaseRegIO.h"

class CSysProfilerEvent;

class CSysProfilerAlternateColor
{
	CSysProfilerEvent*			    	   m_pParentEvent;
	CSysProfilerEventPhaseRegIOSection*    m_pRegIOSection;
	CSysProfilerEventPhaseFunctionSection* m_pFunctionSection;

public:
	CSysProfilerAlternateColor(CSysProfilerEvent* pParentEvent);
	~CSysProfilerAlternateColor(void);

	virtual  BOOL      DeSerialize(CMarkup& xmlReader);
	virtual  BOOL	   Execute();
	virtual  COLORREF  GetColor();

};
