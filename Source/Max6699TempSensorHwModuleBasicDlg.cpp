// Max6699TempSensorHwModuleBasicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "Max6699TempSensorHwModuleBasicDlg.h"


// CMax6699TempSensorHwModuleBasicDlg dialog

CMax6699TempSensorHwModuleBasicDlg::CMax6699TempSensorHwModuleBasicDlg(UINT nIDTemplate, CWnd* pParentWnd)
	: CBasicItemDlg(nIDTemplate,pParentWnd)
{
	int i = 0 ;
	int R = 0,G = 255,B = 0;

	for(; i < 70; i++)
	{	
		if(i > 10)
		{
			R = i*6;
			if(R >= 255)
			{
				R = 255;
				G = 255 - i*6;
				if(G <= 0)
					G = 0;
			}
		}
		colorMap[i] = RGB(R, G, B);
	}
}

CMax6699TempSensorHwModuleBasicDlg::~CMax6699TempSensorHwModuleBasicDlg()
{
}

void CMax6699TempSensorHwModuleBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_TEMP_LABEL_STATIC, m_tempTextLabel);
}


BEGIN_MESSAGE_MAP(CMax6699TempSensorHwModuleBasicDlg, CDialog)
END_MESSAGE_MAP()

BOOL CMax6699TempSensorHwModuleBasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CMax6699TempSensorHwModuleBasicDlg::AlignWindow();

	m_cancelBtn.SetTheme(xtpButtonThemeOffice2000);
	m_PushGraph.CreateFromStatic(IDC_TEMP_MONITOR_GRAPH_STATIC, this);	

	m_PushGraph.AddLine( TEMP_PUSHGRAPH_VAL_SAMPLE_LINE,   m_sampleValuelineColor);
	m_sampleValuelineColor = RGB(80, 189, 231);
	m_PushGraph.SetTextColor(m_sampleValuelineColor);
	m_PushGraph.SetLineColor(m_sampleValuelineColor,TEMP_PUSHGRAPH_VAL_SAMPLE_LINE);

	m_PushGraph.ShowGrid(true);
	m_PushGraph.SetLabelForMin( "0 C°" );
	m_PushGraph.SetLabelForMax( "127 C°" ); 
	m_PushGraph.ShowLabels( true);
	m_PushGraph.SetInterval( 4 );
	m_PushGraph.SetGridSize( 15 );
	m_PushGraph.SetPeekRange( 0, 120);	

	m_tempTextLabel.SubclassDlgItem(IDC_TEMP_LABEL_STATIC, this);
	m_tempTextLabel.SetTextColor(RGB(0, 255, 0));
	m_tempTextLabel.SetBkColor(RGB(0,0,0));
	m_font.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Verdana");
	m_tempTextLabel.SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMax6699TempSensorHwModuleBasicDlg::UpdateGraph()
{
	m_PushGraph.Update();
}





// CMax6699TempSensorHwModuleBasicDlg message handlers
