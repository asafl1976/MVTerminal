// PlatformInfoSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVTerminal.h"
#include "PlatformInfoSettingsDlg.h"
#include "PlatformInfoManager.h"
#include "FileOperations.h"

#define  VK_CTRL_C		TCHAR(3)
#define  VK_CTRL_V		TCHAR(22)

// PlatformInfoSettingsDlg dialog

IMPLEMENT_DYNAMIC(CPlatformInfoSettingsDlg, CDialog)

CPlatformInfoSettingsDlg::CPlatformInfoSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlatformInfoSettingsDlg::IDD, pParent)
{
}

CPlatformInfoSettingsDlg::~CPlatformInfoSettingsDlg()
{

}

void CPlatformInfoSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_PLATFORMINFOSETTINGSTREE, m_tree);
	DDX_Control(pDX, IDC_GRID_PLACEHOLDER, m_wndPlaceHolder);
}


BEGIN_MESSAGE_MAP(CPlatformInfoSettingsDlg, CDialog)
		ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
		ON_NOTIFY(NM_CLICK, IDD_PLATFORMINFOSETTINGSTREE, &CPlatformInfoSettingsDlg::OnNMClickPlatformInfoSettingsTree)
		ON_BN_CLICKED(IDOK, &CPlatformInfoSettingsDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDC_SAVE_AS_DEFAULT_BTN, &CPlatformInfoSettingsDlg::OnBnClickedSaveAsDefaultBtn)
		ON_BN_CLICKED(IDC_RESTORE_DEFAULTS_BTN, &CPlatformInfoSettingsDlg::OnBnClickedRestoreDefaultsBtn)
END_MESSAGE_MAP()

BOOL CPlatformInfoSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// get the size of the place holder, this will be used when creating the grid.
	CRect rc;
	m_wndPlaceHolder.GetWindowRect( &rc );
	ScreenToClient( &rc );

	// create the property grid.
	if ( m_wndPropertyGrid.Create( rc, this, IDC_PROPERTY_GRID ) )
	{
		m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
		//LOGFONT lf;
		//GetFont()->GetLogFont( &lf );
	}

	m_cImageList.Create(16,16, ILC_COLOR32 , 5, 0);
	CBitmap bmpImage;

	bmpImage.LoadBitmap(IDB_CE_CMD_BMP); //0
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_SWMODULE_BMP); //1
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_BOARD_BMP); //2
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_HWMODULE_BMP); //3
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_DEVICE_BMP);      //4
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_UNIT_DIS_BMP);       //5
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_UNIT_EN_BMP);       //6
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	bmpImage.LoadBitmap(IDB_PI_SUBUNIT_BMP);   //7
	m_cImageList.Add(&bmpImage, (COLORREF)NULL);
	bmpImage.DeleteObject();

	m_tree.SetImageList(&m_cImageList, TVSIL_NORMAL);

	InitPlatformInfoTree(m_pPlatformInfo);

	BSTR	  bstr = NULL;
	m_pPlatformInfo->GetDisplayName(&bstr); 
	_bstr_t tmp(bstr, FALSE);  
	CString sName(static_cast<const char*>(tmp));  
	SetWindowText(sName);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlatformInfoSettingsDlg::SetTreeItemEnabled(IPlatformInfoItem* pSubItem, HTREEITEM tiItem)
{
	CFont* treeFont = m_tree.GetFont();
	LOGFONT treeLogFont;
	treeFont->GetLogFont(&treeLogFont);
	string displayName = CPlatformInfoManager::GetInstance().GetItemAttributeValue(pSubItem,"DisplayName");
	CString sDisplayName = displayName.c_str();
	VARIANT_BOOL retVal;
	pSubItem->IsEnabled(&retVal);
	if(retVal == 0)
	{
		treeLogFont.lfItalic = TRUE;
  	    m_tree.SetItemFont(tiItem,treeLogFont);
		m_tree.SetItemColor(tiItem,RGB(100, 100, 100));
		if(sDisplayName.Find("Unit - ",0) != -1)
		{
			m_tree.SetItemImage(tiItem,5,5);
		}
	}
	else
	{
		treeLogFont.lfItalic = FALSE;
        m_tree.SetItemFont(tiItem,treeLogFont);
	    m_tree.SetItemColor(tiItem,RGB(0, 0, 0));
		if(sDisplayName.Find("Unit - ",0) != -1)
		{
			m_tree.SetItemImage(tiItem,6,6);
		}
	}
}


