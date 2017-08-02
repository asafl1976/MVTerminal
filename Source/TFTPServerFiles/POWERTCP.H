/************************************************************************

  Module: powertcp.h

  Purpose:
    PowerTCP header file defines C access function for all PowerTCP DLLs
             
************************************************************************

  Written by Dart Communication Application Programming Group.
  Copyright (c) 1994 Dart Communications.  All Rights Reserved.

************************************************************************

  Revision History
  Updated 12/20/95 To use FTP_DATATYPE for FTP transfers
  
  .mjb.1 8/27/96 Remove FTP Type and Passive functions, and replace with parameters
                 for RetrieveFtp, StoreFtp, AppeFtp, StoreUniqueFtp, ListFtp, NameListFtp

  .tve.2 10/23/96  Added typedefs for SENDTELNETBUF and RECVTELNETBUF.  Added prototypes
					for DLL	prototypes for SendBufEvent, RecvBufEvent, and EncryptTelnet.

  .mjb.2 12/17/96 Added PowerVT 3.0 library support

  .tve.4 12/20/96 Added POP3_STATUS value of POP3_HEADER.  Used with POP3 Event signal
					header data.  POP3_MAIL is not only mail body data
  .tve.5 1/2/97	  Added Pop3 HeaderEvent callback which provides parsed header fields for
					TOP and RETRieve commands.  Added POP3_HEADER definition to POP3_STATUS.
					Added HEADEREVENT arguement to ConnectPop3.
	.mjb.6 3/12/97 Vt220 functions changed to Vt functions 
	.mjb.7 5/17/97 Vt ClickEvent callback adds Type parameter
	.tve.8 6/9/97  Added IMAP4 symbols
   .jrg.9 7/9/97   Added MIME symbols
   .jrg.10 7/15/97 Adding IMAP and MIME to the PT_TYPE for Oem License generation.
   .mjb.11 8/12/97 Adding HTTP to PT_TYPE 
   .gbd.8/26/97	   Added HTTP functions and callbacks
   .jrg.12 9/2/97  Added MIME functions and callbacks
   .jrg.13 9/17/97 Added RecvDataType enums.
	.mjb.14 9/23/97 Added PT_ENV type, EnvironmentXXX Functions
   .awp.15 10/15/97 Corrected mistake between 105 and 107 in PT_ENV
   .gbd.13 11/06/97 Removed RecvDataType enums.
   .tve.16 2/25/98  Added IM_STATUS_INFO
   .awp.17	8/10/98	Added true restart support
*/

#ifndef POWERTCP_H_DEFINED
#define POWERTCP_H_DEFINED


/*
 PT_EXCEPTION enummerates all possible error conditions for any channel
 */

typedef enum
{
PT_OK=0,					/* OK...ErrorDesc can be advisory or NULL */
PT_HARDWARE=1,			/* hardware failure */
PT_PROTOCOL=2,			/* protocol software failure */
PT_BADNAME=3,			/* name of host cannot be resolved to address */
PT_CONNREFUSED=4,		/* connection to host refused - remote host is not listening */ 
PT_NOROUTE=5,			/* no route to host (check network part of address) */
PT_NOHOST=6,			/* remote host is not available (check host part of address) */
PT_NOMEM=7,				/* insufficient resources to accept buffer or create channel */
PT_ADDRINUSE=8,		/* address/port in use */
PT_NOTCONNECTED=9,	/* attempt made to use a session that is not PT_CONNECTED */
PT_NORESOURCE=10,		/* TFTP error code 3, or insufficient resources on host */
PT_NOTACCEPTED=11,	/* applicaton did not properly accept passive connection */
PT_SOFTWARE=12,		/* general software error */
PT_REMOTECLOSE=13,	/* remote host reset or closed connection */
PT_WARNING=14,			/* general warning */
PT_ERROR=15,			/* TFTP error code 0, or general error */
PT_NOFILE=16,			/* TFTP error code 1, or specified file not present */
PT_ACCESSVIO=17,		/* TFTP error code 2 */
PT_ILLEGALOP=18,		/* TFTP error code 4 */
PT_UNKNOWNID=19,		/* TFTP error code 5 */
PT_FILEEXISTS=20,		/* TFTP error code 6 */
PT_NOUSER=21,			/* TFTP error code 7 */
PT_NOTCLOSED=22,		/* attempt made to allocate resources when session is already in use */
PT_NOWINSOCK=23,		/* WINSOCK.DLL or WSOCK32.DLL did not load correctly */
PT_BADARGS = 24,		/* Bad function arguements detected */
PT_BLOCK = 25,			/* Blocking interlock error on method call */
PT_BADFILE = 26,		/* 27 - 33 added to match microsquash scodes. */
PT_BADFILENAME = 27,
PT_FILENOTFOUND = 28,
PT_IOERROR = 29,
PT_INVALIDNULL = 30,
PT_FILEFORMAT =  31,
PT_INVALIDPROPINDEX = 32
}	PT_EXCEPTION;

/*
 PT_PROTOCOL enumerates the PowerTCP protocol types
 */
 
typedef enum
{
PT_NULL=0,			/* NULL Protocol */
PT_TCP=1,			/* Transmission Control Protocol */
PT_TNT=2,			/* TELNET */
PT_FTP=3,			/* File Transfer Protocol */
PT_UDP=4,			/* user datagram protocol */
PT_VT=5,				/* VT emulator */
PT_SNMP=6,			/* Simple Network Management Protocol */
PT_TFTP=7,			/* (TFT) Trivial File Transfer Protocol */
PT_SMTP=8, 			/* (SMT) Simple Mail Transfer Protocol */
PT_POP3=9,          /* (pop3) Post Office Protocol */
PT_IMAP=10,			/* (IMAP) Internet Message Access Protocol (.jrg.10) */
PT_MIME=11,			/* (MIME) Multipurpose Internet Mail Extensions (.jrg.10) */
PT_HTTP=12			/* (HTTP) Hypertext Transfer Protocol (.mjb.11) */
}	PT_TYPE;

/*
 PT_ENV enumerates the PowerTCP development environment types
 */

typedef enum
{
PT_VBX=103,  	/* Visual Basic 3.0 */
PT_DELPHI=105,		/* Delphi */
PT_CPPDLL=107,		/* C++ Class library or DLL */     // .awp.15
PT_ACTIVEX=109,		/* ActiveX */
PT_PROFESSIONAL=119,	/* Professional*/
PT_MAILBUILDER=129	/* MailBuilder */
}	PT_ENV;

/*
 PT_STATE enumerates possible states for any session...
 */

typedef enum
{
PT_INVALID=0,			/* invalid or error */
PT_CLOSED=1,			/* closed session (steady state) */
PT_CONNECTING=2,		/* in process of initializing session (transition state) */
PT_CONNECTED=4,		/* session is operating (steady state) */
PT_LISTENING=8,		/* session is listening for a connection (steady state) */
PT_CLOSING=16			/* in process of closing session (transition state) */
}	PT_STATE;

/*
 PT_FLAGS enumerates possible socket options and flags supported by PowerTCP
 */
 
typedef enum
{
PT_NOFLAGS=0,		/* use this to specify no flags */
PT_DEBUG=1,			/* enable all debugging features for session */
PT_REUSEADDR=2,	/* allow the socket to be bound to an address in use */
PT_KEEPALIVE=4,	/* send keepalives (TCP only) */
PT_SHOW=8,			/* show each listener and connection as an icon */
PT_TCPNODELAY=16,	/* enables TCP_NODELAY socket option */
PT_OVERWRITE=32	/* for TFTP server, allow local files to be overwritten */
}	PT_FLAGS;

/*
 FTP_DATATYPE enumerates possible transfer types
 */
 
typedef enum
	{
	FTP_ASCII,
	FTP_EBCDIC,
	FTP_IMAGE,
	FTP_IGNORE   // use FTP_IGNORE if TYPE command is not to be sent
	} FTP_DATATYPE;
	

/*
 FTP_FILE_STRUCT enumerates possible file structures
 */
 
typedef enum
	{
	FTP_FILE,
	FTP_RECORD,
	FTP_PAGE
	} FTP_FILE_STRUCT;
	
/*
 FTP_TRANSFER_MODE enumerates possible file transfer modes
 */
 
typedef enum
	{
	FTP_STREAM,
	FTP_BLOCK,
	FTP_COMPRESSED
	} FTP_TRANSFER_MODE;
	
/*
 FTP_COMMAND enumerates the different FTP commands sent to host
 */
 
typedef enum {
	FTP_CLOSED=0,	/* control connection closed */
	FTP_CLEAR=1,	/* no outstanding request or user used backdoor */
	FTP_USER=2,		/* username sent */
	FTP_PASS=3,		/* password sent */
	FTP_ACCT=4,		/* acct sent */
	FTP_CWD=5,		/* changing working directory */
	FTP_CDUP=6,		/* changing working directory up one level */
	FTP_SMNT=7,		/* do a structured mount */
	FTP_QUIT=8,		/* quit and release all resources or connection closed */
	FTP_REIN=9,		/* reinitialize */
	FTP_PORT=10,	/* set data port */
	FTP_PASV=11,	/* instruct server to go passive */
	FTP_TYPE=12,	/* change type file */
	FTP_STRU=13,	/* specify file structure */
	FTP_MODE=14,	/* specify data transfer mode */
	FTP_RETR=15,	/* receiving a file */
	FTP_STOR=16,	/* sending a file with create option */
	FTP_STOU=17,	/* sending a file with create unique option */
	FTP_APPE=18,	/* sending a file with append (create) option */
	FTP_ALLO=19,	/* allocate storage on host */
	FTP_REST=20,	/* restart file transfer */
	FTP_RNFR=21,	/* rename from */
	FTP_RNTO=22,	/* rename to */
	FTP_ABOR=23,	/* abort control connection */
	FTP_DELE=24,	/* deleting a file */
	FTP_RMD=25,		/* remove dir */
	FTP_MKD=26,		/* make a directory */
	FTP_PWD=27,		/* print working directory */
	FTP_LIST=28,	/* waiting for a directory listing */
	FTP_NLST=29,	/* waiting for a name listing */
	FTP_SITE=30,	/* special site instructions */
	FTP_SYST=31,	/* get type of operating system */  
	FTP_STAT=32,	/* get status info */
	FTP_HELP=33,	/* help request */
	FTP_NOOP=34,	/* noop request */
	
	FTP_END
			}	FTP_COMMAND;
	
/*
 FTP_STATUS enumerates the status of FTP replies from host
 */
 
typedef enum {
	FTP_UNKNOWN=0,		/* spontaneous data or reply from "backdoor" */
	FTP_SUCCESS=1,		/* operation completed sucessfully */
	FTP_ERROR=2,		/* unexpected error */
	FTP_FAILURE=3,		/* failure to complete successfully */
	FTP_WORKING=4		/* informative...wait for next */
			}	FTP_STATUS;

/*
TFTP_OPERATION enumerates the type of TFTP operation currently going on
*/
typedef enum {
	TFTP_CLOSED=0,	/* TFTP Client or Server has closed it's socket */
	TFTP_GET=1,		/* data is being stored into a local file */
	TFTP_PUT=2,		/* data is being written from a local file */
	TFTP_ABORT=3	/* data transfer was aborted by user */
			}	TFTP_OPERATION;
	
