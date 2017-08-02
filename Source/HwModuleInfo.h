#pragma once

#include "Markup.h"
class CHwModuleItem;
class CHwModuleCtrlUnit;
class CHwModuleCtrl;
class CHwModule;
class CDragDropTreeCtrl;
class CHwModuleCtrlUnitDrv;
class CMuxHwModule;

typedef enum _HWMIFType
{
	HWM_IF_I2C,
	HWM_IF_USB,
	HWM_IF_UNKNOWN
} HWM_IF_TYPE;

class CHwModuleInfo
{
protected:

	CString		          m_sName;
	CString		          m_sID;
	HWM_IF_TYPE           m_eIFType; 
	CHwModuleCtrlUnit*    m_pParentUnit;
	//Needed for identifing the hw module in model tree (up-chain)
	CHwModuleItem*        m_pOwner;

public:
	CHwModuleInfo( );
	CHwModuleInfo(CString sName,CString sID, HWM_IF_TYPE eIFType, CHwModuleCtrlUnit *pParentUnit, CHwModuleItem* pOwner);
	virtual ~CHwModuleInfo(void);
	virtual  HWM_IF_TYPE			   GetInterfaceType()					        {return m_eIFType;}
	virtual  CString				   GetID()										{return m_sID;}
	virtual  CString				   GetName()									{return m_sName;}
	virtual  CHwModuleCtrlUnit*        GetParentUnit()   							{return m_pParentUnit;}
	virtual  CHwModuleItem*            GetOwner()   						    	{return m_pOwner;}
};


