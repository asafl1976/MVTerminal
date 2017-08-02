#pragma once

class CCmdExeCmdItem;

class ICmdExeCmdItemDlg
{
public:
	ICmdExeCmdItemDlg(void);
	virtual ~ICmdExeCmdItemDlg(void);
	virtual  void SetCmdItem(CCmdExeCmdItem* pItem) = 0;
};
