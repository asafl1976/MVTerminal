#pragma once

#include <vector>
#include <map>
using namespace std;

class CCmdExeItem;

class CCmdExeItemContainer
{
protected:

	CString			          m_sName;
	CString			          m_sDisplayName;
	map<CString,CCmdExeItem*> m_cmdItems;

public:
	CCmdExeItemContainer(CString sName);
	virtual			   ~CCmdExeItemContainer();

	virtual CString	     		  GetName();
	virtual CString	     		  GetDisplayName() {return  m_sDisplayName;}
	virtual void	     		  SetDisplayName(CString sDisplayName);
	virtual int          		  GetCmdItemCount();
	virtual void         		  AddCmdItem(CCmdExeItem *pItem);
	virtual void		 		  GetCmdItemNameList(vector<CString>& cmdItemList);
	virtual CCmdExeItem* 		  CreateCmdItem(CString sName);
	virtual CCmdExeItemContainer* Clone();
};


