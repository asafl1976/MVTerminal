#pragma once
#include "CmdExeCmdItem.h"
#include "TextLine.h"


// CCmdExeCmdItemExecuteCommandDlg dialog
class CCmdExeCmdItemExecuteCommandParams
{
public:
	CString      m_sCmd;
	CString      m_sComPort;
};

class CMVTerminalView;

class CCmdExeCmdItemExecuteCommand : public CCmdExeCmdItem
{
	CCmdExeCmdItemExecuteCommandParams	 m_params;

	CMVTerminalView*	    GetView();

public:
	CCmdExeCmdItemExecuteCommand(); 
	virtual ~CCmdExeCmdItemExecuteCommand(void);

	virtual BOOL                          Execute();
	virtual CCmdExeItem   *               Clone();

	virtual BOOL	    				  Serialize(CMarkup & xmlWriter);
	virtual BOOL	    				  DeSerialize(CMarkup & xmlReader);

	virtual  void				 	      SetParams(CCmdExeCmdItemExecuteCommandParams params) {m_params = params;}
	virtual  CCmdExeCmdItemExecuteCommandParams   GetParams() {return m_params;}

};