/*
 SNMP_OBJECT_TYPE enumerates possible SNMP types
 */
 
typedef enum
	{
	SNMP_INTEGER=0x02,      // all ObjectValue[] members are LONGs unless specified as SNMP_STRING
	SNMP_OCTET_STRING=0x04, // LPSNMP_STRING type
	SNMP_NULL=0x05,
	SNMP_OBJECT_ID=0x06,    // LPSNMP_STRING type
	SNMP_SEQUENCE=0x30,
	SNMP_SET_OF=0x31,
	SNMP_IPADDRESS=0x40,    // LPSNMP_STRING type
	SNMP_COUNTER32=0x41,
	SNMP_GUAGE32=0x42,
	SNMP_TIMETICKS=0x43,
	SNMP_OPAQUE=0x44,       // LPSNMP_STRING type
	SNMP_NSAP=0x45,
	SNMP_COUNTER64=0x46,
	SNMP_UINTEGER32=0x47
	} SNMP_OBJECT_TYPE;

/*
 SNMP_ERROR enumerates possible SNMP general errors
 */
 
typedef enum
	{
	SNMP_NO_ERROR=0, 
	SNMP_TOO_BIG=1, 
	SNMP_NO_SUCH_NAME=2, 
	SNMP_BAD_VALUE=3, 
	SNMP_READ_ONLY=4, 
	SNMP_GEN_ERR=5
	} SNMP_ERROR;

/*
 SNMP_TRAP enumerates possible SNMP traps
 */
 
typedef enum
	{
	SNMP_COLD_START=0, 
	SNMP_WARM_START=1, 
	SNMP_LINK_DOWN=2, 
	SNMP_LINK_UP=3, 
	SNMP_AUTHENTICATION_FAILURE=4, 
	SNMP_EGP_NEIGHBOR_LOSS=5, 
	SNMP_ENTERPRISE_SPECIFIC=6
	} SNMP_TRAP;

/*
 SNMP_TYPE enumerates possible SNMP message types
 */
 
typedef enum
	{
	SNMP_GET_REQUEST=0, 
	SNMP_GET_NEXT_REQUEST=1, 
	SNMP_GET_RESPONSE=2,
	SNMP_SET_REQUEST=3, 
	SNMP_TRAP_MESSAGE=4 
	} SNMP_TYPE;

/*
 SMTP_TYPE enumerates possible SMTP message types
 */
 
typedef enum
	{
	SMTP_MAIL_TYPE=0, 
	SMTP_SEND_TYPE=1, 
	SMTP_SENDORMAIL_TYPE=2,
	SMTP_SENDANDMAIL_TYPE=3 
	} SMTP_TYPE;

/*
 SMTP_COMMAND enumerates possible SNMP message types
 */
 
typedef enum
	{
    SMTP_DATA    		 = 0,
    SMTP_EXPN     		 = 1,
    SMTP_HELO     		 = 2,
    SMTP_HELP     		 = 3,
    SMTP_MAIL     		 = 4,
    SMTP_NOOP     		 = 5,
    SMTP_QUIT     		 = 6,
    SMTP_RCPT     		 = 7,
    SMTP_RSET     		 = 8,
    SMTP_SAML    		 = 9,
    SMTP_SEND    		 = 10,
    SMTP_SOML    		 = 11,
    SMTP_TURN      		 = 12,
    SMTP_VRFY      		 = 13,
    /* the types that follow are used internally in the SMTP library */
    SMTP_CONNECT   		 = 14, /* mts - change IDLE -> CONNECT */
    SMTP_DONE 			 = 15,
    SMTP_SENT_TEXT 		 = 16,
    SMTP_DATA_ATTACH 	 = 17,
    SMTP_DATA_ATTACHEND  = 18,
    SMTP_ATTACH_BEGIN 	 = 19,
    SMTP_ATTACH_SECOND 	 = 20,
    SMTP_SENDCMD  	     = 22
	} SMTP_COMMAND;

/*
 SMTP_STATUS enumerates the status of SMTP replies from host
 */
 
typedef enum {
	SMTP_CLOSED = 0,		/* spontaneous data or reply from "backdoor" */
	SMTP_SUCCESS= 1,		/* unexpected error */
	SMTP_ERROR  = 2,		/* failure to complete successfully */
	SMTP_FAILURE= 3,		/* informative...wait for next */
	SMTP_WORKING= 4
			 }	SMTP_STATUS;

/*
 SMTP_ENCODE_TYPE enumerates the status of SMTP replies from host
 */
 
typedef enum {
	SMTP_MIME64=0,		/* spontaneous data or reply from "backdoor" */
	SMTP_UUENCODE=1,		/* operation completed sucessfully */
			}	SMTP_ENCODE_TYPE;

/*
 POP3_STATUS enumerates the status of SMTP replies from host
 */
 
typedef enum {
	POP3_CLOSED=0,		/* connection to POP3 server closed */
	POP3_REPLY_POS=1,		/* "+OK" reply received from POP3 server */
	POP3_REPLY_NEG=2,		/* "-ERR" reply received from POP3 server */
	POP3_MAIL=3,		/* mail body contained in Reply buffer */ 
	POP3_FILE=4,       /* file attachment contained in Reply buffer */
	POP3_HEADER=5       /* header data in Reply buffer */ //.tve.5
			}	POP3_STATUS;

/*
 POP3_COMMAND enumerates possible SNMP message types
 */ 
typedef enum
	{
    POP3_APOP   	 = 0,
    POP3_DELE      = 1,
    POP3_LIST      = 2,
    POP3_NOOP      = 3,
    POP3_PASS      = 4,
    POP3_QUIT      = 5,
    POP3_RSET      = 6,
    POP3_RETR      = 7,
    POP3_STAT      = 8,
    POP3_TOP       = 9,
    POP3_UIDL      = 10,
    POP3_USER      = 11,
    POP3_CONNECT   = 12
	} POP3_COMMAND;

//.tve.8 begin
/*
   IMAP4 COMMAND Constants IM_CMD
   Enumerates possible IMAP4 commands for the Imap4Event.  
*/
typedef enum {
	IM_CMD_NOCOMMAND = 0,	/* No active command.  The response is unsolicited. */
	IM_CMD_ABORT = 1,		/* Abort function. */
// The following command set implement mailbox specific operations
	IM_CMD_MBXCREATE = 2,	/* Create a mailbox with the given name. */
	IM_CMD_MBXDELETE = 3,	/* Permanently remove the mailbox with the given name. */
	IM_CMD_MBXSELECT = 4,	/* Select a mailbox. */
	IM_CMD_MBXEXAMINE = 5,	/* Select mailbox as read-only. */
	IM_CMD_MBXCLOSE = 6,	/* Delete  \Delete'd messages and deselect mailbox. */
	IM_CMD_MBXCHECK = 7,	/* Allow the server to perform mailbox housekeeping. */
	IM_CMD_MBXEXPUNGE = 8,	/* Delete \Delete'd messages. */
	IM_CMD_MBXSUBSCRIBE = 9,	/* Add mailbox name to subscribed list. */
	IM_CMD_MBXUNSUBSCRIBE = 10,	/* Remove mailbox subscription. */
	IM_CMD_MBXAPPEND = 11,	/* MbxAppend function. */
	IM_CMD_MBXCOMMAND = 12,	/* MbxCommand function. */
	IM_CMD_MBXLIST = 13,	/* MbxList function - List mode. */
	IM_CMD_MBXLISTSUB = 14,	/* MbxList function. - Subscribe mode. */
	IM_CMD_MBXRENAME = 15,	/* MbxRename function. */
	IM_CMD_MBXSTATUS = 16,	/* MbxStatus function. */
//The following command set implement message specific operations
	IM_CMD_MSGCOPY = 17,	/* MsgCopy function. */
	IM_CMD_MSGFETCH = 18,	/* MsgFetch function. */
	IM_CMD_MSGSEARCH = 19,	/* MsgSearch function. */
	IM_CMD_MSGSTORE = 20,	/* MsgStore function. */
//The following command set implement server specific operations
	IM_CMD_SVRLOGIN = 21,	/* SvrLogin function. */
	IM_CMD_SVRLOGINSECURE = 22,  /* SvrLoginSecure function. */
	IM_CMD_SVRCOMMAND = 24,	/* SvrCommand function. */
	IM_CMD_SVRLOGOUT = 25,	/* SvrLogout function. */
	IM_CMD_SVRNOOP = 26,	/* Implements the server NOOP command */
	IM_CMD_SVRCAPABILITY = 27,	/* SvrCapability function. */
	IM_CMD_SVRXTENDED = 28	/* Issues user supplied command */
} IM_CMD;

/*
	IMAP4 mailbox access rights; r/o, r/w or 'not reported'
*/
typedef enum {
	IM_ACCESS_NOTREPORTED = 0,
	IM_ACCESS_READONLY = 1,
	IM_ACCESS_READWRITE = 2
} IM_ACCESS;

/*
  IMAP4 Mailbox Command Constants IM_MBXCMD
  Enumerates possible IMAP4 subcommands for the MbxCommand function.
*/
typedef enum {
	IM_MBXCMD_CREATE = 2,	/* Create a mailbox with the given name. */
	IM_MBXCMD_DELETE = 3,	/* Permanently remove the mailbox with the given name. */
	IM_MBXCMD_SELECT = 4,	/* Select a mailbox for access. */
	IM_MBXCMD_EXAMINE = 5,	/* Select a mailbox for access read-only. */
	IM_MBXCMD_CLOSE = 6,	/* Delete  \Delete'd messages and deselect mailbox. */
	IM_MBXCMD_CHECK = 7,	/* Allow the server to perform mailbox housekeeping. */
	IM_MBXCMD_EXPUNGE = 8,	/* Delete \Delete'd messages. */
	IM_MBXCMD_SUBSCRIBE = 9,	/* Add mailbox name to subscribed list. */
	IM_MBXCMD_UNSUBSCRIBE = 10	/* Remove mailbox subscription. */
} IM_MBXCMD;
 /* 
  IMAP4 Server Commands Constants  IM_SVRCMD
  These command codes are used with the SvrCommand function call for server control. 
*/
typedef enum {
	IM_SVRCMD_NOOP = 26,		/* Implements the server NOOP command */
	IM_SVRCMD_CAPABILITY = 27,	/* Implements the server CAPABILITY command. */
	IM_SVRCMD_XTENDED = 28		/* Issues a user supplied command */
} IM_SVRCMD;

/*
  IMAP4 Mailbox Flag Attribute Constants   IM_MBXATT
  IMAP4 Mailbox Attributes returned via the ListEvent. 
*/
#define IM_MBXATT int
#define IM_MBXATT_NOINFERIORS 1/* No child levels exist under this level. */
#define IM_MBXATT_NOSELECT 2	/* The mailbox is not selectable. */
#define IM_MBXATT_MARKED 4	/* The mailbox is marked "interesting" by server. */
#define IM_MBXATT_UNMARKED 8	/* The mailbox contains no new messages */

