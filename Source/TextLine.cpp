#include "StdAfx.h"
#include ".\textline.h"
#include "Markup.h"

CTextLine::CTextLine(void)
{
	m_nDelayBefore = 0; 
	m_nDelayAfter  = 0;
}

CTextLine::CTextLine(CString  sLine)
{
	m_sLine = sLine;
	m_nDelayBefore = 0; 
	m_nDelayAfter  = 0;
}
CTextLine::CTextLine(CString  sLine, UINT nDelayBefore, UINT nDelayAfter, BOOL bHasNewLine)
{	
	m_sLine          = sLine;
	m_nDelayBefore   = nDelayBefore; 
	m_nDelayAfter    = nDelayAfter;
	m_bHasNewLine = bHasNewLine;
}

CTextLine::~CTextLine(void)
{
}

CString  CTextLine::GetStripLine()
{
	CString sStripLine = GetLine();
	int     nPos;
	
	// Remove '//' remarks 
	if((nPos = sStripLine.Find("//")) != -1)
	{
		sStripLine = sStripLine.Left(nPos);
	}
	return sStripLine;
}

CString&  CTextLine::GetLine()
{
	return m_sLine;
}
UINT     CTextLine::GetDelayBefore()
{
	return m_nDelayBefore;
}
UINT     CTextLine::GetDelayAfter()
{
	return m_nDelayAfter;
}
void     CTextLine::SetLine(CString  sLine)
{
	m_sLine = sLine;
}
void     CTextLine::SetDelayBefore(UINT nDelay)
{
	m_nDelayBefore = nDelay;
}
void     CTextLine::SetDelayAfter(UINT nDelay)
{
	m_nDelayAfter = nDelay;
}
void    CTextLine::AppendChar(TCHAR ch)
{
	m_sLine += ch;
}


CString    CTextLine::Serialize()
{
	CMarkup  xmlWriter;

	xmlWriter.AddElem("TextLine");
	xmlWriter.AddAttrib("Line",m_sLine);
	xmlWriter.AddAttrib("DelayBefore",m_nDelayBefore);
	xmlWriter.AddAttrib("DelayAfter",m_nDelayAfter);
	xmlWriter.AddAttrib("HasNewLine",m_bHasNewLine);
	
	return xmlWriter.GetDoc();
}

BOOL    CTextLine::DeSerialize(CMarkup & xmlReader)
{
	CString  sStr;

	m_sLine = xmlReader.GetAttrib("Line");
	sStr = xmlReader.GetAttrib("DelayBefore");
	sscanf(sStr,"%d",&m_nDelayBefore);
	sStr = xmlReader.GetAttrib("DelayAfter");
	sscanf(sStr,"%d",&m_nDelayAfter);
	m_bHasNewLine = (xmlReader.GetAttrib("HasNewLine") == "0")? FALSE:TRUE;

	return TRUE;
}




