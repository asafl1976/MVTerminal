#pragma once

#include "Markup.h"
#include <vector>
#include "SysProfilerEventPhaseFunctionOpCode.h"
using namespace std;
class CSysProfilerEventPhaseRegIO;

class CSysProfilerBasicEvent;
class CSysProfilerEventPhaseFunctionOpCode;
class CSysProfilerEventPhaseFunctionResult;
class CSysProfilerEventPhaseRegIOSection;

class CSysProfilerEventPhase
{
protected:
	CString							   			  m_sName;
	BOOL							   			  m_bExecuted;
	BOOL							   			  m_bExecuteOnce;
	CSysProfilerEventPhaseFunctionSection*	      m_pFunctionSection;
	CSysProfilerEventPhaseRegIOSection*			  m_pRegIOSection;
	CSysProfilerBasicEvent*	    				  m_pParentEvent;

	virtual  BOOL 								  LoadRegisterIO(CMarkup& xmlReader);
	virtual  BOOL 								  LoadFunction(CMarkup& xmlReader);
	virtual  BOOL 								  ExecuteFunction();
	virtual  BOOL 								  ExecuteRegIO();

public:
	CSysProfilerEventPhase(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhase(void);

	BOOL GetExecuteOnce() {return m_bExecuteOnce;}

	virtual  BOOL    DeSerialize(CMarkup& xmlReader) = 0;
	virtual  BOOL    Execute();

};

class CSysProfilerEventInitPhase : public CSysProfilerEventPhase
{

public:
	CSysProfilerEventInitPhase(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventInitPhase(void);

	virtual  BOOL  DeSerialize(CMarkup& xmlReader);

};

class CSysProfilerEventDataPhase : public CSysProfilerEventPhase
{
public:
	CSysProfilerEventDataPhase(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventDataPhase(void);

	virtual  BOOL    DeSerialize(CMarkup& xmlReader);
	virtual  double  GetSampleData();

};