/*
  IMAP4 Mailbox Status Item IM_MBXSTAT
  Mailbox Status Item identifiers requested using MbxStatus. 
*/
#define IM_MBXSTAT int
#define IM_MBXSTAT_MESSAGES 1	/* Number of messages in the mailbox */
#define IM_MBXSTAT_RECENT 2	/* Number of 'recent' messages in mailbox  */
#define IM_MBXSTAT_UNSEEN 4	/* Number of 'unseen' messages . */
#define IM_MBXSTAT_UIDNEXT 8	/* Value of the next valid UID for the mailbox */
#define IM_MBXSTAT_UIDVALIDITY 16	/* Requests the availability of UID references. */
#define IM_MBXSTAT_ALL 31		/* Requests all possible status items */

/*
  IMAP4 Message Store Mode Constants IM_MSGSTOR
  Command Codes which described the modes of the MsgStore command. 
*/
typedef enum {
	IM_MSGSTOR_FLAGS = 0,		/* Replace the flags for the message with the argument. */
	IM_MSGSTOR_FLAGS_SILENT = 1,/* See FLAGS, but without returning a new value. */
	IM_MSGSTOR_ADDFLAGS = 2,		/* Add the argument to the flags for the message. */
	IM_MSGSTOR_ADDFLAGS_SILENT = 3,/* See +FLAGS, but without returning a new value. */
	IM_MSGSTOR_SUBFLAGS = 4,		/* Remove the argument from the flags for the message. */
	IM_MSGSTOR_SUBFLAGS_SILENT = 5/* See -FLAGS, but without returning a new value. */
} IM_MSGSTOR;

/* IMAP4 Security Service Constants
	used by SvrLoginSecure()
*/
typedef enum {
	IM_SECSRVS_NONE = 0,			/* No Security Services specified. */
	IM_SECSRVS_KERBEROS = 1,		/* Kerboeros authorization security */
	IM_SECSRVS_GSSAPI = 2,		/* General Session Security */
	IM_SECSRVS_SKEY = 3			/* S/Key session Security */
} IM_SECSRVS;

/*
	Fetch item message bit definitions
*/
#define IM_MSGITEM_BODY  0x0001L	 //	Requests fetch of the message item BODY.
#define IM_MSGITEM_PEEK  0x0002L	 //		Requests fetch of the message BODY.PEEK.
#define IM_MSGITEM_STRUCTURE  0x0004L	 //		Requests fetch of the message BODYSTRUCTURE.
#define IM_MSGITEM_ENVELOPE  0x0008L	 //		Requests fetch of the message ENVELOPE.
#define IM_MSGITEM_FLAGS  0x0010L	 //		Requests fetch of the message FLAGS.
#define IM_MSGITEM_DATE  0x0020L	 //		Requests fetch of the message INTERNALDATE.
#define IM_MSGITEM_RFC822  0x0040L	 //		Requests fetch of the message RFC822.
#define IM_MSGITEM_HEADER  0x0080L	 //		Requests fetch of the message RFC822.HEADER
#define IM_MSGITEM_SIZE  0x0100L	 //		Requests fetch of the message RFC822.SIZE
#define IM_MSGITEM_TEXT  0x0200L	 //		Requests fetch of the message RFC822.TEXT
#define IM_MSGITEM_UID  0x0400L		 //		Requests fetch of the message UID


/*
	Mailbox and message (perminent) flags
*/
#define IM_FLAG_FLAGGED  1	//Bit 0 defines the \Flagged flag
#define IM_FLAG_SEEN  2		//Bit 1 defines the \Seen flag
#define IM_FLAG_ANSWERED  4	//Bit 2 defines the \Answered flag
#define IM_FLAG_DELETED  8	//Bit 3 defines the \Deleted flag
#define IM_FLAG_DRAFT  16	//Bit 4 defines the \Draft flag

/*
  IMAP4 Security Protection Constants 
	used by SvrLoginSecure()
*/
typedef enum {
	IM_SECPROT_NONE = 0,		
	IM_SECPROT_INTEGRITY = 1,		
	IM_SECPROT_PRIVACY = 2		
} IM_SECPROT;
  
/*
  IMAP4 control Protocol Constants
    used branch all methods in the MbImapCtl.cpp code
*/
typedef enum {
	IM_PROTOCOL_IMAP4 = 0,
	IM_PROTOCOL_POP3 = 1
} IM_PROTOCOL;

/*
  IMAP4 STATUS Constants IM_STATUS
  Enumerates possible POP3 status codes.
*/
//typedef enum {
//	IM_STATUS_CLOSED = 0,	
//	IM_STATUS_REPLY_OK = 1,	
//	IM_STATUS_REPLY_NO = 2,	
//	IM_STATUS_REPLY_BAD = 3,	
//	IM_STATUS_PREAUTH = 4,	
//	IM_STATUS_BYE = 5,		
//	IM_STATUS_AUTHORIZED = 6,	/* The LOGIN exchange completed. */
//	IM_STATUS_MAIL = 7,		/* The RecvData contains all or part of a mail message */
//	IM_STATUS_HEADER = 8,	/* A complete mail header has been received */
//	IM_STATUS_CAPABILITY = 9,
//	IM_STATUS_LIST = 10,	/* Returns subscribed mailbox names that match. */
//	IM_STATUS_LSUB = 11,	/* Returns subscribed mailbox names that match */
//	IM_STATUS_SEARCH = 12,	/* Returns messages that match the search. */
//	IM_STATUS_FLAGS = 13,	/* Returns flags for this mailbox. */
//	IM_STATUS_EXISTS = 14,	/* The number of messages in the mailbox. */
//	IM_STATUS_RECENT = 15,	/* The number of recent messages in the mailbox. */
//	IM_STATUS_EXPUNGE = 16,	/* The message marked for delete are removed. */
///	IM_STATUS_STATUS = 17,	/* Resonse of the mailbox status command. */
//	IM_STATUS_FETCH = 18,	/* The server response was in error. */
//	IM_STATUS_APPEND = 19,	/* Append command completed. */
//	IM_STATUS_ERROR = 20,	/* The server response was in error. */
//	IM_STATUS_INFO = 21		/* Supplemental informat has been provided */
//} IM_STATUS;
//.tve.8 end 

typedef enum {
	IM_STATUS_OK				 =	0x0000	,/* A IMAP4 server has responded with "OK"  */
	IM_STATUS_NO				 =	0x0001	,/* A IMAP4 server has responded with "NO",. */

	IM_STATUS_BAD				 =	0x0102	,/* A IMAP4 server has responded with "BAD" */
	IM_STATUS_PREAUTH			 =	0x0104	,/* The Username is pre-authorized for access. */
	IM_STATUS_BYE				 =	0x0106	,/* A IMAP4 server has responded with "BYE" */

	IM_STATUS_SVR 				 =	0x0200	,
	IM_STATUS_SVR_CAPABILITY	 =	0x0202	, /*RecvData contains capability list. */
	IM_STATUS_SVR_CONNECTED		 =	0x0204	,
	IM_STATUS_SVR_DISCONNECTED	 =	0x0206	,/* The connection to a IMAP4 server is closed. */

	IM_STATUS_MBX				 =	0x0300	,
	IM_STATUS_MBX_EXISTS		 =	0x0302	,
	IM_STATUS_MBX_RECENT		 =	0x0304	,
	IM_STATUS_MBX_FLAGS			 =	0x0306	,
	IM_STATUS_MBX_PERMANENTFLAGS =	0x0308	,
	IM_STATUS_MBX_UNSEEN		 =	0x030A	,
	IM_STATUS_MBX_UID			 =	0x030C	,
	IM_STATUS_MBX_ACCESS		 =	0x030E	,

	IM_STATUS_FETCH					 =	0x0400	,
	IM_STATUS_FETCH_BODY			 =	0x0402	,
	IM_STATUS_FETCH_BODYSTRUCTURE	 =	0x0404	,
	IM_STATUS_FETCH_BODYPEEK		 =	0x0406	,
	IM_STATUS_FETCH_FLAGS			 =	0x0408	,
	IM_STATUS_FETCH_INTERNALDATE	 =	0x040A	,
	IM_STATUS_FETCH_ENVELOPE		 =	0x040C	,
	IM_STATUS_FETCH_UID				 =	0x040E	,
	IM_STATUS_FETCH_RFC822			 =	0x0410	,
	IM_STATUS_FETCH_RFC822HEADER	 =	0x0412	,
	IM_STATUS_FETCH_RFC822TEXT		 =	0x0414	,
	IM_STATUS_FETCH_RFC822SIZE		 =	0x0416	,

	IM_STATUS_INFO			 =	0x0500	,
	IM_STATUS_INFO_ALERT	 =	0x0502	,
	IM_STATUS_INFO_NEWNAME	 =	0x0504	,
	IM_STATUS_INFO_PARSE	 =	0x0506	,
	IM_STATUS_INFO_TRYCREATE =	0x0508	,
	IM_STATUS_INFO_EXPUNGED	 =	0x050C	,
	IM_STATUS_INFO_UNKNOWN	 =	0x050A	,
	IM_STATUS_INFO_COMMENTS	 =	0x050E	,

	IM_STATUS_SEARCH		 =	0x0600	,
	IM_STATUS_LISTALL		 =	0x0702	,
	IM_STATUS_LISTSUBSCRIBED =	0x0704	,
	IM_STATUS_APPEND		 =	0x0800  ,

	IM_STATUS_XTENDCOMMAND	= 0x900, 

// Internal status values
	IM_STATUS_ERROR			 =	0x1000  ,
	IM_STATUS_STATUS		 =	0x1002  ,	
	IM_STATUS_AUTHORIZED	 =  0x1006	,
	IM_STATUS_SIZE			 =  0x1008
} IM_STATUS;

