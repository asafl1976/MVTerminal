//***************************************************************************
//
//  Module: Powerpop.hpp
//
//  Purpose:
//     Declares CPowerPop3 class for Post Office Protocol implementation.
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1996 Dart Communications.  All Rights Reserved.
//
//***************************************************************************

// .mjb.1	9/25/97 Added PT_ENV support

#ifndef POWERPOP_HPP_DEFINED
#define POWERPOP_HPP_DEFINED

#include "powertcp.hpp"

class CPop3;

class CPowerPop3
{  
friend class CPop3;
private:
	CPop3 &Pop;		

protected:
	// virtual CPowerPop3 callback functions provided by CPowerPop3 
	virtual void ConnectPop3Event(LPCSTR RemoteDotAddr, WORD RemotePort, LPCSTR LocalDotAddr,
	  				WORD LocalPort, LPCSTR LocalName);
	virtual void Pop3Event(POP3_STATUS  Status, POP3_COMMAND LastCommand, LPBYTE Reply,
					       UINT ByteCnt, LPCSTR FileSpec, char Mode, WORD PercentComplete);
	virtual void LogEvent(LPCSTR Command);

	virtual void FileEvent(LPCSTR FileSpec, char Mode, DWORD FileSize);

	virtual void ExceptionEvent(PT_EXCEPTION Exception,LPCSTR Msg);

	virtual void HeaderEvent( POP3_COMMAND nLastCommand, LPCSTR lpReturnPath, LPCSTR lpReceived, LPCSTR lpDate, 
					LPCSTR lpFrom, LPCSTR lpSubject, LPCSTR lpSender, LPCSTR lpTo, LPCSTR lpCc );

public: 

    BOOL Close(BOOL NoDelay);
    BOOL Connect(LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR RemoteHost, WORD RemotePort, LPCSTR LocalDotAddr, 
    	         WORD LocalPort, LPCSTR User, LPCSTR Password, LPCSTR Secret, PT_ENV Environment=PT_CPPDLL);
    BOOL Delete(WORD MessageNumber);
    BOOL List(WORD MessageNumber);
    BOOL Noop(void);
    BOOL Reset(void);
    BOOL Retrieve( WORD MessageNumber,LPCSTR MailFile, LPCSTR AttachmentDir );
    BOOL Status(void);
    BOOL Top(WORD MessageNumber, WORD Lines);
    BOOL Uidl(WORD MessageNumber);
	PT_STATE State (void);

	// Constructor & destructor
	CPowerPop3 (HINSTANCE);
	~CPowerPop3 (void);
	int operator = (CPowerPop3 &);
};

#endif // POWERPOP_HPP_DEFINED
