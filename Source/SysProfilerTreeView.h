#pragma once
#include <vector>
#include "SysProfilerEvent.h"
using namespace std;

class CSysProfilerTreeView : public CXTTreeCtrl
{
	CImageList      	  m_cImageList;
	CSysProfilerEvent*	  m_pSelEvent;

	virtual void		  FindTreeItem(DWORD_PTR itemData, HTREEITEM &tiFoundAt,HTREEITEM tiStartItem);
	virtual void		  ExpandTree();
	virtual void		  ExpandTree(HTREEITEM tiStartItem);
public:
	CSysProfilerTreeView(void);
	virtual ~CSysProfilerTreeView(void);
	virtual HTREEITEM	  GetTreeItem(CSysProfilerEvent *pSysProfilerEvent);
	virtual void HandleEventCheck(HTREEITEM ti);
	virtual void CreateImageList();
	virtual int  GetEventCount();
	virtual BOOL AddEvent(CSysProfilerEvent *pSysProfilerEvent);
	virtual void RemoveAllEvents();
	virtual void SetSelectedEvent(CSysProfilerEvent *pSysProfilerEvent);
	virtual CSysProfilerEvent* GetSelectedEvent();
};
