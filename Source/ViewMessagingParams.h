#pragma once

class CViewMessagingParams
{
	BOOL    m_bMessagingEn;
	BOOL    m_bAppendNewLine;

public:
	CViewMessagingParams(void);
	virtual ~CViewMessagingParams(void);

	BOOL		 GetMessagingEn() {return m_bMessagingEn;}
	void		 SetMessagingEn(BOOL bMessagingEn) {m_bMessagingEn = bMessagingEn;}

	BOOL		 GetAppendNewLine() {return m_bAppendNewLine;}
	void		 SetAppendNewLine(BOOL bAppendNewLine) {m_bAppendNewLine = bAppendNewLine;}

};
