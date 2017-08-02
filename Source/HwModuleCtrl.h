#pragma once
#include "HwModuleInfo.h"
#include "HwModuleCtrlDrv.h"
#include "HwModuleItem.h"
#include "Markup.h"
#include <vector>
#include <list>
using namespace std;

class CHwModuleCtrlUnit;
class CHwModuleCtrl;
class IHwModuleContainer;
class CHwModuleCtrlUnitItem;

class CHwModuleCtrl : public CHwModuleItem
{
	
protected:
	vector<CHwModuleCtrlUnit*>  m_units;
	BOOL						m_bConnected;  
	CString					    m_sUniqueID;
	CString					    m_sVendorID;

public:
	CHwModuleCtrl(void);
	virtual ~CHwModuleCtrl(void);	
	virtual BOOL			       IsConnected(){return m_bConnected;}
	virtual void				   ReleaseUnits();

	virtual CString  			   GetVendorID()  {return m_sVendorID;}
	virtual CString  			   GetUniqueID()  {return m_sUniqueID;}
	virtual int					   GetUnitCount() {return (int)m_units.size();}
	virtual void			       AddUnit(CHwModuleCtrlUnit* pUnit);	
	virtual CHwModuleCtrlUnit*     GetUnit(int i);
	virtual CHwModuleCtrlUnit*     GetUnit(CString sName);	
	virtual CHwModuleItem*	       GetSubHwModuleItem(list<CHwModuleItem>& ownerList) ;

	virtual CHwModule*			   GetHwModule(CString sName);
	virtual CHwModuleCtrlUnitItem* GetHwModuleCtrlUnitItem(CString sUnitName,CString sName);
	
	virtual IHwModuleContainer*	   GetHwModuleContainer(CHwModule *pHwModule);
	virtual CHwModuleCtrlUnit* 	   GetHwModuleCtrlUnitItemUnit(CHwModuleCtrlUnitItem *pItem);
	virtual CString			   	   Serialize();
	virtual BOOL			   	   DeSerialize(CMarkup& xmlReader);
	
	virtual BOOL               	   Reset();
	virtual BOOL               	   DetectHwModules();
	virtual void				   ReleaseHwModules();

	virtual void				   EnableDriverErrorMessages(BOOL bEnable)				= 0;
	virtual BOOL			   	   IsTargetConnected()									= 0;
	virtual BOOL			   	   Connect()											= 0;
	virtual BOOL			   	   Disconnect()									    = 0;
};



 