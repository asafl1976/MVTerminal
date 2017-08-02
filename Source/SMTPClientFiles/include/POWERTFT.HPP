//***************************************************************************
//
//  Module: PowerTft.hpp 4/7/95
//
//  Purpose:
//     Declares CPowerTftp class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1994 Dart Communications.  All Rights Reserved.
//
//***************************************************************************

#ifndef POWERTFT_HPP_DEFINED
#define POWERTFT_HPP_DEFINED
#include "powerudp.hpp"

// typedefs used here...

//
// CPowerTftp
// Defines the functions required to provide a TCP stream to parent
//

// .mjb.1 9/25/97 Added PT_ENV support

class CTftpClient; // handles file transfers using Put() and Get()
class CTftpServer; // listens on TFTP well-known port and spawns a CFtpClient for each transfer

class CPowerTftp
{
friend class CTftpClient;
friend class CTftpServer;
private:
	CTftpClient *Client;
	CTftpServer *Server;
	WORD MaxRetries;
	WORD TimeoutInterval;
	PT_STATE TftpState;
	HINSTANCE hMyInstance;
	char LocalDotAddr[15];
	PT_FLAGS Flags; 
	char OemLicense[20];
	
protected:
	// virtual functions you inherit from CPowerUdp follow...
	virtual void ConnectEvent (LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName, WORD MaxByteCnt);
 	virtual void ExceptionEvent(PT_EXCEPTION ErrorCode, LPCSTR ErrorDesc)=0;
	// virtual functions you inherit from CPowerTftp follow...
	virtual void TftpEvent (TFTP_OPERATION Op, LPCSTR LocalFileSpec, WORD Block,
		UINT ByteCnt, DWORD TransferID, WORD ActiveCnt, LPCSTR RemoteDotAddr, WORD RemotePort,
		PT_EXCEPTION Error, LPCSTR ErrorDesc);
public:
	// member function inherited from CPowerUdp...
	BOOL AbortTransfer (DWORD TransferID);
	BOOL Close (BOOL);
	BOOL Connect (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR LocalDotAddr, 
		WORD TimeoutInterval, WORD MaxRetries, PT_ENV Environment=PT_CPPDLL);
	BOOL Get (LPCSTR RemoteHost, WORD RemotePort,
		LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, BOOL Ascii);
	BOOL Listen (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR LocalDotAddr, WORD LocalPort,
		WORD TimeoutInterval, WORD MaxRetries, PT_ENV Environment=PT_CPPDLL);
	BOOL Put (LPCSTR RemoteHost, WORD RemotePort,
		LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, BOOL Ascii);
	PT_STATE State (void);
	CPowerTftp (HINSTANCE);
	~CPowerTftp (void);
	int operator = (CPowerTftp &);
};

#endif // POWERTFT_HPP_DEFINED
