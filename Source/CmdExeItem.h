#pragma once
#include "Markup.h"

class CCmdExeItem
{
protected:

	CString				 m_sName;
	CString				 m_sDisplayName;
	BOOL   				 m_bEnable;
	BOOL    			 m_bStopExecutionOnError;
	BOOL    			 m_bErrorOccured;

public:
	CCmdExeItem(void);
	virtual ~CCmdExeItem(void);

	virtual BOOL   		 GetErrorOccured()			          {return m_bStopExecutionOnError;}
	virtual void   		 SetErrorOccured(BOOL bErrorOccured)  {m_bErrorOccured = bErrorOccured;}

	virtual void   		 SetStopExecutionOnError(BOOL bStop)  {m_bStopExecutionOnError = bStop;}
	virtual BOOL   		 GetStopExecutionOnError()			  {return m_bStopExecutionOnError;}

	virtual void   		 SetEnable(BOOL bEnable)              {m_bEnable = bEnable;}
	virtual BOOL   		 GetEnable()			              {return m_bEnable;}

	virtual void   		 SetDisplayName(CString sDisplayName) {m_sDisplayName = sDisplayName;}
	virtual CString		 GetDisplayName()			          {return m_sDisplayName;}

	virtual void   		 SetName(CString sName)               {m_sName = sName;}
	virtual CString		 GetName()			                  {return m_sName;}

	virtual BOOL	     Serialize(CMarkup & xmlWriter);
	virtual BOOL	     DeSerialize(CMarkup & xmlReader);

	virtual void	   	 Log(CString sLogLine)            = 0;
	virtual BOOL         Execute()					      = 0;
	virtual CCmdExeItem* Clone()						  = 0;
};
