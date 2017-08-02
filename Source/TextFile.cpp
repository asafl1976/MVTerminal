#include "StdAfx.h"
#include "TextFile.h"
#include "Markup.h"

CTextFile::CTextFile(CString sName, CWnd* pWnd, CString  sDelay)
{
	SetDelay(sDelay);
	SetWnd(pWnd);
	SetName(sName);
}
CTextFile::CTextFile()
{
	m_nDelay = 0;
}

CTextFile::~CTextFile(void)
{
}

int  CTextFile::GetLineCount()
{
	return (int)m_textLineList.size();
}
CString  CTextFile::GetName()
{
	return m_sName;
}
void   CTextFile::SetName(CString sName)
{
	m_sName = sName;
}

CString  CTextFile::GetDelay()
{
	return m_sDelay;
}
UINT  CTextFile::GetDelayVal()
{	
	return m_nDelay;
}
void  CTextFile::SetDelay(CString  sDelay)
{
	m_sDelay = sDelay;
	sscanf(m_sDelay,"%d",&m_nDelay);
}
void   CTextFile::AddLine(CTextLine line)
{
	line.SetDelayAfter(m_nDelay);
	line.SetDelayBefore(0);
	m_textLineList.push_back(line);
}

void   CTextFile::AddLine(CString sLine, BOOL bHasNewLine)
{
	CTextLine  txtLine(sLine,0,m_nDelay,bHasNewLine);
	m_textLineList.push_back(txtLine);
}


CTextLine*  CTextFile::GetTextLine(int nIndex)
{
	list<CTextLine>::iterator  iter = m_textLineList.begin();
	int                      nLineCount = 0;

	for(;iter != m_textLineList.end(); iter++)
	{
		if(nLineCount == nIndex)
			return &(*iter);
		nLineCount++;
	}
	return NULL;
}
CString* CTextFile::GetLine(int nIndex)
{
	list<CTextLine>::iterator  iter = m_textLineList.begin();
	int                      nLineCount = 0;

	for(;iter != m_textLineList.end(); iter++)
	{
		if(nLineCount == nIndex)
			return &(*iter).GetLine();
		nLineCount++;
	}
	return NULL;
}
CWnd*   CTextFile::GetWnd()
{
	return m_pWnd; 
}

void   CTextFile::SetWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;
}

	

CString    CTextFile::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("Script");
	xmlWriter.AddAttrib("Name",m_sName);
	xmlWriter.AddAttrib("Delay",m_sDelay);
	xmlWriter.IntoElem();

	list<CTextLine>::iterator iter = m_textLineList.begin(), endIter = m_textLineList.end();
	for(; iter != endIter ;iter++)
	{
		xmlWriter.AddSubDoc( (*iter).Serialize() );
	}

	xmlWriter.OutOfElem();
	return xmlWriter.GetDoc();
}

BOOL    CTextFile::DeSerialize(CMarkup & xmlReader)
{
	CString  sStr;

	m_sName = xmlReader.GetAttrib("Name");
	m_sDelay = xmlReader.GetAttrib("Delay");
	sscanf(m_sDelay,"%d",&m_nDelay);

	xmlReader.IntoElem(); 
	while(xmlReader.FindElem("TextLine") == true)
	{
		CTextLine  textLine;
		if(textLine.DeSerialize(xmlReader) == FALSE)
			return FALSE;
		AddLine(textLine);		
	}

	xmlReader.OutOfElem();
	return TRUE;

}



