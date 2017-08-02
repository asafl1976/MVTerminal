#pragma once

class CCmdHistoryParams
{
	BOOL     m_bRemoveDuplicatesOnMenu;
	CString	 m_sFilter;
	BOOL     m_bUseTerminalHistory;
	int      m_nMaxCmdCount;
	int      m_nCmdMinLenThreshold;
	int      m_nCmdMaxLenThreshold;

public:
	CCmdHistoryParams(void);
	virtual ~CCmdHistoryParams(void);

	BOOL		 GetRemoveDuplicatesOnMenu() {return m_bRemoveDuplicatesOnMenu;}
	void		 SetRemoveDuplicatesOnMenu(BOOL bRemoveDuplicatesOnMenu) {m_bRemoveDuplicatesOnMenu = bRemoveDuplicatesOnMenu;}
	
	CString		 GetFilter() {return m_sFilter;}
	void		 SetFilter(CString sFilter) {m_sFilter = sFilter;}
	
	BOOL		 GetUseTerminalHistory() {return m_bUseTerminalHistory;}
	void		 SetUseTerminalHistory(BOOL bUseTerminalHistory) {m_bUseTerminalHistory = bUseTerminalHistory;}
	
	int		     GetMaxCmdCount() {return m_nMaxCmdCount;}
	void		 SetMaxCmdCount(int nMaxCmdCount) {m_nMaxCmdCount = nMaxCmdCount;}

	int		     GetCmdMinLenThreshold() {return m_nCmdMinLenThreshold;}
	void		 SetCmdMinLenThreshold(int nCmdMinLenThreshold) {m_nCmdMinLenThreshold = nCmdMinLenThreshold;}

	int		     GetCmdMaxLenThreshold() {return m_nCmdMaxLenThreshold;}
	void		 SetCmdMaxLenThreshold(int nCmdMaxLenThreshold) {m_nCmdMaxLenThreshold = nCmdMaxLenThreshold;}


};
