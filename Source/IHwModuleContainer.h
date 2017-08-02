#pragma once
#include "Markup.h"
#include "HwModule.h"
#include <vector>
using namespace std;


class IHwModuleContainer
{
protected:

	vector<CHwModule*>  m_HwModules;

	virtual BOOL    	ReplaceHwModule(CHwModule *pItem, CHwModule* pNewItem);
	virtual void		NotifyRemoveHwModule(CHwModule* pHwModule);
	virtual void		NotifySelectHwModule(CHwModule* pHwModule);
	virtual void		NotifyDeSelectHwModule(CHwModule* pHwModule);
	virtual void		NotifyReplaceHwModule(CHwModule* pHwModule,CHwModule* pNewHwModule);
	virtual void		NotifyCreateHwModule(CHwModule* pHwModule)       = 0;
	virtual void		NotifyHwModuleDisplayNameChange(CHwModule* pHwModule);
public:
	IHwModuleContainer(void);
	virtual ~IHwModuleContainer(void);
	virtual void	            ReleaseHwModules();
	virtual BOOL				IsHwModuleExists(CString sID);            
	
	virtual CHwModule*			GetHwModule(int nIndex); 
	virtual CHwModule*			GetHwModuleByName(CString sName, BOOL bRecursive = FALSE);
	virtual CHwModule*			GetHwModuleByID(CString sID, BOOL bRecursive = FALSE);          

	virtual int     			GetHwModuleCount();            
	virtual CHwModuleItem*	    GetSubHwModuleItem(list<CHwModuleItem>& ownerList);
	virtual BOOL				IsHwModuleExists(CHwModule* pHwModule);            
	virtual IHwModuleContainer*	GetHwModuleContainer(CHwModule* pHwModule);
	virtual void				AddHwModule(CHwModule* pHwModule);
	virtual BOOL        		ReplaceHwModule(CHwModule* pHwModule, CString sNewItemName,CString sNewItemDisplayName = NULL);
	virtual BOOL				RemoveHwModule(CHwModule *pHwModule);
	virtual BOOL				SetHwModuleDisplayName(CHwModule *pHwModule, CString sDisplayName);
	virtual BOOL				SelectHwModule(CHwModule *pHwModule);
	virtual BOOL				DeSelectHwModule(CHwModule *pHwModule);
	virtual BOOL				CreateHwModules(vector<CHwModuleInfo*>& HWMInfo);
	virtual CString				Serialize();
	virtual BOOL				DeSerialize(CMarkup& xmlReader);
	virtual BOOL				DetectHwModules();
	virtual BOOL				DetectHwModules(vector<CHwModuleInfo*>& HWMInfo,CHwModuleItem* pOwner) = 0;
};
