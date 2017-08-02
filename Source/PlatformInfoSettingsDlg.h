#pragma once
#import  "C:\Projects\C# Projects\PlatformInfoLib\PlatformInfoLib\bin\release\PlatformInfoLib.tlb" named_guids raw_interfaces_only
using namespace PlatformInfoLib;

// CPlatformInfoSettingsDlg dialog

class IPlatformInfoSettingsDlgDelegate
{
public:
	virtual void RestoreDefaultsClicked() = 0;
};

class CPlatformInfoSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlatformInfoSettingsDlg)

	virtual void InitPlatformInfoTree(IPlatformInfoItem* pPlatformInfo);
	virtual void AddSubItems(IPlatformInfoItem *pPlatformInfoItem, HTREEITEM tiParent);
	virtual HTREEITEM GetMouseClickedItem();
	virtual void SetTreeItemEnabled(IPlatformInfoItem* pSubItem, HTREEITEM tiItem);

public:
	IPlatformInfoSettingsDlgDelegate* m_delegate;

	CPlatformInfoSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlatformInfoSettingsDlg();
	virtual void LoadProperties(IPlatformInfoItem *pPlatformInfoItem);

// Dialog Data
	enum { IDD = IDD_PLATFORMINFOSETTINGSDLG };

	CXTTreeCtrl          m_tree; 
	IPlatformInfoItem	*m_pSelecetedPlatformInfoItem;
	HTREEITEM			 m_selecetedTreeItem;
	IPlatformInfoItem	*m_pPlatformInfo;
	CStatic              m_wndPlaceHolder;
	CXTPPropertyGrid     m_wndPropertyGrid;
	CImageList			 m_cImageList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	afx_msg void OnNMClickPlatformInfoSettingsTree(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSaveAsDefaultBtn();
	afx_msg void OnBnClickedRestoreDefaultsBtn();
};
