#pragma once

#include "afxwin.h"
#include "resource.h"
#include "HwModuleBoardPowerCtrlStartegy.h"
#include <map>
#include "afxcmn.h"
using namespace std;

class CHwModuleBoardPowerCtrlPowerStartegy;
class CHwModuleBoardPowerCtrlResetStartegy;
class IHwModuleContainer;
class CHwModuleCtrlUnit;
class CHwModuleCtrl;

class CBoardPowerCtrlSettingDlg : 	public CDialog
{
	DECLARE_DYNAMIC(CBoardPowerCtrlSettingDlg)

	CComboBox	    m_powerIOInitialStateCombo;
	CComboBox		m_resetIOInitialStateCombo;
	CComboBox		m_powerIONumCombo;
	CComboBox		m_resetIONumCombo;

	CString			m_sAppDataPath;
	CImageList	    m_cImageList;
	CHwModuleItem*  m_pResetHwModuleItem;
	CHwModuleItem*  m_pPowerHwModuleItem;

public:
	CBoardPowerCtrlSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBoardPowerCtrlSettingDlg();
	afx_msg void  OnOk();	
	virtual void  CreateImageList();
	virtual void  InitHwModuleTreeView();
	virtual HTREEITEM GetMouseClickedItem(CTreeCtrl *pTreeCtrl, NMHDR *pNMHDR, LRESULT *pResult);
	virtual void AddHwModuleCtrlUnit(CTreeCtrl* pTreeCtrl, CHwModuleCtrlUnit* pUnit,HTREEITEM	hParent);
	virtual void AddHwModuleContainer(CTreeCtrl* pTreeCtrl, IHwModuleContainer* pHwModuleContainer,CString sName,HTREEITEM	hParent);
	virtual void AddHwModuleCtrl(CTreeCtrl* pTreeCtrl, CHwModuleCtrl* pCtrl);

	BOOL	 ImportSettingsFromFile();
	BOOL	 ExportSettingsToFile();

	virtual void	EnablePowerControls(BOOL bEnable);
	virtual void	EnableResetControls(BOOL bEnable);

// Dialog Data
	enum { IDD = IDD_APP_POWER_CTRL_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CTreeCtrl m_boardPowerCtrlTree;
	CTreeCtrl m_boardResetCtrlTree;

	afx_msg void OnNMClickBoardPowerHwmoduleTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickBoardResetHwmoduleTree(NMHDR *pNMHDR, LRESULT *pResult);
};
