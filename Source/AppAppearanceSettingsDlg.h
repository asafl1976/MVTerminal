#pragma once
#include "afxwin.h"


// CAppAppearanceSettingsDlg dialog

class CAppAppearanceSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAppAppearanceSettingsDlg)
	CString						 m_sAppTheme;
	XTPPaintTheme			     m_nAppTheme;
	XTPTabAppearanceStyle        m_nTabAppearanceStyle;
	XTPDockingPanePaintTheme     m_nPanesTheme;
	CString					     m_sAppDataPath;

public:
	CAppAppearanceSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAppAppearanceSettingsDlg();
	CString					    GetTheme() {return m_sAppTheme;}
	void						SetTheme(CString sTheme) ;

	XTPPaintTheme				GetAppThemeEnum(){return m_nAppTheme;}
	XTPDockingPanePaintTheme    GetPaneThemeEnum(){return m_nPanesTheme;}
	

	void		 OnOk();
	BOOL		 ImportSettingsFromFile();
	BOOL		 ExportSettingsToFile();

// Dialog Data
	enum { IDD = IDD_APP_APPEARANCE_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_themeList;
public:
	afx_msg void OnLbnSelchangeThemeList();
	virtual BOOL OnInitDialog();
};
