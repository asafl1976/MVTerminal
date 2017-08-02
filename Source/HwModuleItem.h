#pragma once
#include "HwModuleInfo.h"
#include "Markup.h"
#include <vector>
#include <list>
using namespace std;

class CHwModuleItem;
class CCmdExeItemContainer;
class CHwModule;
class CDragDropTreeCtrl;

class CHwModuleItem 

{	
protected:
	CString			  m_sName;
	CString			  m_sDisplayName;
	CString			  m_sID;
	CHwModuleItem*    m_pOwner;

public:
	CHwModuleItem(void);
	CHwModuleItem(CString sName, CString sID, CHwModuleItem* pOwner);
	virtual ~CHwModuleItem(void);

	virtual CString					GetName()                       {return m_sName;}
	virtual CString					GetDisplayName()                {return m_sDisplayName;}
	virtual CHwModuleItem*			GetOwner()		                {return m_pOwner;}
	virtual CString					GetID()			                {return m_sID;}
	virtual BOOL					Init()			                {return TRUE;}
	virtual void	                OnSelect()		                {}
	virtual void	                OnDeSelect()	                {}
	virtual CString					Serialize()		                {return CString("");}
	virtual BOOL					DeSerialize(CMarkup& xmlReader) {return TRUE;}

	virtual CString					SerializeOwnerList();
	virtual void					DeSerializeOwnerList(CMarkup& xmlReader, list<CHwModuleItem>& ownerList);


};

