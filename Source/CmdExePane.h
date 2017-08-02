#pragma once
#include "afxcmn.h"
#include "Markup.h"
#include "CmdExeCmd.h"
#include "CmdExeTreeView.h"
#include "CmdExeModel.h"
#include "CmdExePresenter.h"

#include "CmdExeCmdSettingsDlg.h"
#include "ExCtrls.h"
#include "NewMenu.h"
#include "HwModule.h"

#include <vector>
#include "afxwin.h"
#include "exctrls.h"
using namespace std;


// CCmdExePane form view

class CCmdExePane : public CFormView
{
	DECLARE_DYNCREATE(CCmdExePane)
	
	CCmdExeModel*         m_pCmdExeModel;
	CCmdExePresenter*     m_pCmdExePresenter;

	CCmdExeTreeView       m_cmdTreeView;
	vector<CString>       m_cmdNameVec;
	CCmdExeViewClipboard  m_CmdClipboard;

	CXTPButton			  m_newCmdBtn;
	CXTPButton			  m_removeCmdBtn;
	CXTPButton			  m_removeAllBtn;
	CXTPButton			  m_fileBtn;
	CXTPButton			  m_moreOptionsBtn;
	CXTPButton            m_executeBtn;
	CCmdExeLogView		  m_cmdLogView;

	BOOL				  m_bInfiniteExe;
	int					  m_nLoopCount;
	int					  m_nSelectedCmdItem;
	HTREEITEM			  m_currSelectedTreeState;
	CNewMenu			  m_itemMenu;
	

	//Copy/Paste menu operation
	virtual void		  ClearCmdClipboard();
	virtual void		  CopyCmd(HTREEITEM hItem);
	virtual void		  PasteCmd(HTREEITEM hInsAfterItem);

protected:
	CCmdExePane();           // protected constructor used by dynamic creation
	virtual ~CCmdExePane();

	virtual CCmdExeCmd*	  CreateNewCommand();
	virtual BOOL		  ShowCmdItemDlg(CCmdExeCmdItem *pCmdItem);
	virtual BOOL		  ShowCmdDlg(CCmdExeCmd *pCmd);
	virtual BOOL          PreTranslateMessage(MSG* pMsg);

public:
	enum { IDD = IDD_COMMAND_EXE_PANE };

#ifdef _DEBUG
	virtual void		  AssertValid() const;
	virtual void		  Dump(CDumpContext& dc) const;
#endif
	virtual void		  AddCmdName(CString sName);
	virtual void		  RemoveCmdName(CString sName);
	virtual void		  HandleMenuSelection(UINT nMenuIndex);

protected:
	virtual void		  DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	
	//Used by CMVTerminalApp to restore and save settings
	virtual void          RemoveAllCommands();
	virtual int           GetCommandCount();

	virtual void		  OnStartExecution();
	virtual BOOL		  ExecuteCommands();
	virtual void		  OnEndExecution();
	
	virtual void		  SetLoopCount(int nLoopCount);
	virtual void		  SetLoopIndex(int nLoopIndex);

	virtual BOOL		  IsCommandNameExists(CString sName);
	virtual CString		  GetNewCommandName();
	virtual void		  OnInitialUpdate();
	afx_msg void		  OnBnClickedCeNewCmdBtn();
	HTREEITEM			  GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void		  OnBnClickedCeRemoveBtn();
	afx_msg void		  OnBnClickedCeFileCmdBtn();
	afx_msg void		  OnBnClickedCeRemoveAllCmdBtn();
	afx_msg void		  OnBnClickedCeExecuteBtn();
	afx_msg void		  OnBnClickedCeInfiniteLoopCheck();
	afx_msg void		  OnEnChangeCeLoopCountEdit();
	afx_msg void		  OnBnClickedCeMoreOptionsBtn();

	virtual CString		  Serialize();
	virtual BOOL		  DeSerialize(CMarkup & xmlReader);
	virtual void		  EnableControls(BOOL bEn);

	virtual void		  LoadCommands();
	virtual void	      SaveCommands();


	afx_msg void OnNMDblclkCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickCeCmdTree(NMHDR *pNMHDR, LRESULT *pResult);
	
};


