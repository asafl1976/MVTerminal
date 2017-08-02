#pragma once


#include "IHwModuleView.h"
#include "IHwModuleViewSubject.h"
#include <map>
#include <vector>
using namespace std;

class CHwModuleItem;
class CBasicItemDlg;
class CHwModulesPane;
class CHwModuleCtrlUnitItem;
// CHWMTreeCtrlView

#define  ID_HW_MODULE_CTRL_MENU_SAVE		 (WM_USER+30)
#define  ID_HW_MODULE_CTRL_MENU_LOAD		 (WM_USER+31)
#define  ID_HW_MODULE_CTRL_MENU_REFRESH      (WM_USER+32)
#define  ID_HW_MODULE_CTRL_MENU_RESET_SUB20  (WM_USER+33)
#define  ID_HW_MODULE_CTRL_UNIT_I2C_SET_FREQ (WM_USER+34)

class CHwModuleCtrlMenuItem
{
public:
	CString m_sMenuItem;
	int		m_nMenuItemID;
	int		m_nMenuItemImage;
};

class CHwModuleTreeView : public CXTTreeCtrl, public IHwModuleView, public IHwModuleViewSubject
{
	CHwModulesPane				         *m_pHwModulesPane;
	bool				                  m_bDetectHwModulesOnStartup;
	CImageList						      m_cImageList;

	map<CHwModuleItem*,CBasicItemDlg*>          m_HwModuleDlgMap;
	map<CString,vector<CString>*>		        m_HwModuleMenuReplaceItemsMap;
	vector<CHwModuleCtrlMenuItem>		        m_HwModuleCtrlMenuItems;
	map<CString,vector<CHwModuleCtrlMenuItem>*>	m_HwModuleCtrlUnitMenuItems;

	virtual CHwModuleItem*			      GetHwModuleItem(HTREEITEM tItem);
	virtual BOOL				          IsStringMenuExists(CString sStrMenu, vector<CString>* pVec);
	virtual CBasicItemDlg*	              GetHwModuleItemDlg(CHwModuleItem *pObj);
	virtual LRESULT						  OnHwModuleModelessDlgDestroyed(WPARAM a, LPARAM b);
	virtual LRESULT						  OnHwModuleModelessDlgDisplayNameChange(WPARAM a, LPARAM b);
	virtual LRESULT						  OnHwModuleModelessDlgUnitItemDisplayNameChange(WPARAM a, LPARAM b);
	virtual LRESULT						  OnHwModuleSelect(WPARAM a, LPARAM b);
	virtual LRESULT						  OnHwModuleDeSelect(WPARAM a, LPARAM b);
	virtual void						  ExpandTree();
	virtual void					      ExpandTree(HTREEITEM tiStartItem);
	virtual BOOL						  RemoveHwModuleTreeItem(CHwModuleItem *pObj);
	virtual BOOL						  DeleteRecursiveTreeItems(CHwModuleCtrl *pObj);
	virtual void						  FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem);
	virtual HTREEITEM					  GetTreeItem(CHwModuleItem* pHwModuleItem);
protected:

public:
	CHwModuleTreeView();
	virtual ~CHwModuleTreeView();
	virtual BOOL InitHWMTreeCtrl();
	virtual void				           CreateImageList();
	virtual void						   SetParentWindow(CHwModulesPane *pHwModulesPane) {m_pHwModulesPane = pHwModulesPane;}
	virtual vector<CHwModuleCtrlMenuItem>* GetHwModuleCtrlUnitMenuItems(CHwModuleCtrlUnit *pObj);
	virtual vector<CString>*			   GetHwModuleMenuReplaceItems(CHwModule *pObj);
	virtual vector<CHwModuleCtrlMenuItem>* GetHwModuleCtrlMenuItems(CHwModuleCtrl *pObj);
	virtual void						   ShowHwModuleItemDlg(CHwModuleItem *pObj);
	
	virtual BOOL OnHwModuleCtrlUnitItemDisplayNameChange(CHwModuleCtrlUnitItem* pItem); 
	virtual void OnAddHwModuleItem(CHwModuleCtrlUnitItem *pObj, CHwModuleItem *pParentObj);
	virtual void OnAddHwModuleItem(CHwModule *pObj        , CHwModuleItem *pParentObj);
	virtual void OnAddHwModuleItem(CHwModuleCtrl *pObj    , CHwModuleItem *pParentObj);
	virtual void OnAddHwModuleItem(CHwModuleCtrlUnit *pObj, CHwModuleItem *pParentObj);
	virtual BOOL OnHwModuleDisplayNameChange(CHwModule *pObj);
	virtual BOOL OnSelectHwModule(CHwModule *pObj);
	virtual BOOL OnDeSelectHwModule(CHwModule *pObj);
	virtual BOOL OnRemoveHwModule(CHwModule *pObj);
	virtual BOOL OnRemoveHwModuleCtrl(CHwModuleCtrl *pObj);
	virtual BOOL OnReplaceHwModule(CHwModule* pObj,CHwModule *pNewObj);

	//Requests sent to the model
	virtual BOOL RefreshHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL ResetHwModuleCtrl(CHwModuleCtrl *pCtrl);
	
	virtual BOOL AddUsbDeviceMsg(CString sDevId);
	virtual BOOL SetHwModuleDisplayName(CHwModule *pObj,CString sDisplayName);
	virtual BOOL RemoveUsbDeviceMsg(CString sDevId);
	virtual BOOL ReplaceHwModule(CHwModule *pHwModule, CString sNewHwModuleName);
	virtual BOOL RemoveHwModule(CHwModule *pObj);
	virtual BOOL SelectHwModule(CHwModule *pObj);
	virtual BOOL DeSelectHwModule(CHwModule *pObj);
	virtual BOOL SaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	virtual BOOL LoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	
	
	DECLARE_MESSAGE_MAP()

};


