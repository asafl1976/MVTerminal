//***************************************************************************
//
//  Module: PowerTNT.hpp
//
//  Purpose:
//     Declares CPowerTnt class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1994 Dart Communications.  All Rights Reserved.
//
//***************************************************************************
//
// .mjb.1 SendData() method added so TELNET library can check on IAC characters
//        to double-up IAW RFC. Previously, applications had to do this.
//        Only relevant for streams that will be sending 0xff characters
//	.awp.2 9/30/98 Upped size of CmdBuf to 256

#ifndef POWERTNT_HPP_DEFINED
#define POWERTNT_HPP_DEFINED
#include "powertcp.hpp"

class CPowerTelnet : public CPowerTcp 
{
friend class CPowerTcp;
private:
	LPBYTE Search (LPBYTE ptr, BYTE ch, size_t limit);
	BOOL SendCr (LPVOID Data, UINT ByteCnt, BOOL Urgent, DWORD DataTag); //.mjb.4
  	void RecvCr (LPBYTE Data, UINT ByteCnt); // .mjb.4
	// Space for TELNET commands spanning receives
	BYTE CmdBuf[256];	// .awp.2
	BOOL CrSeen;
	int CmdCnt;
	void CheckTelnetCmd(void);
	// assessor functions from PowerTcp follow...
	PT_TYPE GetType (void);
protected:
	// protected functions must be overriden by derived classes...
	virtual void RecvEvent (LPBYTE Data, UINT ByteCnt);
  	virtual void TelnetRecvEvent (LPBYTE Data, UINT ByteCnt)=0;
	virtual void CmdEvent (BYTE Cmd, BYTE Option, LPBYTE SubOption, UINT SubOptionCnt);
public:
	BOOL SendCmd (BYTE Cmd, BYTE Option, LPBYTE SubOption, UINT SubOptionCnt, DWORD DataTag);
	BOOL SendData (LPVOID Data, UINT ByteCnt, BOOL Urgent, DWORD DataTag); //.mjb.01
	CPowerTelnet (HINSTANCE);
	~CPowerTelnet (void);
//	int operator = (CPowerTelnet &);
};

#endif // POWERTNT_HPP_DEFINED
