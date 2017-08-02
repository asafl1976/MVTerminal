#pragma once
#include "ICmdExeView.h"
#include "ICmdExeViewSubject.h"
#include <vector>
using namespace std;

class CCmdExeCmd;
class CCmdExeItem;

class CCmdExeTreeView : public CXTTreeCtrl, public ICmdExeView, public ICmdExeViewSubject
{
	CImageList      	  m_cImageList;

	virtual HTREEITEM	  GetTreeItem(CCmdExeItem* pCmdExeItem);
	virtual void		  FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem);
	virtual void		  DeleteItemChilds(HTREEITEM tItem);
	virtual BOOL		  UpdateCommand(HTREEITEM tItem, CCmdExeCmd *pCommand);
public:
	CCmdExeTreeView(void);
	virtual ~CCmdExeTreeView(void);
	virtual void CreateImageList();
	virtual int  GetCommandCount();

	//Triggered by presenter
	virtual BOOL OnUpdateCommand(CCmdExeCmd *pCommand);
	virtual BOOL OnAddCommand(CCmdExeCmd *pCommand);
	virtual BOOL OnRemoveCommand(CCmdExeCmd *pCommand);

	//Triggered by user
	virtual BOOL LoadCommands(CMarkup& xmlReader);
	virtual BOOL SaveCommands(CMarkup& xmlWriter);
	virtual BOOL UpdateCommand(CCmdExeCmd *pCommand);
	virtual BOOL RemoveCommand(CCmdExeCmd *pCommand);
	virtual BOOL RemoveAllCommands();
	virtual BOOL AddCommand(CCmdExeCmd *pCommand);

};
