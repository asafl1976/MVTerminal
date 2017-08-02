#pragma once




// CSendEmailDlg dialog

class CSendEmailDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendEmailDlg)

public:
	CSendEmailDlg();
	virtual ~CSendEmailDlg();

// Dialog Data
	enum { IDD = IDD_SEND_EMAIL };

protected:	

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
