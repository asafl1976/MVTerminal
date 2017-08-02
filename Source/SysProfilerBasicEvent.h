#pragma once
#include "Markup.h"
#include "SysProfilerEventPhase.h"
#include <vector>
using namespace std;

class CSysProfilerBasicEvent
{
protected:
	CString						    m_sName;
	vector<CSysProfilerEventPhase*> m_phases;
	double						    m_varArray[64];
	double 						    m_nMinSample;
	double 							m_nMaxSample;

	virtual  BOOL      DeSerialize(CMarkup& xmlReader) = 0;
	virtual  BOOL      ExecutePhases();

public:
	CSysProfilerBasicEvent(void);
	virtual ~CSysProfilerBasicEvent(void);

	virtual  void      SetArrayValue(int nIndex, double nValue);
	virtual  double    GetArrayValue(int nIndex);

	virtual  CString   GetName() {return m_sName;}
	virtual  double    GetData();
	virtual  void      ResetMinMaxSampleData();
	virtual  double    GetMinSampleData() {return m_nMinSample;}
	virtual  double    GetMaxSampleData() {return m_nMaxSample;}
};