// PlatformInfoSettingsDlg message handlers
void CPlatformInfoSettingsDlg::AddSubItems(IPlatformInfoItem *pPlatformInfoItem, HTREEITEM tiParent)
{
	HTREEITEM tiItem, tiParentItem = tiParent;
	long nItem = 0,nSubItemCount = 0; 
		
	pPlatformInfoItem->GetSubItemCount(&nSubItemCount);
	for(nItem = 0; nItem < nSubItemCount; nItem++)
	{
		IPlatformInfoItem* pSubItem;
		pPlatformInfoItem->GetSubItemAtIndex(nItem,&pSubItem);

		if(CPlatformInfoManager::GetInstance().IsSubitemFiltered(pSubItem) == TRUE)
		{
			continue;
		}

		string sDisplayName = CPlatformInfoManager::GetInstance().GetItemAttributeValue(pSubItem,"DisplayName");

		CString sName = sDisplayName.c_str();  
		int image = 7;
		if(sName.Find("Board - ",0) != -1)
		{
			image = 2;
		}
		if(sName.Find("Software Module - ",0) != -1)
		{
			image = 1;
		}
		if(sName.Find("Hardware Module - ",0) != -1)
		{
			image = 3;
		}
		if(sName.Find("Device - ",0) != -1)
		{
			image = 4;
		}
		if(sName.Find("Unit - ",0) != -1)
		{
			VARIANT_BOOL retVal;
			pSubItem->IsEnabled(&retVal);
			if(retVal == 0)
			{
				image = 5;				
			}
			else
			{
				image = 6;
			}
		}

		if((tiItem = m_tree.InsertItem(sDisplayName.c_str(),image,image,tiParentItem)) != NULL)
		{
			SetTreeItemEnabled(pSubItem,tiItem);
			m_tree.SetItemData(tiItem,(DWORD_PTR)pSubItem);
			AddSubItems(pSubItem,tiItem); 
		}

		if((sName.Find("Device - ",0) != -1) || (sName.Find("Board - ",0) != -1))
		{
			m_tree.Expand(tiItem,TVE_EXPAND);
			m_tree.SetItemBold(tiItem);
		}
	}
}

void CPlatformInfoSettingsDlg::InitPlatformInfoTree(IPlatformInfoItem* pPlatformInfo)
{
	HTREEITEM tiItem, tiParentItem = TVI_ROOT;

	m_tree.DeleteAllItems();

	BSTR	  bstr = NULL;
	pPlatformInfo->GetDisplayName(&bstr); 
	_bstr_t tmp(bstr, FALSE);  
	CString sName(static_cast<const char*>(tmp));  

	if((tiItem = m_tree.InsertItem(sName,0,0,tiParentItem)) != NULL)
	{
		m_tree.SetItemBold(tiItem);
		m_tree.SetItemData(tiItem,(DWORD_PTR)pPlatformInfo);
		AddSubItems((IPlatformInfoItem*)pPlatformInfo,tiItem);
	}
	m_tree.Expand(tiItem,TVE_EXPAND);
}

HTREEITEM CPlatformInfoSettingsDlg::GetMouseClickedItem()
{
     CPoint     point;
	 UINT       uFlags;
     HTREEITEM  item = NULL;

	 GetCursorPos(&point);   
	 m_tree.ScreenToClient(&point);
	 item = m_tree.HitTest(point, &uFlags);
	 if(item)
	 { 
         // There is an item under the cursor. 
		 // See what exactly was under the cursor:
		 switch(uFlags)
		 {
		 case TVHT_ONITEMSTATEICON:
	 		break;
	  	 case TVHT_ONITEMBUTTON:
			break;
		 case TVHT_ONITEMLABEL:
			return item;
		 default:
			break;
	   	}
	 }	
	 return NULL;
}


LRESULT CPlatformInfoSettingsDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_EDIT_CHANGED)
	{		
		CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
		if (pEdit && pEdit->GetItem())
		{
			CString textStr;
			pEdit->CEdit::GetWindowText(textStr);
			
			textStr.MakeUpper();
			pEdit->SetSel(0, -1);
			pEdit->ReplaceSel(textStr.Left(textStr.GetLength()));

			//Validate hex string
			if(textStr.GetLength() > 0)
			{
    			switch(textStr.GetAt(textStr.GetLength() - 1))
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7': 
					case '8':
					case '9':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
					case VK_CTRL_C:
					case VK_CTRL_V:
					case VK_DELETE:
					case VK_BACK:
						break;
					default:
						MessageBeep((UINT)-1);
						pEdit->SetSel(0, -1);
						pEdit->ReplaceSel(textStr.Left(textStr.GetLength() - 1));
						return 0;
				}
			}
			//Make sure its 8 char length
			if (textStr.GetLength() > 8)
			{
				MessageBeep((UINT)-1);
				pEdit->SetSel(0, -1);
				pEdit->ReplaceSel(textStr.Left(8));
			}
		}
		return 0;
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		IPlatformInfoItemAttribute *pAttribute = (IPlatformInfoItemAttribute *)pItem->GetItemData();
		if(pAttribute != NULL)
		{
			string sType = CPlatformInfoManager::GetInstance().GetItemAttributeType(pAttribute);

			if (sType == "Edit")
			{
				string strValue = (LPCSTR)pItem->GetValue();
				_bstr_t _valuebstr( strValue.c_str() );
				BSTR valueBstr = _valuebstr.copy();
				pAttribute->SetValue(valueBstr);
				::SysFreeString( valueBstr );
				return 0;
			}
			if(sType == "Combo")
			{
				int nValIndex = 0;
				CString sValIndex = pItem->GetValue();
				sscanf((LPCSTR)sValIndex,"%d",&nValIndex);

				BSTR  bstr = NULL;
				pAttribute->GetValueAtIndex(nValIndex,&bstr);
				_bstr_t tmp(bstr, FALSE);  
				CString sValue(static_cast<const char*>(tmp));  
				::SysFreeString( bstr );

				string strValue = (LPCSTR)sValue;
				_bstr_t _valuebstr( strValue.c_str() );
				BSTR valueBstr = _valuebstr.copy();
				pAttribute->SetValue(valueBstr);
				::SysFreeString( valueBstr );
			}
		}
	}
	SetTreeItemEnabled(m_pSelecetedPlatformInfoItem,m_selecetedTreeItem);
	return 0;
}


void CPlatformInfoSettingsDlg::LoadProperties(IPlatformInfoItem	*pPlatformInfoItem)
{
	m_wndPropertyGrid.ResetContent();

	long attributeCount = 0;
	pPlatformInfoItem->GetAttributeCount(&attributeCount);

	BSTR	  bstr = NULL;
	pPlatformInfoItem->GetDisplayName(&bstr); 
	_bstr_t tmp(bstr, FALSE);  
	CString sName(static_cast<const char*>(tmp));  
	::SysFreeString( bstr );

	CXTPPropertyGridItem* pRootItem = m_wndPropertyGrid.AddCategory(_T((LPCSTR)sName));
	m_wndPropertyGrid.SetTheme(xtpGridThemeNativeWinXP);
	pRootItem->SetTooltip(_T((LPCSTR)sName));	

	for(int attribute = 0; attribute < attributeCount; attribute++)
	{
		IPlatformInfoItemAttribute *pAttribute;
	    pPlatformInfoItem->GetAttributeAtIndex(attribute,&pAttribute);

		string sName     = CPlatformInfoManager::GetInstance().GetItemAttributeName(pAttribute);
		string sCurValue = CPlatformInfoManager::GetInstance().GetItemAttributeValue(pPlatformInfoItem,sName);
		string sDesc     = CPlatformInfoManager::GetInstance().GetItemAttributeDescription(pAttribute);

		if((sName == "DisplayName") || (sName == "ID"))
			continue; //Skip DisplayName and ID attributes

		int nValuesCount = CPlatformInfoManager::GetInstance().GetItemAttributeValueCount(pAttribute);

		if(nValuesCount == 0)
		{
			CXTPPropertyGridItem* pItemName = pRootItem->AddChildItem(new CXTPPropertyGridItem(_T(sName.c_str()), _T(sCurValue.c_str())));
			pItemName->SetReadOnly(TRUE);
			if(sDesc.length() != 0)
			{
				pItemName->SetDescription(sDesc.c_str());
			}
			pItemName->SetItemData((DWORD_PTR)pAttribute);
		}
		else
		{
			string sType = CPlatformInfoManager::GetInstance().GetItemAttributeType(pAttribute);
			if(sType == "ReadOnly")
			{
				CXTPPropertyGridItem* pItemName = pRootItem->AddChildItem(new CXTPPropertyGridItem(_T(sName.c_str()), _T(sCurValue.c_str())));
				pItemName->SetReadOnly(TRUE);
				if(sDesc.length() != 0)
				{
					pItemName->SetDescription(sDesc.c_str());
				}
				pItemName->SetItemData((DWORD_PTR)pAttribute);
			}
			if(sType == "Combo")
			{
				CXTPPropertyGridItem* pItem = pRootItem->AddChildItem(new CXTPPropertyGridItemOption(_T(sName.c_str()), 2));
				
				vector<string> valueVec;
				int curValueIndex = 0;
				for(int value = 0; value < nValuesCount; value++)
				{
					string sValue = CPlatformInfoManager::GetInstance().GetItemAttributeValueAtIndex(pAttribute,value);
					pItem->GetConstraints()->AddConstraint(_T(sValue.c_str()), value);
					if(sCurValue == sValue)
					{
						CString strVal;
						strVal.Format("%d",value);
						pItem->SetValue(strVal);
						//pItem->SetValue(sCurValue.c_str());
					}
				}
				if(sDesc.length() != 0)
				{
					pItem->SetDescription(sDesc.c_str());
				}
				pItem->SetItemData((DWORD_PTR)pAttribute);
			}
			if(sType == "Edit")
			{
				CXTPPropertyGridItem* pItem = pRootItem->AddChildItem(new CXTPPropertyGridItem(_T(sName.c_str()), _T(sCurValue.c_str())));
				pItem->SetReadOnly(FALSE);
				if(sDesc.length() != 0)
				{
					pItem->SetDescription(sDesc.c_str());
				}
				pItem->SetItemData((DWORD_PTR)pAttribute);
			}
		}
	}
	pRootItem->Expand();
}

