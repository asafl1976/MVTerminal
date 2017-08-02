//***************************************************************************
//
//  Module: PowerTcp.hpp
//
//  Purpose:
//     Declares CPowerTcp class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1994 Dart Communications.  All Rights Reserved.
//
//***************************************************************************
//
// Revision History
//
// .tve.01  10/23/96 Changed public Send to virtual to allow it to be overriden
//			in CPowerTelnet supporting callouts for encryption (cybersafe)
// .mjb.02  12/24/96 Added Query() and QueryEvent() methods
// .mjb.03  9/23/97 Added PT_ENV parameter
//
#ifndef POWERTCP_HPP_DEFINED
#define POWERTCP_HPP_DEFINED
#include "powertcp.h"

// PowerTCP.h should be included before this file

// typedefs used here...

// Classes used...
class CTcp;

//
// CPowerTcp
// Defines the functions required to provide a TCP stream to parent
//
class CPowerTcp
{
friend class CTcp;
private:
	CTcp &Tcp; // WINSOCK calls, etc here 
protected:
	// assessor functions from Tcp follow...
	virtual void ConnectEvent (LPCSTR RemoteDotAddr, WORD RemotePort, 
		LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName); 
	virtual void RecvEvent (LPBYTE Data, UINT ByteCnt);
	virtual void SendEvent (DWORD DataTag);
	virtual void ListenEvent (LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName);
	virtual void AcceptEvent (void);
  	virtual void ExceptionEvent(PT_EXCEPTION ErrorCode, LPCSTR ErrorDesc)=0;
	virtual void QueryEvent (WORD Version, LPSTR Description, LPSTR SystemStatus, 
		WORD MaxSockets, WORD MaxUdpDg, LPSTR LocalName, LPSTR FAR * LocalDotAddr); // .mjb.02
	virtual PT_TYPE GetType (void);
public:
	BOOL Connect (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR RemoteHost, WORD RemotePort, 
		LPCSTR LocalDotAddr, WORD LocalPort, PT_ENV = PT_CPPDLL);
	BOOL Accept (LPCSTR OemLicense, PT_FLAGS Flags, CPowerTcp &Listener, PT_ENV = PT_CPPDLL);
	virtual BOOL Send (LPVOID Data, UINT ByteCnt, BOOL Urgent, DWORD DataTag); //.tve.01
	BOOL Recv (UINT MaxByteCnt);
	BOOL Listen (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR LocalDotAddr, WORD LocalPort, WORD Timeout=0, PT_ENV = PT_CPPDLL);
	BOOL Close (BOOL NoDelay);
	// version 4.0 dropped support for GetCount() {not used}
	// void GetCount (DWORD FAR *InCnt, DWORD FAR *OutCnt);
	PT_STATE State (void);
	BOOL Query (void); // .mjb.02
	CPowerTcp (HINSTANCE);
	~CPowerTcp (void);
	int operator = (CPowerTcp &);
};


#endif // POWERTCP_HPP_DEFINED
