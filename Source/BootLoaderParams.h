#pragma once

class CBootLoaderParams
{	
	CString		  m_sRunCommand;
	CString		  m_sTFTPLoadCommand;
	CString		  m_sTFTPBurnAsIsCommand;
	CString		  m_sTFTPBurnExeCommand;
    CString		  m_sTFTPLoadAddress;

public:
	CBootLoaderParams(void);
	~CBootLoaderParams(void);

	CString		 GetTFTPLoadAddress() {return m_sTFTPLoadAddress;}
	void		 SetTFTPLoadAddress(CString sTFTPLoadAddress) {m_sTFTPLoadAddress = sTFTPLoadAddress;}
	
	CString		 GetRunCommand() {return m_sRunCommand;}
	void		 SetRunCommand(CString sRunCommand) {m_sRunCommand = sRunCommand;}
	
	CString		 GetTFTPLoadCommand() {return m_sTFTPLoadCommand;}
	void		 SetTFTPLoadCommand(CString sTFTPLoadCommand) {m_sTFTPLoadCommand = sTFTPLoadCommand;}
	
	CString		 GetTFTPBurnAsIsCommand() {return m_sTFTPBurnAsIsCommand;}
	void		 SetTFTPBurnAsIsCommand(CString sTFTPBurnAsIsCommand) {m_sTFTPBurnAsIsCommand = sTFTPBurnAsIsCommand;}

	CString		 GetTFTPBurnExeCommand() {return m_sTFTPBurnExeCommand;}
	void		 SetTFTPBurnExeCommand(CString sTFTPBurnExeCommand) {m_sTFTPBurnExeCommand = sTFTPBurnExeCommand;}

};
