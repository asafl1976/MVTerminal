#pragma once

#include <list>
using namespace std;

#define PREVIOUS_COMMAND   0
#define NEXT_COMMAND	   1


class CCmdHistoryLogger
{  
	list<CString>    m_cmdList;
	CString          m_sCommand;
	CString			 m_sLastCmd;
	CString          m_sFilter;
	int				 m_nMaxCommandCount;
	int			     m_nCommandMinLenThreshold;
	int			     m_nCommandMaxLenThreshold;
	int				 m_nCmdIndex;
	bool			 m_bWasAtGetPrev;
	bool			 m_bWasAtGetNext;
	BOOL			 m_bEnable;

	void           Log(CString sCh);
	void           EndLog();
	void           AddCommand();
	BOOL		   IsCmdExist(CString sCmd);
	BOOL		   Filter(CString sCmd);
	void		   DeleteCommand(CString sCmd);

public:
	CCmdHistoryLogger(void);
	~CCmdHistoryLogger(void);

	CString	       GetCurrentCommand();
	CString	       GetCommandByIndex(int nCmd);
	int			   GetCommandCount();
	CString	       GetCommandByDirection(int nDirection);
	void	       GetInput(CString sCh);
	void	       ClearHistoryList() {m_cmdList.clear();}

	void	       SetMaxCmdCount(int nCmdCount) {m_nMaxCommandCount = nCmdCount;}
	int  	       GetMaxCmdCount() { return m_nMaxCommandCount;}

	void   	       SetFilter(CString sFilter) { m_sFilter = sFilter;}
	CString	       GetFilter() { return m_sFilter;}

	void	       SetCommandMinLenThreshold(int nCmdLen) {m_nCommandMinLenThreshold = nCmdLen;}
	int  	       GetCommandMinLenThreshold() { return m_nCommandMinLenThreshold;}
	void	       SetCommandMaxLenThreshold(int nCmdLen) {m_nCommandMaxLenThreshold = nCmdLen;}
	int  	       GetCommandMaxLenThreshold() { return m_nCommandMaxLenThreshold;}

	void   	       SetEnable(BOOL bEnable) { m_bEnable = bEnable;}
	BOOL   	       GetEnable()			   { return m_bEnable;}
	void           AddCommand(CString sCmd);
	void           Release();

};
