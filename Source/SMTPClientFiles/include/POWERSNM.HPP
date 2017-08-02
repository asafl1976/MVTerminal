//***************************************************************************
//
//  Module: PowerSnm.hpp
//
//  Purpose:
//     Declares CPowerSnmp class
//             
//***************************************************************************
//
//  Written by Dart Communication Application Programming Group.
//  Copyright (c) 1995 Dart Communications.  All Rights Reserved.
//
//***************************************************************************

#ifndef POWERSNM_HPP_DEFINED
#define POWERSNM_HPP_DEFINED
#include "powerudp.hpp"

// typedefs used here...

// Classes used...
//
// CPowerSnmp
// Defines the functions required to provide SNMP processing
//
class CPowerSnmp : public CPowerUdp
{  
private:
	WORD LocalPort; // 161 for Agent, 162 for Trap, other for management functions
	WORD PktSize; // Max size allowed for datagram...size of packet buffer
	LPSTR Pkt; // global allocation for packet buffer
	LPSTR Scratch; // global scratch array 
	char License[20]; // local storage for OEM license or PowerTCP License
	// virtual CPowerUdp functions we provide a body for...
 	PT_TYPE GetType (void);
	void ConnectEvent (LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName, WORD MaxByteCnt); 
	void RecvEvent (LPBYTE Data, UINT ByteCnt, LPCSTR RemoteDotAddr, WORD RemotePort);
	LPBYTE GetOctet(BYTE tag, DWORD len, DWORD FAR* octetstring, LPBYTE Data);  // returns octet string in ... and Data is incremented
	LPBYTE GetInteger(BYTE tag, DWORD len, LPLONG integer, LPBYTE Data);  // returns integer in... and sets data pointer ahead
	void GetOIDs(LPSTR FAR* FAR*ObjectID, LONG FAR* FAR*ObjectValue, SNMP_OBJECT_TYPE FAR* FAR*ObjectType, UINT FAR* nObjects,
			LPBYTE Data);
	//LONG GetNumberOids(LPBYTE Data);
	void StrtoIP(LPBYTE IPString);  // puts 4 bytes in IPString
	void SnmpStrtoIP(LPSNMP_STRING IPString);  // puts 4 bytes in IPString
	void StrtoOID(LPCSTR OIDString, LPINT ActualOidSize, DWORD FAR *TempOid); 
	void OIDtoStr(LONG len, LPSTR retstr); 
	BOOL BuildMsg(LPCSTR Community, DWORD RequestID, SNMP_ERROR ErrorStatus, WORD ErrorIndex, SNMP_TYPE pkt_type, 
			LPSTR FAR* ObjectID, LONG FAR* ObjectValue, SNMP_OBJECT_TYPE FAR* ObjectType, UINT nObjects, 
			LPCSTR RemoteHost, WORD RemotePort); // builds get and get next msgs
	LPBYTE AddOids(LPSTR FAR* ObjectID, LONG FAR* ObjectValue, SNMP_OBJECT_TYPE FAR* ObjectType, UINT nObjects, LPBYTE packet, 
			int FAR* index, SNMP_TYPE pkt_type); // add object ids to the packet
	HINSTANCE hMyInstance;
protected:
	// virtual CPowerSnmp functions provided by CPowerSnmp...
	virtual void RecvSnmpEvent (LPCSTR Community, DWORD RequestID, 
		SNMP_ERROR SnmpError, int ErrorIndex, SNMP_TYPE MessageType,
		UINT nObjects, LPSTR FAR* ObjectID, LONG FAR* ObjectValue, 
		SNMP_OBJECT_TYPE FAR* ObjectType, LPCSTR RemoteDotAddr,
		WORD RemotePort); 	
	virtual void RecvTrapEvent (LPCSTR Community, LPCSTR Enterprise, LPCSTR NodeDotAddr, SNMP_TRAP SnmpTrap, 
		int SpecialTrap, DWORD TimeStamp, UINT nObjects, LPSTR FAR* ObjectID, 
		LONG FAR* ObjectValue, SNMP_OBJECT_TYPE FAR* ObjectType, LPCSTR RemoteDotAddr, WORD RemotePort);
	virtual void ConnectSnmpEvent (LPCSTR LocalDotAddr, WORD LocalPort, LPCSTR LocalName, 
		WORD MaxByteCnt);
   void CleanupBuffer ();
	CPowerSnmp (HINSTANCE);
	~CPowerSnmp (void);
	//Next two inherited from CPowerUdp
 	// virtual void ExceptionEvent(PT_EXCEPTION ErrorCode, LPCSTR ErrorDesc, DWORD RequestID);
	// virtual void SendEvent (DWORD RequestID);
public:
	BOOL Connect (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR LocalDotAddr, WORD LocalPort, PT_ENV Environment=PT_CPPDLL);
	BOOL SendGetRequest (LPCSTR RemoteHost, WORD RemotePort, LPCSTR Community, DWORD RequestID, 
		UINT nObjects, LPSTR FAR* ObjectID );
	BOOL SendGetNextRequest (LPCSTR RemoteHost, WORD RemotePort, LPCSTR Community, DWORD RequestID, 
		UINT nObjects, LPSTR FAR* ObjectID);
	BOOL SendSetRequest (LPCSTR RemoteHost, WORD RemotePort, LPCSTR Community, DWORD RequestID, 
		UINT nObjects, 
		LPSTR FAR *ObjectID, LONG FAR *ObjectValue, SNMP_OBJECT_TYPE FAR *ObjectType);
	BOOL SendGetResponse (LPCSTR RemoteHost, WORD RemotePort, LPCSTR Community, DWORD RequestID, 
		SNMP_ERROR ErrorStatus, WORD ErrorIndex, UINT nObjects, 
		LPSTR FAR *ObjectID, LONG FAR *ObjectValue, SNMP_OBJECT_TYPE FAR *ObjectType);
	BOOL SendTrap (LPCSTR RemoteHost, WORD RemotePort, LPCSTR Community, LPCSTR Enterprise, 
		LPCSTR NodeDotAddr, SNMP_TRAP GeneralTrap, int SpecialTrap, DWORD TimeStamp, UINT nObjects, 
		LPSTR FAR *ObjectID, LONG FAR *ObjectValue, SNMP_OBJECT_TYPE FAR *ObjectType);
};

#endif // POWERSNM_HPP_DEFINED