/*
HTTP_FIELD enumerates the header field types used by HTTP 1.0 and 1.1
*/
typedef enum {
	HTTP_CLEAR_HEADERS,				/* used to reset HeaderLines */
	HTTP_ALL_HEADERS,				/* used to return all HeaderLines */
	HTTP_CACHE_CONTROL,				/* General-Header 1.1 only */
	HTTP_CONNECTION,				/* General-Header 1.1 only */
	HTTP_DATE,						/* General-Header 1.0 and 1.1 */
	HTTP_PRAGMA,					/* General-Header 1.0 and 1.1 */
	HTTP_TRANSFER_ENCODING,			/* General-Header 1.1 only */
	HTTP_UPGRADE,					/* General-Header 1.1 only */
	HTTP_VIA,						/* General-Header 1.1 only */
	HTTP_ACCEPT,					/* Request-Header 1.1 only */
	HTTP_ACCEPT_CHARSET,			/* Request-Header 1.1 only */
	HTTP_ACCEPT_ENCODING,			/* Request-Header 1.1 only */
	HTTP_ACCEPT_LANGUAGE,			/* Request-Header 1.1 only */
	HTTP_AUTHORIZATION,				/* Request-Header 1.0 and 1.1 */
	HTTP_FROM,						/* Request-Header 1.0 and 1.1 */
	HTTP_HOST,						/* Request-Header 1.1 only */
	HTTP_IF_MODIFIED_SINCE,			/* Request-Header 1.0 and 1.1 */
	HTTP_IF_MATCH,					/* Request-Header 1.1 only */
	HTTP_IF_NONE_MATCH,				/* Request-Header 1.1 only */
	HTTP_IF_RANGE,					/* Request-Header 1.1 only */
	HTTP_UNMODIFIED_SINCE,			/* Request-Header 1.1 only */
	HTTP_MAX_FORWARDS,				/* Request-Header 1.1 only */
	HTTP_PROXY_AUTHORIZATION,		/* Request-Header 1.1 only */
	HTTP_RANGE,						/* Request-Header 1.1 only */
	HTTP_REFERER,					/* Request-Header 1.0 and 1.1 */
	HTTP_USER_AGENT,				/* Request-Header 1.0 and 1.1 */
	HTTP_AGE,						/* Response-Header 1.1 only */
	HTTP_LOCATION,					/* Response-Header 1.0 and 1.1 */
	HTTP_PROXY_AUTHENTICATE,		/* Response-Header 1.1 only */
	HTTP_PUBLIC,					/* Response-Header 1.1 only */
	HTTP_RETRY_AFTER,				/* Response-Header 1.1 only */
	HTTP_SERVER,					/* Response-Header 1.0 and 1.1 */
	HTTP_VARY,						/* Response-Header 1.1 only */
	HTTP_WARNING,					/* Response-Header 1.1 only */
	HTTP_WWW_AUTHENTICATE,			/* Response-Header 1.0 and 1.1 */
	HTTP_ALLOW,						/* Entity-Header 1.0 and 1.1 */
	HTTP_CONTENT_BASE,				/* Entity-Header 1.1 only */
	HTTP_CONTENT_ENCODING,			/* Entity-Header 1.0 and 1.1 */
	HTTP_CONTENT_LANGUAGE,			/* Entity-Header 1.1 only */
	HTTP_CONTENT_LENGTH,			/* Entity-Header 1.0 and 1.1 */	
	HTTP_CONTENT_LOCATION,			/* Entity-Header 1.1 only */
	HTTP_CONTENT_MD5,				/* Entity-Header 1.1 only */
	HTTP_CONTENT_RANGE,				/* Entity-Header 1.1 only */
	HTTP_CONTENT_TYPE,				/* Entity-Header 1.0 and 1.1 */
	HTTP_ETAG,						/* Entity-Header 1.1 only */
	HTTP_EXPIRES,					/* Entity-Header 1.0 and 1.1 */
	HTTP_LAST_MODIFIED,				/* Entity-Header 1.0 and 1.1 */
	HTTP_EXTENSION_HEADER			/* Entity-Header 1.0 and 1.1 */
			}	HTTP_FIELD;

/*
HTTP_METHOD enumerates the HTTP method to be performed on the URI
*/
typedef enum {
	HTTP_OPTIONS,				/* 1.1 ONLY */
	HTTP_GET,					/* 1.0 AND 1.1 */
	HTTP_HEAD,					/* 1.0 AND 1.1 */
	HTTP_POST,                  /* 1.0 AND 1.1 */
	HTTP_PUT,                   /* 1.1 ONLY */
	HTTP_DELETE,                /* 1.1 ONLY */
	HTTP_TRACE,                 /* 1.1 ONLY */
	HTTP_EXTENSION_METHOD       /* 1.0 AND 1.1 */
			} HTTP_METHOD;

/*
HTTP_VERSION enumerates the HTTP version levels
*/
typedef enum {
	HTTP_1_0,					/* 1.1 ONLY */
	HTTP_1_1					/* 1.0 AND 1.1 */
			} HTTP_VER;

/*
HTTP_SECTION enumerates the HTTP message parts
*/
typedef enum {
	HTTP_START_LINE,
	HTTP_HEADERS,
	HTTP_CONTENT
			} HTTP_SECTION;

/*
 PT_ATTRIBUTE enumerates possible VT220 attributes
 */
 
typedef enum
	{
	PT_ERR=0,
	PT_TTY=1, // set to no emulation
	PT_52=2, // set to VT52
	PT_100=3, // set to VT100
	PT_220_7=4, // set to VT220 seven bit mode
	PT_220_8=5,	// set to VT220 eight bit mode
	PT_AUTOPRINT=6,
	PT_AUTOREPEAT=7,
	PT_AUTOWRAP=8,
	PT_BELL=9,
	PT_BUFFERROWS=10,
	PT_CLEAR=11,
	PT_COLORBACK=12, // use COLORREF
	PT_COLORBOLD=13, // use COLORREF
	PT_COLORFORE=14, // use COLORREF
	PT_COLS=15,	// either 80 or 132
	PT_COLWIDTH=16,
	PT_CURSOR=17,
	PT_CURSORCOL=18,
	PT_CURSORKEYS=19,	// set for application cursor keys
	PT_CURSORROW=20,
	PT_CURSORSTYLE=21, // set for underline, reset for block
	PT_ENABLED=22,
	PT_INVERSE=23, // invert foreground/background
	PT_KEYPAD=24,	// set for application keypad
	PT_NEWLINE=25,	// set to generate CR/LF pair
	PT_OEMLICENSE=26,
	PT_ONLINE=27,	// if set, you can turn off input if you like
	PT_PRINTCURSORLINE=28, // print line cursor is on
	PT_PRINTERCONTROLLER=29,
	PT_PRINTPASSTHROUGH=30,
	PT_PRINTSCREEN=31,
	PT_RESET=32, // reset the terminal
	PT_ROWS=33,
	PT_ROWHEIGHT=34, // read only
	PT_SCROLL=35,	// set to use smooth scroll
	PT_SELLENGTH=36,
	PT_SELSTART=37,
	PT_ANSWERBACK=38, // string for Answerback (5) reply 16 Aug 95
	PT_ENABLENEWLINE=39, // enable NewLine virtual callback
	PT_TABS=40,	// comma delimited string of tab position (1-relative)
	PT_CHARSET=41, // ASCII, UK, Dutch, Finnish, etc...
	PT_BACKSPACE=42,  // use 0 for ASCII 127 (DEL), 1 for ASCII 8 (BS)
// .mjb.2 Added for PowerVT version 3.0
	PT_ENABLESTATUSLINE=43, // 0 for none, 1 for indicator, 2 for host-writable
	PT_STATUSLINE=44, // NULL terminated string of VT320 emulation characters
	PT_320_7=45,
	PT_320_8=46,
	PT_ENVIRONMENT=47 // set appropriate environment
	} PT_ATTRIBUTE;

/*
 VT_CHARSET enumerates possible VT220 Multinational character sets
 */
 
typedef enum
	{
	CS_ASCII,
	CS_BRITISH,
	CS_DUTCH,     
	CS_FINNISH,
	CS_FRENCH,
	CS_FRENCH_CANADIAN,
	CS_GERMAN,
	CS_ITALIAN,
	CS_NORWEGIAN_DANISH,
	CS_PORTUGUESE,
	CS_SPANISH,
	CS_SWEDISH,
	CS_SWISS
	} VT_CHARSET;

//jrg.9 MIME symbols
/*
MI_STATUS enumerates possible MIME status types for DataEvent
*/
typedef enum
{
	MI_STATUS_COMPLETED = 0,
	MI_STATUS_LABELS = 1,
	MI_STATUS_822HDR = 2,
	MI_STATUS_IMAP4HDR = 3,
	MI_STATUS_MAIL = 4,
	MI_STATUS_FILE= 5
}
MI_STATUS;

/*
   PowerTCP typedefs...
*/

DECLARE_HANDLE (HPOWERTCP);   /* handle */

typedef void (CALLBACK * ACCEPTEVENT)(
	HPOWERTCP hListener,	/* identifies listener of incoming connections */ 
	DWORD     UserData   /* UserData from Listen() function */
	);
typedef void (CALLBACK * CMDEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	BYTE      Cmd,       /* TELNET command (WILL, WONT, DO, DONT...) */ 
	BYTE      Option,    /* TELNET option number */ 
	LPBYTE    SubOption, /* pointer to sub-option string */ 
	UINT    ByteCnt    /* byte count for SubOption */
	);
typedef void (CALLBACK * CONNECTEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    RemoteDotAddr, /* remote host connected to in dot notation */
	WORD      RemotePort,/* remote port connected to */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName  /* name of default local host */
	);
typedef void (CALLBACK * CONNECTSMTPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    RemoteDotAddr, /* remote host connected to in dot notation */
	WORD      RemotePort,/* remote port connected to */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName  /* name of default local host */
	);
typedef void (CALLBACK * CONNECTPOP3EVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    RemoteDotAddr, /* remote host connected to in dot notation */
	WORD      RemotePort,/* remote port connected to */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName  /* name of default local host */
	);
typedef void (CALLBACK * CONNECTUDPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName, /* name of default local host */
	WORD      MaxByteCnt /* largest datagram size supported */
	);
typedef void (CALLBACK * CONNECTSNMPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName, /* name of default local host */
	WORD      MaxByteCnt /* largest datagram size supported */
	);
typedef void (CALLBACK * EXCEPTIONEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	PT_EXCEPTION ErrorCode,  /* spontaneous exception notification */
	LPCSTR    ErrorDesc  /* description of error */
	);
typedef void (CALLBACK * FILEEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,   /* UserData from Listen() function */
	LPCSTR 	  Filespec,
	char 	  Mode,
	DWORD      FileSize
	);
typedef void (CALLBACK * LISTENEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    LocalDotAddr, /* local host address in dot notation */
	WORD      LocalPort, /* local port */
	LPCSTR    LocalName  /* name of default local host */
	);
typedef void (CALLBACK * LOGEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    Message    /* Message log */
	);
typedef void (CALLBACK * RECVEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPBYTE    Data,  		/* data from remote source */ 
	UINT      ByteCnt    /* byte count for Data */
	);
//.tve.01  Added Telnet callback on Send  // .tve.01 Insert Begin...
typedef void (CALLBACK * RECVTELNETBUF)(
	HPOWERTCP hSession,	 /* identifies session notifying our app */ 
	LPBYTE    Data,  	 /* data from remote source for TELNET */ 
	UINT      ByteCnt    /* byte count for Data */
	);
typedef void (CALLBACK * SENDTELNETBUF)(
	HPOWERTCP hSession,	 /* identifies session notifying our app */ 
	LPBYTE    Data,      /* data from TELNET for TCP layer */
	UINT      ByteCnt    /* byte count for Data */
	);
//.tve.01  Insert Ends...
typedef void (CALLBACK * RECVSNMPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    Community, /* SNMP community */
	DWORD     RequestID,
	SNMP_ERROR SnmpError,
	int       ErrorIndex,
	SNMP_TYPE MessageType,
	UINT    nObjects,
	LPSTR FAR *ObjectID,
	LONG FAR *ObjectValue,
	SNMP_OBJECT_TYPE FAR *ObjectType,
	LPCSTR    RemoteDotAddr, /* source address of datagram */
	WORD      RemotePort /* source port of datagram */
	);
typedef void (CALLBACK * RECVTRAPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    Community, /*  */
	LPCSTR    Enterprise, /*  */
	LPCSTR NodeDotAddr,
	SNMP_TRAP SnmpTrap,
	int       SpecialTrap,
	DWORD     TimeStamp,
	UINT    nObjects,
	LPSTR FAR *ObjectID,
	LONG FAR *ObjectValue,
	SNMP_OBJECT_TYPE FAR *ObjectType,
	LPCSTR    RemoteDotAddr, /* source address of datagram */
	WORD      RemotePort /* source port of datagram */
	);
