#pragma once
#include "Markup.h"
#include <vector>
using namespace std;

class CSysProfilerBasicEvent;
class CSysProfilerEventPhaseFunctionOpCode;
class CSysProfilerEventPhaseFunctionResult;

class CSysProfilerEventPhaseFunctionSection
{
protected:
	vector<CSysProfilerEventPhaseFunctionOpCode*>  m_functionOpCodes;
	CSysProfilerBasicEvent*						   m_pParentEvent;
	CSysProfilerEventPhaseFunctionResult*          m_pResult; 

public:
	CSysProfilerEventPhaseFunctionSection(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionSection(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();
	virtual  double  GetResult();
};


class CSysProfilerEventPhaseFunctionOpCode
{
protected:
	CSysProfilerBasicEvent*		m_pParentEvent;
	UINT32					    m_nDataArrayIndex;
	UINT32					    m_nWithDataArrayIndex;
	UINT32					    m_nStoreAtDataArrayIndex;

	UINT32					    m_nGlobalDataArrayIndex;
	UINT32					    m_nWithGlobalDataArrayIndex;
	UINT32					    m_nStoreAtGlobalDataArrayIndex;

	BOOL	                    m_bConst;
	UINT32					    m_nConst;
	UINT32					    m_nMask;
public:
	CSysProfilerEventPhaseFunctionOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionOpCode(void);

	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute() = 0;

};

class CSysProfilerEventPhaseFunctionAndOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
	UINT32 m_nMask;
public:
	CSysProfilerEventPhaseFunctionAndOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionAndOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionShiftLeftOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionShiftLeftOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionShiftLeftOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionShiftRightOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionShiftRightOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionShiftRightOpCode(void);

	virtual  BOOL  Execute();

};


class CSysProfilerEventPhaseFunctionMulOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionMulOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionMulOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionAddOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionAddOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionAddOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionDivOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionDivOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionDivOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionSubOpCode : public CSysProfilerEventPhaseFunctionOpCode
{
public:
	CSysProfilerEventPhaseFunctionSubOpCode(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionSubOpCode(void);

	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseFunctionResult
{
	CSysProfilerBasicEvent*		m_pParentEvent;
	int  					    m_nDataArrayIndex;
	int                         m_nGlobalDataArrayIndex;

public:
	CSysProfilerEventPhaseFunctionResult(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseFunctionResult(void);

	virtual  double  GetResult();
	virtual  BOOL    DeSerialize(CMarkup& xmlReader);

};