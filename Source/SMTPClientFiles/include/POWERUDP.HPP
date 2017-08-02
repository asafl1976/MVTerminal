//***************************************************************************
//
//  Module: PowerUdp.hpp
//
//  Purpose:
//     Declares CPowerUdp class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1994 Dart Communications.  All Rights Reserved.
//
//***************************************************************************

#ifndef POWERUDP_HPP_DEFINED
#define POWERUDP_HPP_DEFINED
#include "powertcp.h"

// typedefs used here...

// Classes used...
class CUdp;

//
// CPowerUdp
// Defines the functions required to provide a TCP stream to parent
//
class CPowerUdp
{
friend class CUdp;
private:
	CUdp &Udp; // WINSOCK calls, etc here 
 	virtual PT_TYPE GetType (void);
protected:
	// assessor functions from Udp follow...
	virtual void ConnectEvent (LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName,
		WORD MaxByteCnt); 
 	virtual void ExceptionEvent(PT_EXCEPTION ErrorCode, LPCSTR ErrorDesc)=0;
	virtual void QueryEvent (WORD Version, LPSTR Description, LPSTR SystemStatus, 
		WORD MaxSockets, WORD MaxUdpDg, LPSTR LocalName, LPSTR FAR * LocalDotAddr); // .mjb.02
	virtual void RecvEvent (LPBYTE Data, UINT ByteCnt, LPCSTR RemoteDotAddr,
		WORD RemotePort);
	virtual void SendEvent (DWORD DataTag);
public:
	BOOL Close (BOOL NoDelay);
	BOOL Connect (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR LocalDotAddr, WORD LocalPort, PT_ENV Environment=PT_CPPDLL);
	BOOL Send (LPCSTR RemoteHost, WORD RemotePort, 
		LPVOID Data, UINT ByteCnt, DWORD DataTag);
	PT_STATE State (void);
	BOOL Query (void); // .mjb.02
	// 12/11/95 either use new CPowerUdp(hInst) or new CPowerUdp() and Init(hInst)
	CPowerUdp (HINSTANCE);
	CPowerUdp (void);
	void Init(HINSTANCE hInstance);
	~CPowerUdp (void);
	int operator = (CPowerUdp &);
};

#endif // POWERUDP_HPP_DEFINED

