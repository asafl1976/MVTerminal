#pragma once
#include "Markup.h"

class CTFTPFileDescriptor
{
	CString		m_sFileName;
	CString		m_sExecuteArguments;
	CString		m_sLoadAddress;
	BOOL		m_bAutoDetectLoadAddress;
	BOOL		m_bLoadAndGo;
	BOOL		m_bIsLoadAddressDetectable;
	BOOL        m_bForceCacheableLoadAddr;

public:
	CTFTPFileDescriptor();
	CTFTPFileDescriptor(CString sFileName, CString sExecuteArgumentsm, CString sLoadAddress);
	virtual ~CTFTPFileDescriptor(void);
	void     SetFileName(CString sFileName) {m_sFileName = sFileName;}
	void     SetLoadAddress(CString sLoadAdrress) {m_sLoadAddress = sLoadAdrress;}
	void     SetExecuteArguments(CString sExecuteArguments) {m_sExecuteArguments = sExecuteArguments;}
	void     SetLoadAndGo(BOOL bLoadAndGo) {m_bLoadAndGo = bLoadAndGo;}
	void     SetAutoDetectLoadAddress(BOOL bAutoDetectLoadAddress) {m_bAutoDetectLoadAddress = bAutoDetectLoadAddress;}
	void     SetIsLoadAddressDetectable(BOOL bIsLoadAddressDetectable) {m_bIsLoadAddressDetectable =  bIsLoadAddressDetectable;}
	void     SetForceCacheableLoadAddr(BOOL bForceCacheableLoadAddr) {m_bForceCacheableLoadAddr =  bForceCacheableLoadAddr;}

	CString  GetFileName() {return m_sFileName;}
	CString  GetLoadAddress() {return m_sLoadAddress;}
	CString  GetExecuteArguments() {return m_sExecuteArguments;}
	BOOL     GetLoadAndGo() {return m_bLoadAndGo;}
	BOOL     GetAutoDetectLoadAddress() {return m_bAutoDetectLoadAddress;}
	BOOL     GetIsLoadAddressDetectable() {return m_bIsLoadAddressDetectable;}
	BOOL     GetForceCacheableLoadAddr() {return m_bForceCacheableLoadAddr;}

	CString  Serialize();
	BOOL     DeSerialize(CMarkup & xmlReader);

};
