#pragma once
#include "hwmodule.h"
#include "IHwModuleContainer.h"
#include "Markup.h"

class CMuxHwModuleChannel : public CHwModule, public IHwModuleContainer
{
	int                  m_nChannel;
	CMuxHwModule*	     m_pMux;

	virtual void            NotifyCreateHwModule(CHwModule* pHwModule);
	virtual BOOL		    UpperChannelIsHwModuleExists(CString sID);

public:
	CMuxHwModuleChannel(int nChannel, CMuxHwModule *pMux);
	virtual     		~CMuxHwModuleChannel(void);

	virtual CString		    Serialize();
	virtual BOOL		    DeSerialize(CMarkup& xmlReader);

	virtual BOOL 		    Select(); //Set this channel to active
	virtual int 		    GetChannelNumber() {return m_nChannel;}

	virtual BOOL		    DetectHwModules();
	virtual BOOL		    DetectHwModules(vector<CHwModuleInfo*>& HwModuleInfo,CHwModuleItem* pOwner);	
	virtual BOOL		    CreateHwModules(vector<CHwModuleInfo*>& HWMInfo);

	//Overrides
	virtual BOOL		    SelectHwModule(CHwModule *pHwModule);
	virtual BOOL		    DeSelectHwModule(CHwModule *pHwModule);

	virtual CHwModule*      Clone(CHwModuleInfo* pInfo) {return NULL;}
};
