#pragma once
#include "Markup.h"
#include "sysprofilerbasicevent.h"
#include "SysProfilerAlternateColor.h"
#include <map>
using namespace std;

class CSysProfilerEvent : public CSysProfilerBasicEvent
{
	int       m_nMin;
	int       m_nMax;
	CString   m_sUnits;
	CString	  m_sColorKey;
	BOOL	  m_bUseAlternateColor;
	BOOL	  m_bEnableTwoDotLine;

	CSysProfilerAlternateColor* m_pAlternateColor;
	
	map<CString, COLORREF> m_colorMap;
public:
	int		  m_nRC;
	CSysProfilerEvent();
	~CSysProfilerEvent(void);

	virtual BOOL	  GetEnableTwoDotLine() {return m_bEnableTwoDotLine;}
	virtual void	  SetEnableTwoDotLine(BOOL bEnable){m_bEnableTwoDotLine = bEnable;}
	virtual BOOL	  GetUseAlternateColor() {return m_bUseAlternateColor;}
	virtual void	  SetUseAlternateColor(BOOL bUse){m_bUseAlternateColor = bUse;}
	virtual BOOL DeSerialize(CMarkup& xmlReader);
	virtual CString   GetUnits()  {return m_sUnits;}
	virtual COLORREF  GetDefaultColor();
	virtual COLORREF  GetColor(CString sColorKey);
	virtual COLORREF  GetGraphLineColor();
	virtual int       GetMin() {return m_nMin;}
	virtual int		  GetMax() {return m_nMax;}
	virtual BOOL      ExecutePhases();


};