void CPlatformInfoSettingsDlg::OnNMClickPlatformInfoSettingsTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM  item = GetMouseClickedItem();

	if(item != NULL)
	{
		m_selecetedTreeItem = item;
		 m_pSelecetedPlatformInfoItem = (IPlatformInfoItem*)m_tree.GetItemData(item);
		 if(m_pSelecetedPlatformInfoItem != NULL)
		 {
			 LoadProperties(m_pSelecetedPlatformInfoItem);
	     }
	 }

	*pResult = 0;
}



void CPlatformInfoSettingsDlg::OnBnClickedOk()
{
	OnOK();
}

void CPlatformInfoSettingsDlg::OnBnClickedSaveAsDefaultBtn()
{
	BSTR	  bstr = NULL;
	m_pPlatformInfo->GetDisplayName(&bstr); 
	_bstr_t tmp(bstr, FALSE);  
	CString sName(static_cast<const char*>(tmp));  
	CPlatformInfoManager::GetInstance().SavePlaformInfoXmlRepresentation((LPCSTR)sName);
	//AfxMessageBox( CPlatformInfoManager::GetInstance().GetPlaformInfoXmlRepresentation((LPCSTR)sName).c_str() );

	OnOK();
}

void CPlatformInfoSettingsDlg::OnBnClickedRestoreDefaultsBtn()
{
	CStdioFile  file;

	int res = AfxMessageBox("Restore XML Platform info file to original state (factory defaults)?",MB_YESNO|MB_ICONWARNING);
	if(res == IDNO)
		return;

	CString sSourcePath = "\\\\marvell\\israel\\groups\\eecv\\Utilities\\LabTools\\MVTerminal\\PlatformInfo\\ARM\\Devices";

	BSTR	  bstr = NULL;
	m_pPlatformInfo->GetDisplayName(&bstr); 
	_bstr_t tmp(bstr, FALSE);  
	CString sName(static_cast<const char*>(tmp)); 

	string sPlatformInfoXmlFilePath = CPlatformInfoManager::GetInstance().GetPlaformInfoXmlFilePath((LPCSTR)sName);
	CString sTargetPath = sPlatformInfoXmlFilePath.c_str();	

	sSourcePath += sTargetPath.Right(sTargetPath.GetLength() - sTargetPath.ReverseFind('\\') );

	CFileOperation fo;      
	fo.SetOverwriteMode(true); 
	fo.DoFileCopy(sSourcePath, sTargetPath);
	CPlatformInfoManager::GetInstance().ReloadData();
	m_delegate->RestoreDefaultsClicked();
	InitPlatformInfoTree(m_pPlatformInfo);
	OnOK();
}