typedef void (CALLBACK * TFTPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */  
	TFTP_OPERATION Op, 
	LPCSTR    LocalFileSpec, /* local file data was dumped to, if any */
	WORD      Block,     /* Block number of TFTP packet */
	UINT      ByteCnt,    /* byte count for Data */
   DWORD     TransferID,
	WORD 	    ActiveCnt,
	LPCSTR    RemoteDotAddr, /* source or destination address of datagram */
	WORD      RemotePort, /* source or destination port of datagram */
	PT_EXCEPTION ErrorCode,
	LPCSTR    ErrorDesc
	);
typedef void (CALLBACK * RECVUDPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPBYTE    Data,  		/* data from remote source */ 
	UINT      ByteCnt,   /* byte count for RecvData */
	LPCSTR    RemoteDotAddr, /* source address of datagram */
	WORD      RemotePort /* source port of datagram */
	);
typedef void (CALLBACK * QUERYEVENT)(	// for both TCP and UDP	
	DWORD     UserData,  /* UserData from Connect() function */
	WORD Version,		/* version of Windows Sockets specification in use */
	LPSTR Description,	/* description of Winsock implementation w/ vender info */
	LPSTR SystemStatus, /* status or configuration information */
	WORD MaxSockets,	/* max number of sockets app can potentially open */
	WORD MaxUdpDg,		/* size of largest UDP datagram in bytes */
	LPSTR LocalName,	/* name of local host */
	LPSTR FAR * LocalDotAddr /* ptr to array of strings identifying available hosts */
	); 
typedef void (CALLBACK * REPLYEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	FTP_STATUS Status,   /* FTP completion status */
	FTP_COMMAND LastCommand, /* last command sent */
	int ReplyCode,       /* 3-digit FTP reply code */
	LPCSTR ReplyStr      /* Reply string */
	);
typedef void (CALLBACK * SENDEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	DWORD     DataTag    /* DataTag from Send() functions */
	);
typedef void (CALLBACK * TRANSFEREVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	FTP_COMMAND Command, /* FTP_APPE, FTP_STOR, FTP_STOU, FTP_RETR */ 
	DWORD     BlockCnt,  /* starts at 1 and increments for each transfer, 0 indicates close */
	DWORD     ByteCnt,   /* number of bytes transfered so far */
	LPCSTR    LocalFileSpec /* fully specified local file involved in transfer */
	);
typedef void (CALLBACK * HOSTCOMMANDEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	PT_ATTRIBUTE Attr,   /* attribute being changed */
	DWORD Value,         /* value can be boolean  */
	BOOL FAR *Ignore		/* you can set to make the command a noop */
	);
typedef void (CALLBACK * KEYDOWNEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	UINT FAR *KeyCode,   /* virtual key code */
	UINT FAR *Shift,     /* shift and control key state */
	BOOL Extended   		/* TRUE for extended keys */
	);
typedef void (CALLBACK * KEYPRESSEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPBYTE Data,   		/* character(s) generated by emulator */
	UINT ByteCnt         /* byte count of data */
	);
typedef void (CALLBACK * CLICKEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	WORD xPos,   			/* x position of cursor */
	WORD yPos,	         /* y position of cursor */
	WORD cType,				/* .mjb.7 
			0 - left down 
			1 - left up
			2 - right down
			3 - right up
			4 - left double click
			5 - right double click */
	WORD fwKeys				/* .mjb.7
		Combination of:
			MK_LBUTTON          1
			MK_RBUTTON          2
			MK_SHIFT            4
			MK_CONTROL          8
			MK_MBUTTON          16   */
	);
typedef void (CALLBACK * SMTPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
    SMTP_STATUS Status,
    SMTP_COMMAND LastCommand,
    int ReplyCode,
    LPCSTR ReplyStr,
    BOOL Complete
    );
typedef void (CALLBACK * LOGSMTPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    Command    /* DataTag from Send() functions */
	);
typedef void (CALLBACK * ATTACHSMTPEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
    LPCSTR    FileSpec,
	DWORD     DataTag    /* DataTag from Send() functions */
	);
typedef void (CALLBACK * LOGPOP3EVENT)(
	HPOWERTCP hSession,	 /* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR    Command    /* Message log */
	);
typedef void (CALLBACK * POP3EVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,	/* UserData from Connect() function */
	POP3_STATUS Status,	/* Status of progress */  		 
	POP3_COMMAND LastCommand, /* Last POP3 command sent */
	LPBYTE    Reply,        /* reply from POP3 server */
	UINT      ByteCnt,		/* byte count for reply */
	LPCSTR    FileSpec,     /* fully specified name for Attachment or mail */
	char      Mode,         /* B (MIME), U (UUENCODE) or N (NONE) */
	WORD      PercentComplete /* completion indicator 0-100 */   
	);
//.tve.5 added typedef
typedef void (CALLBACK * HEADEREVENT) (
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,	/* UserData from Connect() function */
	POP3_COMMAND LastCommand, /* Last POP3 command sent */
	LPCSTR lpReturnPath, /* Null Termimated Return Path Field */
	LPCSTR lpReceived,	/* Null Termimated Received Field */
	LPCSTR lpDate,		/* Null Termimated Date Field */
	LPCSTR lpFrom,		/* Null Termimated From Field */
	LPCSTR lpSubject, 	/* Null Termimated Subject Field */
	LPCSTR lpSender, 	/* Null Termimated Sender Field */
	LPCSTR lpTo,		/* Null Termimated To Field */
	LPCSTR lpCc			/* Null Termimated Cc Field */
	);

//.jrg.12  MIME Callbacks
typedef void (CALLBACK * DATAEVENT) (
	HPOWERTCP hSession,	/* identifies session notifying our app */
	DWORD	  UserData,	/* UserData from Decode()/Encode() function */
	MI_STATUS Status,	/* Status of state of MIME decode/encode process */
	LPBYTE    MimeData, /* Raw data currently being decoded */
	UINT	  MimeCnt,	/* Number of bytes in MimeData */
	LPCSTR	  FileSpec	/* Specification of file currently being written */
	);

typedef void (CALLBACK * MIMEHEADEREVENT) (
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,	/* UserData from Decode()/Encode() function */
	LPCSTR lpReturnPath, /* Null Termimated Return Path Field */
	LPCSTR lpReceived,	/* Null Termimated Received Field */
	LPCSTR lpDate,		/* Null Termimated Date Field */
	LPCSTR lpFrom,		/* Null Termimated From Field */
	LPCSTR lpSubject, 	/* Null Termimated Subject Field */
	LPCSTR lpSender, 	/* Null Termimated Sender Field */
	LPCSTR lpTo,		/* Null Termimated To Field */
	LPCSTR lpCc,		/* Null Termimated Cc Field */
	LPCSTR lpBcc,		/* Null Termimated Bcc Field */
	LPCSTR lpMsgId,		/* Null Termimated MsgId Field */
	LPCSTR lpInReplyTo,	/* Null Termimated InReplyTo Field */
	LPCSTR lpNewsGroups,/* Null Termimated NewsGroups Field (NNTP only) */
	LPCSTR lpRelayVersion, /* Null Termimated RelayVersion Field (NNTP only) */
	LPCSTR lpPostingVersion, /* Null Termimated PostingVersion Field (NNTP only) */
	LPCSTR lpPath		/* Null Termimated Path Field (NNTP only) */
	);

typedef void (CALLBACK * MIMEFILEEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,   /* UserData from Listen() function */
	LPCSTR 	  Filespec,
	LPCSTR 	  Mode,
	DWORD     FileSize
	);

typedef void (CALLBACK * LABELEVENT) (
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,	/* UserData from Decode()/Encode() function */
	LPCSTR	  SectionSpecifier,		/* Part specifier of MIME structure */	
	LPCSTR	  ContentType,			/* Content type header field */
	LPCSTR	  ContentSubType,		/* Content type sub-type header field */
	LPCSTR	  ContentParameters,	/* Content Parameter data for the Content type field */
	LPCSTR	  ContentTransferEncoding,  /* Specifies the encoding mechanism for the body part */
	LPCSTR    ContentDisposition,		/* Identifies the disposition fo the bodypart content */
	LPCSTR    DispositionParameters,	/* Additional parameters for content disposition */
	LPCSTR	  ContentID,				/* Optional identifier for the body part */
	LPCSTR	  ContentDescription,		/* Optional body part description */
	LPCSTR	  OtherLabels				/* Additional label and parameter information */
	);

/* use structure to reference SNMP ObjectValue Types */
/* Cnt is number of bytes in Data array...LPBYTE is used instead of array for memory model independence */
typedef struct {
	WORD Cnt;
	BYTE Data[1]; } SNMP_STRING;
	
typedef SNMP_STRING FAR * LPSNMP_STRING;

