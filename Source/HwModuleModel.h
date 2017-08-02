#pragma once

#include "IHWModuleModel.h"
#include "HwModule.h"
#include "HwModuleCtrl.h"
#include "HwModuleFactory.h"
#include "IHwModuleModelSubject.h"
#include "libsub.h"

#include <vector>
#include <map>
using namespace std;

class IHwModuleContainer;
class CSUB20HwModuleCtrl;
class CLPT2I2CHwModuleCtrl;

class CHwModuleModel : public CIHWModuleModel, public IHwModuleModelSubject
{
	BOOL						  m_bDetectHwModulesOnStartup;
	BOOL					      m_bInitialized;
protected:
	vector<CHwModuleCtrl*>	      m_HwModuleCtrls;

	CHwModuleModel(void);

	virtual void			       CreateUsbHwModuleCtrl();
	virtual BOOL                   IsHwModuleCtrlExist(CHwModuleCtrl *pCtrl);
	virtual BOOL				   IsUsbDeviceExist(CString sVendorID,CString sUniqueID);
	virtual CHwModuleCtrl*		   GetHwModuleCtrl(CString sName,CString sVendorID,CString sUniqueID);
	virtual CHwModuleCtrlUnit*     GetHwModuleCtrlUnitItemUnit(CHwModuleCtrlUnitItem *pItem);
	//LPT-2-I2C
	virtual CLPT2I2CHwModuleCtrl*  CreateLPT2I2CModuleCtrl();

	//SUB-20
	virtual BOOL			       AddSub20UsbDevice();
	virtual BOOL			       FindSub20Devices(vector<sub_handle>& sub20handles);
	virtual void				   CreateSub20UsbHwModuleCtrls(vector<sub_handle>&  sub20handles);
	virtual CSUB20HwModuleCtrl*	   CreateSub20UsbHwModuleCtrl(sub_handle handle,CString sUniqueID);
	virtual CString			       GetSub20UniqueID(sub_handle handle);


public:

	virtual ~CHwModuleModel(void);
	static  CHwModuleModel&        GetInstance();
	virtual CHwModuleCtrl*         GetHwModuleCtrl(CString sName, CString sID);
	virtual void			       Init(BOOL bDetHwModule);
	virtual BOOL                   DetectHwModules();

	virtual CString			       Serialize();
	virtual BOOL			       DeSerialize(CMarkup & xmlReader);

	virtual BOOL			       GetDetectHwModulesOnStartup() {return m_bDetectHwModulesOnStartup;}
	virtual void			       SetDetectHwModulesOnStartup(BOOL bDetectHwModulesOnStartup) {m_bDetectHwModulesOnStartup = bDetectHwModulesOnStartup;}

	virtual BOOL			       SaveHwModuleModelSettings(CString sFile);
	virtual BOOL			       SaveHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);
	virtual BOOL			       LoadHwModuleModelSettings(CString sFile);
	virtual BOOL			       LoadHwModuleModelSettings(CString sFile,CHwModuleCtrl *pCtrl);

	virtual BOOL			       RefreshHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual BOOL			       ResetHwModuleCtrl(CHwModuleCtrl *pCtrl);
	virtual void			       ReleaseHwModuleCtrls();
	virtual void			       ReleaseHwModules();
	virtual int                    GetHwModuleCtrlCount();
	virtual CHwModuleCtrl*         GetHwModuleCtrl(int i);
	virtual CHwModuleItem*		   GetHwModuleItem(list<CHwModuleItem>& ownerList) ;
	virtual IHwModuleContainer*    GetHwModuleContainer(CHwModule *pHwModule);
	virtual BOOL                   ReplaceHwModule(CHwModule *pObj, CString sNewHwModuleName);
	virtual BOOL                   RemoveHwModule(CHwModule *pHwModule);	
	virtual BOOL                   RemoveHwModuleCtrl(CHwModuleCtrl *pHwModuleCtrl);
	virtual BOOL                   SetHwModuleCtrlUnitItemDisplayName(CHwModuleCtrlUnitItem* pItem,CString sDisplayName);
	virtual BOOL                   SetHwModuleDisplayName(CHwModule* pHwModule, CString sDisplayName);
	virtual BOOL                   SelectHwModule(CHwModule *pHwModule);
	virtual BOOL                   DeSelectHwModule(CHwModule *pHwModule);
	virtual BOOL                   AddUsbDevice(CString sDevID);
	virtual BOOL                   RemoveUsbDevice(CString sDevID);
	virtual void                   GetHwModuleCtrlList(vector<CHwModuleCtrl*>& HWCtrlList);
	virtual CHwModule*             GetHwModule(CString sHwModuleCtrlName, int nHwModuleCtrlID, CString sName);
	virtual CHwModuleCtrlUnitItem* GetHwModuleCtrlUnitItem(CString sHwModuleCtrlName, int nHwModuleCtrlID,CString sUnitName, CString sName);
	
};
