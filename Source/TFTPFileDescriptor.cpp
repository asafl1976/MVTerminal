#include "StdAfx.h"
#include ".\tftpfiledescriptor.h"
#include "Markup.h"

CTFTPFileDescriptor::CTFTPFileDescriptor(void)
{
}
CTFTPFileDescriptor::CTFTPFileDescriptor(CString sFileName, CString sExecuteArguments,  
										 CString sLoadAddress)
{
	m_bForceCacheableLoadAddr = FALSE;
	m_bAutoDetectLoadAddress = TRUE;
	m_bLoadAndGo             = TRUE;
	m_sFileName              = sFileName;
	m_sExecuteArguments      = sExecuteArguments;
	m_sLoadAddress           = sLoadAddress;

}

CTFTPFileDescriptor::~CTFTPFileDescriptor(void)
{
}

	BOOL     DeSerialize(CMarkup & xmlReader);


CString    CTFTPFileDescriptor::Serialize()
{
	CMarkup xmlWriter;

	xmlWriter.AddElem("TFTPServerFileDesc");
	xmlWriter.AddAttrib("IsLoadAddressDetectable",m_bIsLoadAddressDetectable);
	xmlWriter.AddAttrib("AutoDetectLoadAddress",m_bAutoDetectLoadAddress);
	xmlWriter.AddAttrib("LoadAndGo",m_bLoadAndGo);
	xmlWriter.AddAttrib("ExecuteArguments",m_sExecuteArguments);
	xmlWriter.AddAttrib("FileName",m_sFileName);
	xmlWriter.AddAttrib("LoadAddress",m_sLoadAddress);

	return xmlWriter.GetDoc();
}

BOOL    CTFTPFileDescriptor::DeSerialize(CMarkup & xmlReader)
{
	m_bIsLoadAddressDetectable = (xmlReader.GetAttrib("IsLoadAddressDetectable") == "0")? FALSE:TRUE;
	m_bAutoDetectLoadAddress = (xmlReader.GetAttrib("AutoDetectLoadAddress") == "0")? FALSE:TRUE;
	m_bLoadAndGo = (xmlReader.GetAttrib("LoadAndGo") == "0")? FALSE:TRUE;
	m_sExecuteArguments = xmlReader.GetAttrib("ExecuteArguments");
	m_sFileName = xmlReader.GetAttrib("FileName");
	m_sLoadAddress = xmlReader.GetAttrib("LoadAddress");

	return TRUE;
}