#ifdef __cplusplus
extern "C" {
#endif

// gbd added HTTP typedefs
typedef void (CALLBACK * STATUSEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPCSTR Version, 
	LPCSTR StatusCode, 
	LPCSTR ReasonPhrase 
	);

typedef void (CALLBACK * HEADERLINEEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	HTTP_FIELD FieldType, 
	LPCSTR FieldValue, 
	LPCSTR HeaderLine	
	);

typedef void (CALLBACK * CONTENTEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPBYTE	  Content, 
	UINT	  ContentCnt, 
	UINT	  CumContentCnt, 
	UINT	  TotalContentCnt
	);

typedef void (CALLBACK * HTTPRECVEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
	LPBYTE Data, 
	UINT ByteCnt	
	);

typedef void (CALLBACK * HTTPLOGEVENT)(
	HPOWERTCP hSession,	/* identifies session notifying our app */ 
	DWORD     UserData,  /* UserData from Connect() function */
  	LPCSTR Data, 
	UINT DataCnt, 
	HTTP_SECTION Section, 
	UINT CumMessageCnt, 
	UINT TotalMessageCnt
	);

/*
   PowerTCP DLL functions follow...
*/

/* TCP functions for PxxTCPC6.DLL */
/* ConnectTelnet() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTcp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort,     /* port to connect to (usually 23 for TELNET) */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTcp() always generates an ListenEvent() notification */
HPOWERTCP CALLBACK ListenTcp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,      /* port for accepting connections (usually 23 for TELNET) */ 
	LISTENEVENT ListenEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* new for v. 1.1...notifies you of closed listener */ 
	ACCEPTEVENT AcceptEvent, /* your event handler for accepting a connection */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* AcceptTcp() always generates a ConnectEvent() notification */
HPOWERTCP CALLBACK AcceptTcp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	HPOWERTCP hListener,	/* hPowerTcp that generated AcceptEvent() */ 
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ConnectTelnet() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTcp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort,     /* port to connect to (usually 23 for TELNET) */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTcp() always generates an ListenEvent() notification */
HPOWERTCP CALLBACK ListenTcp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,      /* port for accepting connections (usually 23 for TELNET) */ 
	LISTENEVENT ListenEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* new for v. 1.1...notifies you of closed listener */ 
	ACCEPTEVENT AcceptEvent, /* your event handler for accepting a connection */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* AcceptTcp() always generates a ConnectEvent() notification */
HPOWERTCP CALLBACK AcceptTcp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	HPOWERTCP hListener,	/* hPowerTcp that generated AcceptEvent() */ 
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* SendTcp() always generates a SendEvent() notification */
BOOL CALLBACK SendTcp (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPVOID Data,     		/* pointer to data for sending */ 
	UINT ByteCnt,      /* data count to send */
	BOOL Urgent,			/* urgent flag for out-of-band data (usually not used) */ 
	DWORD DataTag        /* id tag for data sent */
	);
/* CloseTcp() always generates a RecvEvent() notification with NULL parameters */
BOOL CALLBACK CloseTcp (
	HPOWERTCP hSession,	/* identifies session */ 
	BOOL NoDelay			/* if TRUE, outstanding buffers are lost */
	                     /* if FALSE, gracefull close is attempted */
	);
/* QueryTcp() always generates a QueryEvent() notification with network information */ 
BOOL CALLBACK QueryTcp (
	DWORD UserData,			 /* user-defined instance passed back by all callback functions */ 
	QUERYEVENT QueryEvent,		  /* event handler for query request callback */
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* StateTcp() just returns PT_STATE */
PT_STATE CALLBACK StateTcp (
	HPOWERTCP hSession	/* identifies session */ 
	);
/* RecvTcp() will generate at least one RecvEvent() if data is in the network buffers */
BOOL CALLBACK RecvTcp (
	HPOWERTCP hSession,	/* identifies session */
	UINT MaxByteCnt		/* maximum ByteCnt for any RecvEvent() */ 
	);

/* TELNET functions for PxxTNTC5.DLL */
/* ConnectTelnet() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTelnet (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort,     /* port to connect to (usually 23 for TELNET) */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	CMDEVENT CmdEvent,   /* your event handler for received TELNET commands */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTelnet() always generates an ListenEvent() notification */
HPOWERTCP CALLBACK ListenTelnet (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,      /* port for accepting connections (usually 23 for TELNET) */ 
	LISTENEVENT ListenEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* new for v. 1.1...notifies you of closed listener */ 
	ACCEPTEVENT AcceptEvent, /* your event handler for accepting a connection */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* AcceptTelnet() generates a ConnectEvent() notification if successful */
HPOWERTCP CALLBACK AcceptTelnet (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,   /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	HPOWERTCP hListener,	/* hPowerTcp that generated AcceptEvent() */ 
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	CMDEVENT CmdEvent,   /* your event handler for received TELNET commands */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	); 
/* ConnectTelnet() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTelnet1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort,     /* port to connect to (usually 23 for TELNET) */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	CMDEVENT CmdEvent,   /* your event handler for received TELNET commands */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTelnet() always generates an ListenEvent() notification */
HPOWERTCP CALLBACK ListenTelnet1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */ 
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,      /* port for accepting connections (usually 23 for TELNET) */ 
	LISTENEVENT ListenEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* new for v. 1.1...notifies you of closed listener */ 
	ACCEPTEVENT AcceptEvent, /* your event handler for accepting a connection */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* AcceptTelnet() generates a ConnectEvent() notification if successful */
HPOWERTCP CALLBACK AcceptTelnet1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,   /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	HPOWERTCP hListener,	/* hPowerTcp that generated AcceptEvent() */ 
	CONNECTEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVEVENT RecvEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	CMDEVENT CmdEvent,   /* your event handler for received TELNET commands */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
	
//.tve.01  Insert Begin ...
// Add accessor to setup callbacks for data buffer send/receive, and send & rec functions
BOOL CALLBACK EncryptTelnet (
	HPOWERTCP hListener,	/* hPowerTcp that generated EncryptTelnet() */ 
	RECVTELNETBUF RecvTelnetBuf, /* your event handler for receiving data */ 
	SENDTELNETBUF SendTelnetBuf  /* your event handler for Send confirmation */ 
	);
BOOL CALLBACK SendBufEvent (
	HPOWERTCP hSession,	/* identifies session for data being sent */ 
	LPVOID Data,         /* pointer to data being sent */ 
	UINT ByteCnt          /* data count being sent */ 
	);
BOOL CALLBACK RecvBufEvent (
	HPOWERTCP hSession,	/* identifies session for received data */ 
	LPVOID Data,         /* pointer to data being received */ 
	UINT ByteCnt          /* data count received */ 
	);
//.tve.01  Insert End...

/* SendTelnet() always generates a SendEvent() notification */
BOOL CALLBACK SendTelnet (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPVOID Data,         /* pointer to data for sending */ 
	UINT ByteCnt,          /* data count to send */ 
	BOOL Urgent,			/* urgent flag for out-of-band data (usually not used) */ 
	DWORD DataTag   /* instance identification of data sent */
	);
/* SendTelnetCmd() always generates a SendEvent() notification */
BOOL CALLBACK SendTelnetCmd (
	HPOWERTCP hSession,	/* identifies session */ 
	BYTE      Cmd,       /* TELNET command (WILL, WONT, DO, DONT...) */ 
	BYTE      Option,    /* TELNET option number */ 
	LPBYTE    SubOption, /* pointer to sub-option string */ 
	UINT    SubOptionCnt,       /* byte count for SubOption */
	DWORD DataTag   /* instance identification of data sent */
	);
/* CloseTelnet() always generates an ExceptionEvent() notification */
BOOL CALLBACK CloseTelnet (
	HPOWERTCP hSession,	/* identifies session */ 
	BOOL NoDelay			/* if TRUE, outstanding buffers are lost */
	                     /* if FALSE, gracefull close is attempted */
	);
/* StateTelnet() just returns PT_STATE */
PT_STATE CALLBACK StateTelnet (
	HPOWERTCP hSession	/* identifies session */ 
	);
/* RecvTelnet() will generate at least one RecvEvent() if data is in the network buffers */
BOOL CALLBACK RecvTelnet (
	HPOWERTCP hSession,	/* identifies session */ 
	UINT MaxByteCnt		/* maximum ByteCnt for any RecvEvent() */ 
	);

/* TELNET command definitions */
#define SE_CMD 240
#define NOP_CMD 241
#define DATA_MARK_CMD 242
#define BREAK_CMD 243
#define INTERRUPT_PROCESS_CMD 244
#define ABORT_OUTPUT_CMD 245
#define ARE_YOU_THERE_CMD 246
#define ERASE_CHARACTER_CMD 247
#define ERASE_LINE_CMD 248
#define GO_AHEAD_CMD 249
#define WILL_CMD 251
#define WONT_CMD 252
#define DO_CMD 253
#define DONT_CMD 254
#define SB_CMD 250 /* suboption negotiation */
#define IAC_CMD 255 /* decimal 255 */
#define SE_CMD 240 /* decimal 240, suboption negotiation */

/* TELNET options */
#define TERMTYPE_OPTION 24

/* FTP functions for PxxFTPC6.DLL */


/* AbortFtp() */
BOOL CALLBACK AbortFtp(HPOWERTCP hSession);

/* AllocateFtp() */
BOOL CALLBACK AllocateFtp(HPOWERTCP hSession,LONG MaxFileSize,
						  LONG MaxRecordSize);
/* AppeFtp() */
/* .mjb.1 BOOL CALLBACK AppeFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, WORD ListenTimeout); */
BOOL CALLBACK AppeFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout);

/* ChDirFtp() */
BOOL CALLBACK ChDirFtp(HPOWERTCP hSession,LPCSTR PathName);

/* ChDirUpFtp() */
BOOL CALLBACK ChDirUpFtp(HPOWERTCP hSession);

/* CloseFtp() */
BOOL CALLBACK CloseFtp(HPOWERTCP hSession);

/* CloseDataFtp() */
BOOL CALLBACK CloseDataFtp(HPOWERTCP hSession);

/* CommandFtp() */
BOOL CALLBACK CommandFtp(HPOWERTCP hSession,LPCSTR CommandStr);

/* DeleteFtp() */
BOOL CALLBACK DeleteFtp(HPOWERTCP hSession,LPCSTR PathName);

/* FileStructFtp() */
BOOL CALLBACK FileStructFtp(HPOWERTCP hSession,FTP_FILE_STRUCT Structure);

/* HelpFtp() */
BOOL CALLBACK HelpFtp(HPOWERTCP hSession,LPCSTR Command);

/* LastCommandFtp() */
FTP_COMMAND CALLBACK LastCommandFtp(HPOWERTCP hSession);

/* ListFtp() */
/* .mjb.1 BOOL CALLBACK ListFtp(HPOWERTCP hSession,LPCSTR PathName, WORD ListenTimeout); */
BOOL CALLBACK ListFtp(HPOWERTCP hSession,LPCSTR PathName, BOOL Passive, WORD ListenTimeout);

/* LoginFtp */
/* PxxFTPC6.DLL added ExceptionEvent callback to LoginHostFtp */
HPOWERTCP CALLBACK LoginHostFtp(DWORD UserData,
								LPCSTR OemLicense,
								PT_FLAGS Flags,
								LPCSTR RemoteHost,
								LPCSTR LocalDotAddr,
								LPCSTR User,
								LPCSTR Password,
								LPCSTR Account,
								CONNECTEVENT ConnectEvent,
								LOGEVENT LogEvent,
								RECVEVENT RecvEvent,
								REPLYEVENT ReplyEvent,
								SENDEVENT SendEvent,
								TRANSFEREVENT TransferEvent,
								EXCEPTIONEVENT ExceptionEvent);
								
/* LoginFtp */
/* PxxFTPC6.DLL added ExceptionEvent callback to LoginHostFtp */
HPOWERTCP CALLBACK LoginHostFtp1(DWORD UserData,
								LPCSTR OemLicense,
								PT_FLAGS Flags,
								LPCSTR RemoteHost,
								LPCSTR LocalDotAddr,
								LPCSTR User,
								LPCSTR Password,
								LPCSTR Account,
								CONNECTEVENT ConnectEvent,
								LOGEVENT LogEvent,
								RECVEVENT RecvEvent,
								REPLYEVENT ReplyEvent,
								SENDEVENT SendEvent,
								TRANSFEREVENT TransferEvent,
								EXCEPTIONEVENT ExceptionEvent);

/* LogoutFtp() */
BOOL CALLBACK LogoutFtp(HPOWERTCP hSession);

/* MakeDirFtp() */
BOOL CALLBACK MakeDirFtp(HPOWERTCP hSession,LPCSTR PathName);

/* ModeFtp() */
BOOL CALLBACK ModeFtp(HPOWERTCP hSession,FTP_TRANSFER_MODE TransferMode);

/* NameListFtp() */
/* .mjb.1 BOOL CALLBACK NameListFtp(HPOWERTCP hSession,LPCSTR PathName, WORD ListenTimeout); */
BOOL CALLBACK NameListFtp(HPOWERTCP hSession,LPCSTR PathName, BOOL Passive, WORD ListenTimeout);

/* NoopFtp() */
BOOL CALLBACK NoopFtp(HPOWERTCP hSession);

/* PassiveFtp() */
/* .mjb.1 BOOL CALLBACK PassiveFtp(HPOWERTCP hSession); */

/* PortFtp() */
BOOL CALLBACK PortFtp(HPOWERTCP hSession,LPCSTR HostPort);

/* PrintWorkingDirFtp() */
BOOL CALLBACK PrintWorkingDirFtp(HPOWERTCP hSession);

/* ReinitializeFtp() */
BOOL CALLBACK ReinitializeFtp(HPOWERTCP hSession);

/* RemoveDirFtp() */
BOOL CALLBACK RemoveDirFtp(HPOWERTCP hSession,LPCSTR PathName);

/* RenameFtp() */
BOOL CALLBACK RenameFtp(HPOWERTCP hSession,LPCSTR FromPathName,
						LPCSTR ToPathName);
// .awp.17						
/* RestartFtp() */
BOOL CALLBACK RestartFtp(HPOWERTCP hSession,LONG Marker);

/* RetrieveFtp() */
/* .mjb.1 BOOL CALLBACK RetrieveFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, WORD ListenTimeout); */
BOOL CALLBACK RetrieveFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout);
		
/* SendFtp() */
BOOL CALLBACK SendFtp(HPOWERTCP hSession, const LPVOID Data, UINT Cnt);

/* SiteFtp() */
BOOL CALLBACK SiteFtp(HPOWERTCP hSession,LPCSTR Desc);
 
/* StateFtp() */
PT_STATE CALLBACK StateFtp(HPOWERTCP hSession);

/* StatusFtp() */
BOOL CALLBACK StatusFtp(HPOWERTCP hSession,LPCSTR PathName);

/* StoreFtp() */
/* .mjb.1 BOOL CALLBACK StoreFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, WORD ListenTimeout); */
BOOL CALLBACK StoreFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout);
		
