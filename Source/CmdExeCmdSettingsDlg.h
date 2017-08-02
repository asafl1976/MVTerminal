#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include "DragDropTreeCtrl.h"
#include "CmdExeItemContainer.h"
#include "CmdExeCmd.h"

class CCmdExePane;
class CCmdExeCmd;
class CCmdExeItem;
class CCmdExeCmdItem;
class CHwModuleItem;

// CCmdExeCmdSettingsDlg dialog
class CCmdItemClipboard
{
public:
	bool	      m_bIsEmpty;
	TVITEM		  m_tvItem;
    TCHAR		  m_szText[1024];
	CCmdExeItem  *m_pCmdItem; 
};

class CCmdExeCmdSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CCmdExeCmdSettingsDlg)

	CCmdItemClipboard		 		 m_CmdItemClipboard;
	CWnd*						     m_pParentWnd;
	BOOL                 	 		 m_bIsNewCommand;
	CCmdExeCmd                      *m_pCmd;
	
	CCmdExeItemContainer            *m_pGeneralCmdExeCmdItemContainer;
	//vector<CCmdExeItemContainer*>          m_HwModuleItemCmdItemContainerVec;

	CXTPButton						 m_okBtn;
	CXTPButton						 m_cancelBtn;
	CXTPButton						 m_browseBtn;
	CSpinButtonCtrl				     m_loopCountSpin;
	CEdit							 m_loopCountEdit;
	CDragDropTreeCtrl    			 m_templateCmdItemTreeView;
	CDragDropTreeCtrl    			 m_cmdTreeView;
	CImageList			 			 m_cImageList;

	void			     			 AlignWindow();
	virtual BOOL			         ShowCmdItemDlg(CCmdExeCmdItem *pCmdItem);
	virtual HTREEITEM    			 GetMouseClickedItem(NMHDR *pNMHDR, LRESULT *pResult);

	LRESULT			     			 OnDestTreeItemAdded(WPARAM a, LPARAM b) ;
	virtual	void		 			 HandleMenuSelection(UINT nMenuIndex);
	virtual	void		 			 CopyCmdItem(HTREEITEM hItem);
	virtual	void		 			 PasteCmdItem(HTREEITEM hInsAfterItem);
	virtual	void		 			 DeleteTreeItem(HTREEITEM item);
	virtual HTREEITEM				 CreateHwModuleItemContainer(CHwModuleItem* pItem, HTREEITEM hParent);
	virtual HTREEITEM				 AddHwModuleContainerCmdItemContainer(CHwModuleItem* pItem, HTREEITEM hParent);
public:
	CDragDropTreeCtrl& GetCmdTree()  {return m_cmdTreeView;}

	virtual void 					 InitCmdTreeView();
	virtual HTREEITEM			     AddCmdExeCmdItemContainer(CCmdExeItemContainer* pContainer, HTREEITEM tiParent);
	virtual void 					 InitTemplateCmdItemTreeView();	
	virtual void 					 InitCmdItemContainerList();
	virtual void 					 CreateImageList();

	CCmdExeCmdSettingsDlg(CCmdExeCmd* pCmd,CWnd* pParent);
	virtual ~CCmdExeCmdSettingsDlg();

// Dialog Data
	enum { IDD = IDD_CMD_EXE_CMD_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL					 OnInitDialog();
	afx_msg void					 OnBnClickedCmdeCancelBtn();
	afx_msg void					 OnBnClickedCeCmdEnCheck();
	afx_msg void					 OnBnClickedCmdeCmdStopOnErrCheck();
	virtual BOOL					 PreTranslateMessage(MSG* pMsg);
	afx_msg void				     OnBnClickedOk();
	afx_msg void OnNMDblclkCmdExeCmdSettingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickCmdExeCmdSettingTree(NMHDR *pNMHDR, LRESULT *pResult);
};
