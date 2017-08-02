#pragma once

#include <list>
using namespace std;
#include "TextLine.h"
#include "Markup.h"

class CTextFile
{
	list<CTextLine>   m_textLineList;
	CString			  m_sName;
	CString			  m_sDelay;
	UINT			  m_nDelay;
	CWnd             *m_pWnd;

public:
	CTextFile();
	CTextFile(CString sName, CWnd* pWnd, CString  sDelay);
	~CTextFile(void);
	list<CTextLine>&  GetLineList() {return m_textLineList;}
	int				  GetLineCount();
	CString			  GetName();
	void			  SetName(CString sName);
	CString			  GetDelay();
	UINT			  GetDelayVal();
	void			  SetDelay(CString  sDelay);
	void			  AddLine(CTextLine line);
	void			  AddLine(CString sLine, BOOL bHasNewLine);
	CTextLine*		  GetTextLine(int nIndex);
	CString*		  GetLine(int nIndex);
	CWnd*   		  GetWnd();
	void    		  SetWnd(CWnd* pWnd);
	void    		  Clear() {m_textLineList.clear();}

	CString			  Serialize();
	BOOL			  DeSerialize(CMarkup & xmlReader);

};