/* StoreUniqueFtp() */
/* .mjb.1 BOOL CALLBACK StoreUniqueFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, WORD ListenTimeout); */
BOOL CALLBACK StoreUniqueFtp(HPOWERTCP hSession,LPCSTR LocalFileSpec, LPCSTR RemoteFileSpec, UINT BufferSize,
		FTP_DATATYPE Type, BOOL Passive, WORD ListenTimeout);
		
/* StructMountFtp() */
BOOL CALLBACK StructMountFtp(HPOWERTCP hSession,LPCSTR PathName);

/* SystemFtp() */
BOOL CALLBACK SystemFtp(HPOWERTCP hSession);

/* TypeFtp() */
/* .mjb.1 BOOL CALLBACK TypeFtp(HPOWERTCP hSession,LPCSTR RepType); */

/* UDP functions for PxxUDPC6.DLL */
/* ConnectUdp() generates an ConnectUdpEvent() notification if successful */
HPOWERTCP CALLBACK ConnectUdp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTUDPEVENT ConnectUdpEvent, /* your event handler for session Open/Close notification */ 
	RECVUDPEVENT RecvUdpEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ConnectUdp() generates an ConnectUdpEvent() notification if successful */
HPOWERTCP CALLBACK ConnectUdp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTUDPEVENT ConnectUdpEvent, /* your event handler for session Open/Close notification */ 
	RECVUDPEVENT RecvUdpEvent, /* your event handler for receiving data */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);

/* SendUdp() always generates a SendUdpEvent() notification */
BOOL CALLBACK SendUdp (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort,     /* port to connect to (usually 23 for TELNET) */ 
	LPVOID Data,     		/* pointer to data for sending */ 
	UINT ByteCnt,      /* data count to send */
	DWORD DataTag        /* id tag for data sent */
	);
/* CloseUdp() always generates a RecvUdpEvent() notification with NULL parameters */
BOOL CALLBACK CloseUdp (
	HPOWERTCP hSession,	/* identifies session */
	BOOL Abort				/* immediately release buffers */ 
	);
