#pragma once
#include "CmdExeCmdItem.h"

class CMVTerminalView;

class CCmdExeCmdItemClearWindowParams
{
public:
	CString               m_sComPort;
};


class CCmdExeCmdItemClearWindow: public CCmdExeCmdItem
{
	CCmdExeCmdItemClearWindowParams   m_params;

private:

	CMVTerminalView*                    GetView();

public:
	CCmdExeCmdItemClearWindow();
	virtual ~CCmdExeCmdItemClearWindow(void);

	virtual  void				 	    SetParams(CCmdExeCmdItemClearWindowParams params) {m_params = params;}
	virtual  CCmdExeCmdItemClearWindowParams   GetParams() {return m_params;}

	virtual BOOL	    				Serialize(CMarkup & xmlWriter);
	virtual BOOL	    				DeSerialize(CMarkup & xmlReader);
	virtual BOOL        				Execute();
	virtual CCmdExeItem*				Clone();

};
