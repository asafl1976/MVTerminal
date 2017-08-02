#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "CmdHistoryParams.h"
#include "Markup.h"
// CCmdHistorySettings dialog

class CCmdHistorySettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CCmdHistorySettingsDlg)

	CCmdHistoryParams m_cmdHistoryParams;

public:
	CCmdHistorySettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdHistorySettingsDlg();
	
	CCmdHistoryParams & GetCmdHistoryParams() { return m_cmdHistoryParams;}
	void                SetCmdHistoryParams(CCmdHistoryParams cmdHistoryParams) { m_cmdHistoryParams = cmdHistoryParams;}

	int		 GetMaxHistoryCount()                {return m_cmdHistoryParams.GetMaxCmdCount();}
	void     SetMaxHistoryCount(int nCount)      {m_cmdHistoryParams.SetMaxCmdCount(nCount);}
	void     SetFilter(CString sFilter)          {m_cmdHistoryParams.SetFilter(sFilter);}
	CString  GetFilter()                         {return m_cmdHistoryParams.GetFilter();}
	int		 GetCmdMinLenThreshold()             {return m_cmdHistoryParams.GetCmdMinLenThreshold();}
	void     SetCmdMinLenThreshold(int nLen)     {m_cmdHistoryParams.SetCmdMinLenThreshold(nLen);}
	int		 GetCmdMaxLenThreshold()             {return m_cmdHistoryParams.GetCmdMaxLenThreshold();}
	void     SetCmdMaxLenThreshold(int nLen)     {m_cmdHistoryParams.SetCmdMaxLenThreshold(nLen);}
	void	 SetUseTerminalHistory(BOOL bUse)    {m_cmdHistoryParams.SetUseTerminalHistory(bUse);}
	BOOL	 GetUseTerminalHistory()			 {return m_cmdHistoryParams.GetUseTerminalHistory();}
	BOOL	 GetRemoveDuplicatesOnMenu()		 {return m_cmdHistoryParams.GetRemoveDuplicatesOnMenu();}

	void	 UpdateWindowData();
	CString	 Serialize();
	BOOL	 DeSerialize(CMarkup & xmlReader);



// Dialog Data
	enum { IDD = IDD_CMD_HISTORY_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClearHistoryBtn();
private:
	CXTPButton m_clearHistoryBtn;
public:
	afx_msg void OnBnClickedSendKeyCodesAsIs();
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedActAsHistory();
	virtual void OnOK();

	afx_msg void OnBnClickedRemDupStringsMenuCheck();
};
