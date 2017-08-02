#pragma once
#include "afxwin.h"
#include "HListBox.h"
#include "TextFile.h"
#include "Markup.h"
#include "SyntaxHighlightEdit.h"


#include <list>
using namespace std;

class CMainFrame;
class CChildFrame;
class CMVTerminalView;

// CSendScriptPane form view

class CSendScriptPane : public CFormView
{
	DECLARE_DYNCREATE(CSendScriptPane)

	CString          m_sDefaultScriptFolder;
	BOOL             m_bLoadDefaultScripts;
	CString          m_sAppDataPath;
	list<CTextFile>  m_scriptList;
	CFont*		     m_pScriptEditFont;


	void			 AddScript(CTextFile  script);
	void			 RemoveScript(CTextFile  *pScript);
	CTextFile*		 GetSelectedScript();
	CTextFile*		 GetScript(CString sScriptName);
	int    		     GetScriptCount() {return (int)m_scriptList.size();}
	CString			 GenerateScriptName();
	void			 ShowScript(CTextFile   *pScript);
	void			 EnableButtons(BOOL bEnable);
	CMVTerminalView* GetActiveView();
	LRESULT			 OnLoadScriptEnd(WPARAM a, LPARAM b);
	CTextFile		 ImportScript(CString sPath);
	BOOL			 ExportScript(CTextFile script, CString sDirPath);
	BOOL			 ExportScripts(CString sDirPath);
	void             ImportScripts(CString sDirPath);
	void			 CreateDefaultScripts();
	CString			 GetDefaultScriptsFolder();
	BOOL			 GetLoadDefaultScript();
	CString			 Serialize();
	BOOL			 DeSerialize(CMarkup & xmlReader);

public:
	void			 RemoveAllScripts();

protected:
	CSendScriptPane();           // protected constructor used by dynamic creation
	virtual ~CSendScriptPane();

public:
	enum { IDD = IDD_SEND_SCRIPT_PANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CHListBox m_scriptListCtrl;
	CXTPButton m_newScriptBtn;
	CXTPButton m_removeScriptBtn;
	CXTPButton m_loadScriptBtn;
public:
	virtual void OnInitialUpdate();
private:
	//CRichEditCtrl m_scriptEditCtrl;
	CSyntaxHighlightEdit m_scriptEditCtrl;
public:
	afx_msg void OnBnClickedNewScriptBtn();
	afx_msg void OnBnClickedRemoveScriptBtn();
	afx_msg void OnBnClickedLoadScriptBtn();
	afx_msg void OnEnKillfocusLineDelayEdit();
	afx_msg void OnEnKillfocusScriptNameEdit();
	afx_msg void OnEnKillfocusScriptView();
	afx_msg void OnLbnSelchangeScriptList();
private:
	CEdit m_lineDelayEditCtrl;
public:
	CEdit m_scriptNameEditCtrl;
	afx_msg void OnLbnDblclkScriptList();
private:
	CXTPButton m_browseBtn;
public:
	afx_msg void OnBnClickedBrowseBtn();

	class CImportScriptsParams
	{
		CSendScriptPane  *m_pScriptPane;
		CString			  m_sDirPath;
	public:
		CImportScriptsParams() {}
		void SetParams(CSendScriptPane *pScripPane, CString sDirPath)
		{
			m_pScriptPane  = pScripPane;
			m_sDirPath    = sDirPath;
		}
		CString            GetDirPath() {return m_sDirPath;}
		CSendScriptPane*   GetScriptPane() {return m_pScriptPane;}
	};
	CImportScriptsParams	m_importScriptParams;


protected:
};

UINT ImportScriptsThread(LPVOID pParam);


