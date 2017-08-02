#pragma once

#include "Markup.h"

class CTextLine
{
	CString   m_sLine;
	UINT	  m_nDelayBefore;
	UINT	  m_nDelayAfter;
	BOOL	  m_bHasNewLine;

public:
	CTextLine(void);
	CTextLine(CString  sLine);
	CTextLine(CString  sLine, UINT nDelayBefore, UINT nDelayAfter, BOOL bHasNewLine);

	CString&  GetLine();
	CString   GetStripLine();
	UINT      GetDelayBefore();
	UINT      GetDelayAfter();

	void      SetLine(CString  sLine);
	void      SetDelayBefore(UINT nDelay);
	void      SetDelayAfter(UINT nDelay);
	void      AppendChar(TCHAR ch);
	void      SetHasNewLine(BOOL bHasNewLine) {m_bHasNewLine = bHasNewLine;}
	BOOL      HasNewLine() {return m_bHasNewLine;}

	CString	  Serialize();
	BOOL      DeSerialize(CMarkup & xmlReader);


	~CTextLine(void);
};