/* QueryUdp() always generates a QueryEvent() notification with network information */ 
BOOL CALLBACK QueryUdp (
	DWORD UserData,			 /* user-defined instance passed back by all callback functions */ 
	QUERYEVENT QueryEvent,		  /* event handler for query request callback */
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* StateUdp() just returns PT_STATE */
PT_STATE CALLBACK StateUdp (
	HPOWERTCP hSession	/* identifies session */ 
	);

/* SNMP functions for PxxSNMC5.DLL */
/* CloseSnmp() always generates a RecvSnmpEvent() and RecvTrapEvent() notification with NULL parameters */
BOOL CALLBACK CloseSnmp (
	HPOWERTCP hSession,	/* identifies session */ 
	BOOL Abort
	);
/* ConnectSnmp() generates a ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectSnmp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVSNMPEVENT SnmpEvent, /* your event handler for receiving SNMP version 1 information */ 
	RECVTRAPEVENT TrapEvent, /* your event handler for receiving SNMP version 1 Traps */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ConnectSnmp() generates a ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectSnmp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for "don't care", but can be specified */
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open/Close notification */ 
	RECVSNMPEVENT SnmpEvent, /* your event handler for receiving SNMP version 1 information */ 
	RECVTRAPEVENT TrapEvent, /* your event handler for receiving SNMP version 1 Traps */ 
	SENDEVENT SendEvent, /* your event handler for Send confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* SendGetNextRequestSnmp() always generates at least one SendEvent() notification */
BOOL CALLBACK SendGetNextRequestSnmp(
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */
	WORD RemotePort,
	LPCSTR Community,
	DWORD RequestID,
	UINT nObjects,
	LPSTR FAR *ObjectID
	);
/* SendGetRequestSnmp() always generates at least one SendEvent() notification */
BOOL CALLBACK SendGetRequestSnmp(
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */
	WORD RemotePort,
	LPCSTR Community,
	DWORD RequestID,
	UINT nObjects,
	LPSTR FAR *ObjectID
	);
/* SendGetResponseSnmp() always generates a SendEvent() notification */
BOOL CALLBACK SendGetResponseSnmp(
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */
	WORD RemotePort,
	LPCSTR Community,
	DWORD RequestID,
	SNMP_ERROR ErrorStatus,
	WORD ErrorIndex,
	UINT nObjects,
	LPSTR FAR *ObjectID,
	LONG FAR *ObjectValue,
	SNMP_OBJECT_TYPE FAR *ObjectType
	);
/* SendSetRequestSnmp() always generates a SendEvent() notification */
BOOL CALLBACK SendSetRequestSnmp(
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */
	WORD RemotePort,
	LPCSTR Community,
	DWORD RequestID,
	UINT nObjects,
	LPSTR FAR *ObjectID,
	LONG FAR *ObjectValue,
	SNMP_OBJECT_TYPE FAR *ObjectType
	);
/* SendTrapSnmp() always generates at least one SendEvent() notification */
BOOL CALLBACK SendTrapSnmp ( 
	HPOWERTCP hSession, 
	LPCSTR RemoteHost, 
	WORD RemotePort, 
	LPCSTR Community, 
	LPCSTR Enterprise, 
	LPCSTR NodeDotAddr, 
	SNMP_TRAP SnmpTrap, 
	int SpecialTrap, 
	DWORD TimeStamp, 
	UINT nObjects, 
	LPSTR FAR *ObjectID, 
	LONG FAR *ObjectValue, 
	SNMP_OBJECT_TYPE FAR *ObjectType 
	) ;
/* StateUdp() just returns PT_STATE */
PT_STATE CALLBACK StateSnmp (
	HPOWERTCP hSession	/* identifies session */ 
	);

/* TFTP functions for PxxTFTC5.DLL */
/* ConnectTftp() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTftp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD TimeoutInterval,		/* normally 0 for TFTP client, 69 for TFTP server operation */
	WORD MaxRetries,
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open notification */ 
	TFTPEVENT TftpEvent, /* your event handler for sending/receiving data confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ConnectTftp() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ConnectTftp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD TimeoutInterval,		/* normally 0 for TFTP client, 69 for TFTP server operation */
	WORD MaxRetries,
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open notification */ 
	TFTPEVENT TftpEvent, /* your event handler for sending/receiving data confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTftp() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ListenTftp (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for TFTP client, 69 for TFTP server operation */
	WORD TimeoutInterval,
	WORD MaxRetries,
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open notification */ 
	TFTPEVENT TftpEvent, /* your event handler for sending/receiving data confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* ListenTftp() generates an ConnectEvent() notification if successful */
HPOWERTCP CALLBACK ListenTftp1 (
	DWORD UserData,      /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,    /* use license number assigned to you by Dart */
	PT_FLAGS Flags,		/* specify desired flags */ 
	LPCSTR LocalDotAddr, /* NULL or local dot address for multi-homed host */ 
	WORD LocalPort,		/* normally 0 for TFTP client, 69 for TFTP server operation */
	WORD TimeoutInterval,
	WORD MaxRetries,
	CONNECTUDPEVENT ConnectEvent, /* your event handler for session Open notification */ 
	TFTPEVENT TftpEvent, /* your event handler for sending/receiving data confirmation */ 
	EXCEPTIONEVENT ExceptionEvent /* your event handler for exception notifications */
	);
/* GetTftp() starts a file tranfer from the remote machine to the local machine */
BOOL CALLBACK GetTftp (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort, 
	LPCSTR LocalFileSpec,   /* fully specified local file spec for OpenFile() function */ 
	LPCSTR RemoteFileSpec,   /* fully specified remote file spec */ 
	BOOL Ascii     /* TRUE for NETASCII transfer, FALSE for OCTET transfer */ 
	);
/* PutTftp() starts a file tranfer to the remote machine from the local machine */
BOOL CALLBACK PutTftp (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPCSTR RemoteHost,   /* use name or dot notation */ 
	WORD RemotePort, 
	LPCSTR LocalFileSpec,   /* fully specified local file spec for OpenFile() function */ 
	LPCSTR RemoteFileSpec,   /* fully specified remote file spec */ 
	BOOL Ascii     /* TRUE for NETASCII transfer, FALSE for OCTET transfer */ 
	);
/* CloseTftp() always generates a RecvUdpEvent() notification with NULL parameters */
BOOL CALLBACK CloseTftp (
	HPOWERTCP hSession,	/* identifies session */ 
	BOOL Abort	/* abort cancels ongoing transfers immediately */ 
	);
/* StateTftp() just returns PT_STATE */
PT_STATE CALLBACK StateTftp (
	HPOWERTCP hSession	/* identifies session */ 
	);
/* AbortTransferTftp() returns a BOOL value */
BOOL CALLBACK AbortTransferTftp(
	HPOWERTCP hSession,
	DWORD TransferID
	);

/* VT220 functions for PxxVT2C5.DLL */
/* CreateVt220() creates the VT220 window and initializes resources */
HPOWERTCP CALLBACK CreateVt (
	HWND FAR *hWnd,        /* location for the new window handle to go */ 
	HWND hParentWnd,       /* specify parent window (child window is created) */
	DWORD UserData,        /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,     /* use license number assigned to you by Dart */ 
	HOSTCOMMANDEVENT HostCommand,		/* callback for all host command notifications */ 
	KEYDOWNEVENT KeyDown,  /* callback for all key down events */ 
	KEYPRESSEVENT KeyPress, /* callback for all key press events */ 
	CLICKEVENT Click			/* callback for left button down event */ 
	);
/* CreateVt220() creates the VT220 window and initializes resources */
HPOWERTCP CALLBACK CreateVt1 (
	HWND FAR *hWnd,        /* location for the new window handle to go */ 
	HWND hParentWnd,       /* specify parent window (child window is created) */
	DWORD UserData,        /* user-defined data passed back by all callback functions */ 
	LPCSTR OemLicense,     /* use license number assigned to you by Dart */ 
	HOSTCOMMANDEVENT HostCommand,		/* callback for all host command notifications */ 
	KEYDOWNEVENT KeyDown,  /* callback for all key down events */ 
	KEYPRESSEVENT KeyPress, /* callback for all key press events */ 
	CLICKEVENT Click			/* callback for left button down event */ 
	);
/* DestroyVt220() releases all resources, closes window */
BOOL CALLBACK DestroyVt (
	HPOWERTCP hSession	/* identifies session for sending data */ 
	);
/* DisplayVt220() displays info  */
BOOL CALLBACK DisplayVt (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	LPVOID Data,     		/* pointer to data for displaying */ 
	UINT ByteCnt         /* data count to display */
	);
/* GetVt220() gets an attribute */
DWORD CALLBACK GetVt (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	PT_ATTRIBUTE Attr		/* attribute of interest */
	);
/* GetStyleVt220() returns the style of the text */
BOOL CALLBACK GetStyleVt (
	HPOWERTCP hSession,	/* identifies session */ 
	LPSTR Buffer,			
	UINT FAR *Size
	);
/* GetTerminalVt220() returns the terminal type being emulated */
PT_ATTRIBUTE CALLBACK GetTerminalVt (
	HPOWERTCP hSession	/* identifies session */ 
	);
/* GetTextVt220() returns the text */
BOOL CALLBACK GetTextVt (
	HPOWERTCP hSession,	/* identifies session */ 
	LPSTR Buffer,			
	UINT FAR *Size
	);
/* SetVt220() sets an attribute */
BOOL CALLBACK SetVt (
	HPOWERTCP hSession,	/* identifies session for sending data */ 
	PT_ATTRIBUTE Attr,	/* attribute of interest */
	DWORD Value				/* value to set attribute to */
	);
	
BOOL CALLBACK GetStyleExVt (
	HPOWERTCP hSession,	/* identifies session */ 
	LPSTR Buffer,			
	UINT FAR *Size,
	int StartLine
	);

BOOL CALLBACK GetTextExVt (
	HPOWERTCP hSession,	/* identifies session */ 
	LPSTR Buffer,			
	UINT FAR *Size,
	int StartLine
	);

/* Simple Mail functions go here... */

BOOL CALLBACK CloseSmtp (
    HPOWERTCP hSession,
    BOOL      NoDelay
    );
                                    
BOOL CALLBACK ExpandSmtp (
    HPOWERTCP hSession,
    LPCSTR MailingList
    );
    	
BOOL CALLBACK HelpSmtp (
    HPOWERTCP hSession,
    SMTP_COMMAND Topic
    );
        
BOOL CALLBACK MailSmtp (
    HPOWERTCP hSession,
    SMTP_TYPE SmtpType, 
    LPCSTR Sender, 
    LPCSTR Recipients,
    BOOL HeaderDate,
	BOOL HeaderFrom, 
	LPCSTR HeaderSubject, 
	BOOL HeaderTo, 
	LPBYTE Message, 
	UINT ByteCnt,
	LPCSTR Attachments,
	LPCSTR eType
	);
	    
BOOL CALLBACK NoopSmtp (
    HPOWERTCP hSession
    );
        
BOOL CALLBACK ResetSmtp (
    HPOWERTCP hSession
    );
        
BOOL CALLBACK SendSmtp(
    HPOWERTCP hSession,
    LPVOID  Data, 
    UINT ByteCnt, 
    DWORD DataTag
    );
    
HPOWERTCP CALLBACK ConnectSmtp(
    DWORD UserData, 
    LPCSTR OemLicense, 
    PT_FLAGS Flags, 
    LPCSTR RemoteHost, 
    WORD RemotePort,
	LPCSTR LocalDotAddr,
	WORD LocalPort, 
	CONNECTSMTPEVENT ConnectEvent,
	SMTPEVENT SmtpEvent,
	LOGSMTPEVENT LogEvent,
	ATTACHSMTPEVENT AttachEvent, 
	EXCEPTIONEVENT ExceptionEvent
	);
	
HPOWERTCP CALLBACK ConnectSmtp1(
    DWORD UserData, 
    LPCSTR OemLicense, 
    PT_FLAGS Flags, 
    LPCSTR RemoteHost, 
    WORD RemotePort,
	LPCSTR LocalDotAddr,
	WORD LocalPort, 
	CONNECTSMTPEVENT ConnectEvent,
	SMTPEVENT SmtpEvent,
	LOGSMTPEVENT LogEvent,
	ATTACHSMTPEVENT AttachEvent, 
	EXCEPTIONEVENT ExceptionEvent
	);
    
BOOL CALLBACK TurnSmtp(
    HPOWERTCP hSession
    );
    
PT_STATE CALLBACK StateSmtp(
    HPOWERTCP hSession
    ); 
       
BOOL CALLBACK VerifySmtp(
    HPOWERTCP hSession,
    LPCSTR UserName
    );

//
// Put the POP3 stuff here....

BOOL CALLBACK ClosePop3(
    HPOWERTCP hSession,
    BOOL NoDelay);
         
HPOWERTCP CALLBACK ConnectPop3(
    DWORD UserData,
    LPCSTR OemLicense, 
    PT_FLAGS Flags, 
    LPCSTR RemoteHost, 
    WORD RemotePort, 
    LPCSTR LocalDotAddr, 
    WORD LocalPort, 
    LPCSTR User, 
    LPCSTR Password, 
    LPCSTR Secret,
    CONNECTEVENT ConnectEvent,
    POP3EVENT Pop3Event,
    FILEEVENT FileEvent, 
    LOGPOP3EVENT LogPop3Event, 
	HEADEREVENT HeaderEvent,			//.tve.5
	EXCEPTIONEVENT ExceptionEvent);  
	
HPOWERTCP CALLBACK ConnectPop31(
    DWORD UserData,
    LPCSTR OemLicense, 
    PT_FLAGS Flags, 
    LPCSTR RemoteHost, 
    WORD RemotePort, 
    LPCSTR LocalDotAddr, 
    WORD LocalPort, 
    LPCSTR User, 
    LPCSTR Password, 
    LPCSTR Secret,
    CONNECTEVENT ConnectEvent,
    POP3EVENT Pop3Event,
    FILEEVENT FileEvent, 
    LOGPOP3EVENT LogPop3Event, 
	HEADEREVENT HeaderEvent,			//.tve.5
	EXCEPTIONEVENT ExceptionEvent);
     
BOOL CALLBACK DeletePop3(
    HPOWERTCP hSession,
    WORD MessageNumber);
    
BOOL CALLBACK ListPop3(
    HPOWERTCP hSession,
    WORD MessageNumber);
    
BOOL CALLBACK NoopPop3(
    HPOWERTCP hSession); 
    
BOOL CALLBACK ResetPop3(
    HPOWERTCP hSession);
    
BOOL CALLBACK RetrievePop3(
    HPOWERTCP hSession,
    WORD MessageNumber,
    LPCSTR MailFileSpec, 
    LPCSTR AttachmentDir);
    
PT_STATE CALLBACK StatePop3(
    HPOWERTCP hSession);
    
BOOL CALLBACK StatusPop3(
    HPOWERTCP hSession); 
    
BOOL CALLBACK TopPop3(
    HPOWERTCP hSession,
    WORD MessageNumber, 
    WORD Lines);
    
BOOL CALLBACK UidlPop3(
    HPOWERTCP hSession,
    WORD MessageNumber);

/* HTTP functions go here... */
// to make an active connection
HPOWERTCP CALLBACK 
ConnectHttp (
	DWORD UserData,      // user-defined instance passed back by all callback functions // 
	LPCSTR OemLicense,   // use license number assigned to you by Dart //
	PT_FLAGS Flags,		 // specify desired options // 
	LPCSTR RemoteHost,   // use name or dot notation // 
	WORD RemotePort,     // port to connect to (usually 80 for HTTP) // 
	LPCSTR LocalDotAddr, // NULL or local dot address for multi-homed host // 
	WORD LocalPort,		 // local port if desired //
	CONNECTEVENT ConnectEvent, // your event handler for session Open/Close notification // 
	STATUSEVENT StatusEvent,  // your event handler for incoming status data of http message //
	HEADERLINEEVENT HeaderLineEvent, // your event handler for incoming headerlines of http message //
	CONTENTEVENT ContentEvent,  // your event handler for incoming content data of http message //
	HTTPRECVEVENT HttpRecvEvent, // your event handler for incoming http data //
	HTTPLOGEVENT LogEvent, // your event handler progressing transmission of http message //
	EXCEPTIONEVENT ExceptionEvent // your event handler for exception notifications //
	);
// to make an active connection
HPOWERTCP CALLBACK 
ConnectHttp1 (
	DWORD UserData,      // user-defined instance passed back by all callback functions // 
	LPCSTR OemLicense,   // use license number assigned to you by Dart //
	PT_FLAGS Flags,		 // specify desired options // 
	LPCSTR RemoteHost,   // use name or dot notation // 
	WORD RemotePort,     // port to connect to (usually 80 for HTTP) // 
	LPCSTR LocalDotAddr, // NULL or local dot address for multi-homed host // 
	WORD LocalPort,		 // local port if desired //
	CONNECTEVENT ConnectEvent, // your event handler for session Open/Close notification // 
	STATUSEVENT StatusEvent,  // your event handler for incoming status data of http message //
	HEADERLINEEVENT HeaderLineEvent, // your event handler for incoming headerlines of http message //
	CONTENTEVENT ContentEvent,  // your event handler for incoming content data of http message //
	HTTPRECVEVENT HttpRecvEvent, // your event handler for incoming http data //
	HTTPLOGEVENT LogEvent, // your event handler progressing transmission of http message //
	EXCEPTIONEVENT ExceptionEvent // your event handler for exception notifications //
	);
                                     
BOOL CALLBACK 
CloseHttp (HPOWERTCP hSession, BOOL Abort);
                                     
PT_STATE CALLBACK 
StateHttp (HPOWERTCP hSession);
 
BOOL CALLBACK 
SendRequestHttp (
	HPOWERTCP hSession,	
	HTTP_VER Version, 
	HTTP_METHOD Method, 
	LPCSTR Extension, 
	LPCSTR URI, 
	LPCSTR Headers,
	LPVOID Content, 
	DWORD  ContentCnt, 
	LPCSTR ContentSendFile, 
	LPCSTR ContentRecvFile  
	);

LPCSTR CALLBACK 
AddHeaderLineHttp ( 
	HPOWERTCP hSession, 
	HTTP_FIELD FieldType, 
	LPCSTR FieldValue, 
	LPCSTR HeaderLine 
	);

//+ .jrg.12
/*  MIME functions go here */

HPOWERTCP CALLBACK 
StartMime ( 
	DWORD UserData,      // user-defined instance passed back by all callback functions // 
	DATAEVENT DataEvent,
	FILEEVENT FileEvent,
	MIMEHEADEREVENT HeaderEvent,
	LABELEVENT LabelEvent,
	EXCEPTIONEVENT ExceptionEvent
	);

void CALLBACK
StopMime (
	HPOWERTCP hSession
		);

BOOL CALLBACK
ConfigMime (
	HPOWERTCP hSession,
	LPCSTR OemLicense,
	PT_FLAGS Flags,
	LPCSTR AttachmentDir,
	UINT DecodeBuffSize 
		);

BOOL CALLBACK
DecodeMime (
	HPOWERTCP hSession,
	LPCSTR SourceFile,
	LPSTR SourceData,
	LPCSTR OutFileSpec
		);

void CALLBACK 
ResetMime (
	HPOWERTCP hSession
		);
//-jrg.12


#ifdef __cplusplus
}
#endif

#endif // POWERTCP_H_DEFINED

