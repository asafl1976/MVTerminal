//
// CPowerFTP
// Defines the functions required to provide FTP protocol processing
//
// Revision History
// .mjb.1	8/27/96 MJB Added Member functions supporting PASV operation
// .mjb.2   7/1/09  Added PreviewCommandEvent() and PreviewReplyEvent()
// .mjb.3   9/25/97 Added PT_ENV support

#ifndef POWERFTP_HPP_DEFINED
#define POWERFTP_HPP_DEFINED
#include "powertcp.hpp"

class CFtpControl;
class CFtpListen;
class CFtpData;

class CPowerFtp
{
friend CFtpControl;
friend CFtpListen;
friend CFtpData;
private:
	// references to Control, Listener, and Data are used so that FTP.HPP
	// does not have to be distributed
	CFtpControl &Control;
protected:
	// protected functions must be overridden by derived classes...
	// informs us of a successful connection and login
	virtual void ConnectEvent (LPCSTR RemoteDotAddr, 
		WORD RemotePort, LPCSTR LocalDotAddr, WORD LocalPort, 
		LPCSTR LocalName);
	// informs us that an exception
	virtual void LogEvent (LPCSTR Message)=0;
	// informs us that file or list data has been received
	virtual void RecvEvent (LPBYTE Data, UINT ByteCnt);
	// informs us that file is being spooled
	virtual void TransferEvent (FTP_COMMAND Command, DWORD BlockCnt, DWORD ByteCnt, LPSTR LocalFileSpec); 
	// informs us that FTP has a reply
	virtual void ReplyEvent (FTP_STATUS Status, FTP_COMMAND LastCommand, int ReplyCode, LPCSTR ReplyStr)=0;
	// informs us that previously submitted data has been sent
	virtual void SendEvent (DWORD Tag);
	// informs us of an exception
	// new for Version PowerTCP 2.0. Now behaves like all other libraries.
	// Either ConnectEvent or ExceptionEvent will be called LoginHost is attempted
	virtual void ExceptionEvent (PT_EXCEPTION Error, LPCSTR ErrorDesc);
	// .mjb.2 allows application to preview Commands sent to FTP server
	virtual void PreviewCommandEvent (LPSTR FAR *pCommand, UINT FAR *pByteCnt);
	// .mjb.2 allows application to preview Commands received from FTP server
	virtual void PreviewReplyEvent (LPSTR FAR *pReply, UINT FAR *pByteCnt);

	CPowerFtp (HINSTANCE);
	~CPowerFtp (void);
	int operator = (CPowerFtp &);
public:
	BOOL LoginHost (LPCSTR OemLicense, PT_FLAGS Flags, LPCSTR RemoteHost, 
		LPCSTR LocalDotAddr, LPCSTR User, LPCSTR Password, LPCSTR Account, PT_ENV Environment=PT_CPPDLL);
	BOOL Close (void);
	BOOL CloseData (void);
	FTP_COMMAND LastCommand(void);
	PT_STATE State(void); // return state of control connection
	
	// Send Data over data connection
	BOOL Send (const LPVOID Data, UINT ByteCnt);
	
	// Send User-built Command over control connection
	BOOL Command (LPCSTR CommandStr);
	
	// ACCESS Control Commands
	BOOL ChDir (LPCSTR NewDir);
	BOOL ChDirUp (void);
	BOOL StructMount (LPCSTR PathName);
	BOOL Reinitialize (void);
	BOOL Logout (void);
	
	// TRANSFER Parameter Commands
	// .mjb.1 Type, Passive no longer needed
	// BOOL Type (LPCSTR Type);
	// BOOL Port (LPCSTR HostPort);
	// BOOL Passive (void);
	BOOL FileStruct (FTP_FILE_STRUCT Structure);
	BOOL Mode (FTP_TRANSFER_MODE Mode);
	
	// FILE Service Commands
	// 10/19/95 added parameters to Retrieve, Store, StoreUnique, Appe, List and NameList:
	//   ListenTimeout - #seconds to wait for data connection from host
	//   Type - specify ASCII, Binary, EBCDIC, Don't change
	// .mjb.1 Set Passive parameter to TRUE to do Passive Transfer
	BOOL Retrieve (LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize, FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout=15);
	BOOL Store (LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize, FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout=15);
	BOOL StoreUnique (LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize, FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout=15);
	BOOL Appe (LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize, FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout=15);
	BOOL Allocate (LONG MaxFileSize, LONG MaxRecordSize);
	// .awp.1
	//BOOL Restart (LPCSTR Marker);
	BOOL Restart (LONG marker);
	BOOL Rename (LPCSTR FromPathName, LPCSTR ToPathName);
	BOOL Abort (void);
	BOOL Delete (LPCSTR PathName);
	BOOL RemoveDir (LPCSTR PathName);
	BOOL MakeDir (LPCSTR PathName);
	BOOL PrintWorkingDir (void);
	// .mjb.1 Set Passive parameter to TRUE to do Passive Transfer
	BOOL List (LPCSTR PathName, BOOL Passive, WORD ListenTimeout=15);
	BOOL NameList (LPCSTR PathName, BOOL Passive, WORD ListenTimeout=15);
	BOOL Site (LPCSTR Desc);
	BOOL System (void);
	BOOL Status (LPCSTR PathName);
	BOOL Help (LPCSTR Topic);
	BOOL Noop (void);
};

#endif // POWERFTP_HPP_DEFINED
