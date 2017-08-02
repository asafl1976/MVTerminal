//***************************************************************************
//
//  Module: PowerHTT.hpp
//
//  Purpose:
//     Declares CPowerHttp class
//             
//***************************************************************************
//
//  Written by Dart Communications Application Programming Group.
//  Copyright (c) 1997 Dart Communications.  All Rights Reserved.
//
//***************************************************************************
//

// .awp.1 Made declaration of static buffer for m_szRequest be dynamic.

#ifndef POWERHTTP_HPP_DEFINED
#define POWERHTTP_HPP_DEFINED
#include "powertcp.hpp"

class CDartByteArray;


class CPowerHttp : public CPowerTcp 
{
friend class CPowerTcp;
private:
	// assessor functions from PowerTcp follow...
	PT_TYPE GetType (void);
	char*		Headers;
	size_t		HeadersSize; 
	UINT		m_unParsingState; 

	// used for HttpLogEvent
	UINT		m_unContentCnt;
	UINT		m_unSendContentFileCnt;
	UINT		m_unRequestCnt;
	UINT		m_unRequestTotalCnt;

	// used for ContentEvent
	UINT		m_unRecvContent;
	UINT		m_unRecvContentLength;

	UINT		m_unRecvd; 
	LPCSTR		m_lpContent;
	BYTE		m_szRecvd[256];
	LPSTR		m_lpSendBuffer;
	HFILE		m_hRecvFile;
	OFSTRUCT	ofRecvFile;
	HFILE		m_hSendFile; 
	OFSTRUCT	ofSendFile;


	char*		m_HttpFieldMap[64]; //array of strings
	DWORD		OpenSendFile( LPCSTR ContentSendFile);
	void		ProcessContent( LPBYTE Content, UINT ContentCnt);
	BOOL		RecvContentFile( LPCSTR ContentRecvFile);
	BOOL		SendContentFile( void);
	HTTP_FIELD	HeaderFieldType( LPCSTR HeaderLine);
	HTTP_FIELD	m_unFieldType;
	LPCSTR		HeaderFieldValue( LPCSTR HeaderLine);
	// char		m_szRequest[120];	
	char		*m_szRequest;		//.awp.1
	UINT	m_RequestSize;			//.awp.1
	CDartByteArray *HeaderLines; 
	CDartByteArray *baAllHeaders;
	void SendEvent( DWORD dwDataTag);
	void RecvEvent (LPBYTE Data, UINT ByteCnt);

protected:
	// protected functions must be overriden by derived classes...
	virtual void ContentEvent (LPBYTE Content, UINT ContentCnt, 
					UINT CumContentCnt, UINT TotalContentCnt);
	virtual void HeaderLineEvent (HTTP_FIELD FieldType, 
					LPCSTR FieldValue, LPCSTR HeaderLine);
  	virtual void HttpRecvEvent (LPBYTE Data, UINT ByteCnt);
  	virtual void LogEvent (LPCSTR Data, UINT DataCnt, HTTP_SECTION Section, 
					UINT CumMessageCnt, UINT TotalMessageCnt);
  	virtual void StatusEvent (LPCSTR Version, LPCSTR StatusCode, LPCSTR ReasonPhrase);

public:
	LPCSTR AddHeaderLine (HTTP_FIELD FieldType, LPCSTR FieldValue, LPCSTR HeaderLine );
	BOOL SendRequest (HTTP_VER Version, HTTP_METHOD Method, LPCSTR Extension, LPCSTR URI, LPCSTR Headers,
		LPVOID Content, DWORD ContentCnt, LPCSTR ContentSendFile, LPCSTR ContentRecvFile);
	CPowerHttp (HINSTANCE);
	~CPowerHttp (void);
};

#endif // POWERHTTP_HPP_DEFINED
