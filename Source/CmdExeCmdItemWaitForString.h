#pragma once
#include "CmdExeCmdItem.h"

class CCmdExeCmdItemWaitForStringParams
{
public:
	int			m_nTimeout;
	CString     m_sStr;
	CString     m_sComPort;
};

class CMVTerminalView;

class CCmdExeCmdItemWaitForString : 	public CCmdExeCmdItem
{
	CMVTerminalView*	          GetView();

	CCmdExeCmdItemWaitForStringParams m_params;

public:
	CCmdExeCmdItemWaitForString();
	~CCmdExeCmdItemWaitForString(void);

	virtual BOOL			            Execute();
	virtual CCmdExeItem*                Clone();
	virtual BOOL	    				Serialize(CMarkup & xmlWriter);
	virtual BOOL	    				DeSerialize(CMarkup & xmlReader);

	CCmdExeCmdItemWaitForStringParams   GetParams() {return m_params;}
	void                                SetParams(CCmdExeCmdItemWaitForStringParams params) {m_params = params;}


};
