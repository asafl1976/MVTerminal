#pragma once
#include "HwModuleItemCmdExeCmdItem.h"
#include "HwModuleCtrl.h"
#include "UnknownI2CHwModule.h"


class CCmdExeCmdItemUnknownI2CReadWrite : public CHwModuleItemCmdExeCmdItem
{
	CUnknownI2CHwModuleParams	      m_params;

public: 
	CCmdExeCmdItemUnknownI2CReadWrite();
	virtual ~CCmdExeCmdItemUnknownI2CReadWrite(void);

	virtual void			      OnDeleteObject();
	virtual void			      SetHwModuleItem(CHwModuleItem* pHwModule);

	virtual BOOL	    	      Serialize(CMarkup & xmlWriter);
	virtual BOOL	    	      DeSerialize(CMarkup & xmlReader);

	virtual BOOL			      OnStartExecution();
	virtual BOOL			      OnEndExecution();
	virtual BOOL			      Execute();
	virtual CCmdExeCmdItem*       Clone();
	virtual CUnknownI2CHwModuleParams GetParams() {return m_params;}
	virtual void				  SetParams(CUnknownI2CHwModuleParams params) {m_params = params;}

};
