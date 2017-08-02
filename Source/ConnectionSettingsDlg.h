#pragma once

//#include "MSComm.h"
#include "sportax1.h"

#include "ConnectionParams.h"
#include "Markup.h"
#include "Resource.h"

#include <vector>
using namespace std;

// CPortPropetiesDlg dialog

class CConnectionSettingsDlg : public CDialog
{
private:

	DECLARE_DYNAMIC(CConnectionSettingsDlg)

	BOOL			  m_bValid;
	BOOL			  m_bPortAlreadyOpen;
	int			      m_nDlgType;
	CXTPButton		  m_restoreDefaultBtn;
	CComboBox	      m_baudRateCombo;
	CComboBox	      m_comPortCombo;
	CComboBox	      m_dataBitsCombo;
	CComboBox	      m_parityCombo;
	CComboBox	      m_stopBitsCombo;
	CSportax1         m_SPCommCtrl;
	CConnectionParams m_connectionParams;
	vector<CString>   m_comPortVec;


public:

	CConnectionSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectionSettingsDlg();
	void	       UpdateWindowData();
		
	CString			GetFirstCOMPort(vector<CString>& ComPortVec);
	CConnectionParams & GetConnectionParams() {return m_connectionParams;}
	void		        SetConnectionParams(CConnectionParams connectionParams) {m_connectionParams = connectionParams;}
	BOOL		   IsPortAlreadyOpen() {return m_bPortAlreadyOpen;}
	int		       GetDlgType();
	void           SetDlgType(int nType);
	BOOL           IsValid(){return m_bValid;}

	CSportax1&	   GetTestSPCommCtrl() {return m_SPCommCtrl;}
	CString		   Serialize();
	BOOL		   DeSerialize(CMarkup & xmlReader);

	virtual  void  OnOK();

// Dialog Data
	enum { IDD = IDD_CONNECTION_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeBaudRateCombo();
	afx_msg void OnBnClickedRestoreDefaultBtn();
};
