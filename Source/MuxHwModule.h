#pragma once
#include "HwModule.h"
#include "IHwModuleContainer.h"
#include "Markup.h"
#include <vector>
using namespace std;

class CMuxHwModuleAccessStartegy;
class CMuxHwModuleChannel;

class CMuxHwModule : 	public CHwModule, public IHwModuleContainer
{
protected:

	vector<CMuxHwModuleChannel*>  m_MuxChannels;
	CMuxHwModuleAccessStartegy*	  m_pAccessStrategy;
	int							  m_nChannelCount;
	int							  m_nActiveChannel;
	CMuxHwModuleChannel			 *m_pUpperMuxChannel; //NULL if not connected under a mux channel

	virtual void				  NotifyCreateHwModule(CHwModule* pHwModule);

public:
	CMuxHwModule(void);
	CMuxHwModule(CHwModuleInfo* pInfo);
	virtual ~CMuxHwModule(void);

	virtual CHwModule*			  GetHwModuleByName(CString sName,BOOL bRecursive);
	virtual int     			  GetHwModuleCount();            
	virtual CHwModule*			  GetHwModule(int nIndex);   
	virtual CHwModuleItem*		  GetSubHwModuleItem(list<CHwModuleItem>& ownerList);
	virtual CMuxHwModuleChannel*  GetChannel(int nNumber);
	virtual BOOL				  DeSerialize(CMarkup& xmlReader);
	virtual CString				  Serialize();
	virtual CMuxHwModuleChannel*  GetUpperMuxChannel()                              {return m_pUpperMuxChannel;}
	virtual void    			  SetUpperMuxChannel(CMuxHwModuleChannel *pChannel) {m_pUpperMuxChannel = pChannel;}

	virtual BOOL				  SelectChannel(int nChannel);
	virtual BOOL				  DeSelectChannels();

	virtual int                   GetActiveChannel() {return m_nActiveChannel;}
	virtual int                   GetChannelCount();
	virtual void				  AddChannel(CMuxHwModuleChannel* pChannel);
	virtual IHwModuleContainer*	  GetHwModuleContainer(CHwModule* pHwModule);
	virtual BOOL				  IsHwModuleExists(CString sID);
	virtual BOOL			      ReplaceHwModule(CString sNewItemName, CHwModule* pItem);
	virtual BOOL				  RemoveHwModule(CHwModule *pHwModule);
	virtual BOOL	              SelectHwModule(CHwModule *pHwModule);
	virtual BOOL	              DeSelectHwModule(CHwModule *pHwModule);

	virtual BOOL				  DetectHwModules();
	virtual BOOL				  DetectHwModules(vector<CHwModuleInfo*>& HwModuleInfo, CHwModuleItem* pOwner);

	virtual  BOOL			      Init();

};
