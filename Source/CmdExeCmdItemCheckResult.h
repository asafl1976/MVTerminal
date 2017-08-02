#pragma once
#include "CmdExeCmdItem.h"


// CCmdExeCmdItemCheckResultDlg dialog
typedef enum _ceCheckResultType
{
	CE_CHECK_RESULT_COMPARE_STRING_TYPE,
	CE_CHECK_RESULT_COMPARE_FILE_TYPE
}CE_CHECK_RESULT_TYPE;

class  CCmdExeCmdItemCheckResultParams
{
public:
	BOOL                  m_bFailIfFound;
	CString               m_sStr;
	CE_CHECK_RESULT_TYPE  m_type;
	CString               m_sComPort;
};

class CMVTerminalView;

class CCmdExeCmdItemCheckResult : public CCmdExeCmdItem 
{
	CString                m_sAppDataPath;
	CMVTerminalView		  *m_pView;

	CCmdExeCmdItemCheckResultParams     m_params;
	CMVTerminalView*					GetView();

public:
	CCmdExeCmdItemCheckResult();
	virtual        ~CCmdExeCmdItemCheckResult(void);

	virtual  void				 	    SetParams(CCmdExeCmdItemCheckResultParams params) {m_params = params;}
	virtual  CCmdExeCmdItemCheckResultParams   GetParams() {return m_params;}

	virtual BOOL	    				Serialize(CMarkup & xmlWriter);
	virtual BOOL	    				DeSerialize(CMarkup & xmlReader);
	virtual BOOL        				Execute();
	virtual CCmdExeItem*				Clone();
};
