#pragma once

#include "SysProfilerBasicEvent.h"
#include <vector>
using namespace std;

class CSysProfilerEventPhaseRegIO;

class CSysProfilerEventPhaseRegIOSection
{
protected:
	vector<CSysProfilerEventPhaseRegIO*>  m_regIO;
	CSysProfilerBasicEvent*				  m_pParentEvent;

public:
	CSysProfilerEventPhaseRegIOSection(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIOSection(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();
};

class CSysProfilerEventPhaseRegIO
{
protected:
	CSysProfilerBasicEvent*		m_pParentEvent;

public:
	CSysProfilerEventPhaseRegIO(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIO(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader) = 0;
	virtual  BOOL  Execute() = 0;
};

class CSysProfilerEventPhaseRegIORead : public CSysProfilerEventPhaseRegIO
{
	UINT32 m_nAddress;
	UINT32 m_nStoreAtDataArrayIndex;
	UINT32 m_nStoreAtGlobalDataArrayIndex;
public:
	CSysProfilerEventPhaseRegIORead(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIORead(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseRegIOWrite : public CSysProfilerEventPhaseRegIO
{	
	UINT32 m_nAddress;
	UINT32 m_nValue;
	BOOL   m_bUseValue;
	UINT32 m_nGlobalDataArrayIndex;
	UINT32 m_nDataArrayIndex;

public:
	CSysProfilerEventPhaseRegIOWrite(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIOWrite(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseRegIOReadModifyWrite : public CSysProfilerEventPhaseRegIO
{	
	UINT32 m_nAddress;
	UINT32 m_nMask;
	UINT32 m_nValue;

public:
	CSysProfilerEventPhaseRegIOReadModifyWrite(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIOReadModifyWrite(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseRegIOResetBit : public CSysProfilerEventPhaseRegIO
{	
	UINT32 m_nAddress;
	int    m_nBit;

public:
	CSysProfilerEventPhaseRegIOResetBit(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIOResetBit(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();

};

class CSysProfilerEventPhaseRegIOSetBit : public CSysProfilerEventPhaseRegIO
{	
	UINT32 m_nAddress;
	int    m_nBit;

public:
	CSysProfilerEventPhaseRegIOSetBit(CSysProfilerBasicEvent* pParentEvent);
	~CSysProfilerEventPhaseRegIOSetBit(void);
	virtual  BOOL  DeSerialize(CMarkup& xmlReader);
	virtual  BOOL  Execute();

};
