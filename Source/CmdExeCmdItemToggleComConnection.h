#pragma once
#include "CmdExeCmdItem.h"

// CCmdExeCmdItemToggleComConnectionDlg dialog
class CCmdExeCmdItemToggleComConnectionParams
{
public:
	CCmdExeCmdItemToggleComConnectionParams()
	{
		m_bConnect    = TRUE;
		m_bDisconnect = TRUE;
		m_sDelay      = "300";
	}
	BOOL      m_bConnect;
	BOOL      m_bDisconnect;
	CString   m_sDelay;
	CString   m_sComPort;
};

class CMVTerminalView;

class CCmdExeCmdItemToggleComConnection : public CCmdExeCmdItem
{
	CCmdExeCmdItemToggleComConnectionParams  m_params;

	CMVTerminalView*	 GetView();

public:
	CCmdExeCmdItemToggleComConnection(void);
	virtual ~CCmdExeCmdItemToggleComConnection(void);

	virtual BOOL	    							  Serialize(CMarkup & xmlWriter);
	virtual BOOL	    							  DeSerialize(CMarkup & xmlReader);
	virtual BOOL			  						  Execute();
	virtual CCmdExeItem*     						  Clone();
	virtual void				 	                  SetParams(CCmdExeCmdItemToggleComConnectionParams params) {m_params = params;}
	virtual CCmdExeCmdItemToggleComConnectionParams   GetParams() {return m_params;}
};


