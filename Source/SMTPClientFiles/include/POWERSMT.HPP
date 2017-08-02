//***************************************************************************
//
//  Module: Powersmt.hpp
//
//  Purpose:
//     Declares CPowerSMTP class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1995 Dart Communications.  All Rights Reserved.
//
//***************************************************************************

// .awp.1	12/7/98 corrected 16-bit new / delete problem

#ifndef POWERSMT_HPP_DEFINED
#define POWERSMT_HPP_DEFINED
#include "powertcp.hpp"
#include "mime.hpp"
#include "uucode.hpp"

#include <stdio.h>
#include <string.h> 
#include <malloc.h>
#include <time.h>
#include <io.h>

#pragma pack(16)
// typedefs used here...

// Classes used...
//
// CPowerSmtp
// Defines the functions required to provide Smtp processing
//

class CPowerSmtp : public CPowerTcp
{  
private:
    // private data for CPowerSmtp
	char			LocalName[64];
	char			SenderLocal[255];
	char			attachments[1024];
	// .awp.1	
	// LPSTR			pRecipients;
	char			*pRecipients;
	LPSTR			pNextRecipient;
	char			attachArray[20][128];
	LPSTR			pBeg;
	LPSTR			pEnd;
	char			eArray[20][10];
	UINT			nAttachments;
	UINT			nTypes;
	UINT			usedAttachments;
	UINT			oldByteCnt; 
	HFILE			hInfile;
	SMTP_COMMAND CurrentCommand;
	HGLOBAL		hglb;
	LPSTR			MessageBuff;
	char			holdfilename[255];
	char			eTypeLocal[64];
	char			eTypes[144];
	char			szBuf[144];
	long			TotalBytesToSend;
	long			TotalBytesSent;
	char			attachmentsLocal[1024];
	char			temp[1024];
	char			first3[4];
	long			TotalBytes;
	BOOL			DeleteFile; 
	BOOL			InitialConnect;
	CMime			MIME;
	CUucode		UUCODE;
	LPSTR parseAdr( LPSTR str );	// parse addre strings
	BOOL MailAttach(void);
	BOOL EncodeMime(LPCSTR infile, LPCSTR outfile);
	BOOL DecodeMime(LPCSTR infile, LPCSTR outfile);
	void ConnectEvent (LPCSTR RemoteDotAddr, WORD RemotePort, LPCSTR LocalDotAddr,
							WORD LocalPort, LPCSTR LocalName); 
	void RecvEvent (LPBYTE Data, UINT ByteCnt);
	void SendEvent (DWORD DataTag);
	PT_TYPE GetType (void);

protected:	
	// Constructor & destructor
	CPowerSmtp (HINSTANCE);
	~CPowerSmtp (void);
	// virtual CPowerSmtp functions that you can override... 
	virtual void ConnectSmtpEvent(LPCSTR RemoteDotAddr, WORD RemotePort, LPCSTR LocalDotAddr,
							WORD LocalPort, LPCSTR LocalName);
	virtual void SmtpEvent(SMTP_STATUS  Status, SMTP_COMMAND LastCommand, int ReplyCode,
						   LPCSTR ReplyStr, BOOL Complete);
	virtual void LogEvent(LPCSTR Command);
	virtual void AttachEvent(LPCSTR FileSpec, long PercentComplete);

public: 
	BOOL Close (BOOL NoDelay);
	BOOL Expand (LPCSTR MailingList);	
	BOOL Help (SMTP_COMMAND Topic);    
	BOOL Mail (SMTP_TYPE SmtpType, LPCSTR Sender, LPCSTR Recipients, BOOL HeaderDate,
					BOOL HeaderFrom, LPCSTR HeaderSubject, BOOL HeaderTo, LPVOID Message, 
					UINT ByteCnt, LPCSTR Attachments, LPCSTR eType);    
	BOOL Noop (void);    
	BOOL Reset (void);    
	BOOL SendCmd(LPVOID  Data, UINT ByteCnt, DWORD DataTag);    
	BOOL Turn(void);    
	BOOL Verify(LPCSTR UserName);
};

#pragma pack()

#endif // POWERSMT_HPP_DEFINED

