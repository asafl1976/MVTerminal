#pragma once
#include "HwModuleCtrl.h"
#include "libsub.h"
#include "SUB20HwModuleCtrlDrv.h"

#define USB_ID_SUB_20 "USB\\VID_04D8&PID_FFC3\\"

class CSUB20HwModuleCtrlDrv;

class CSUB20HwModuleCtrl : public CHwModuleCtrl
{
	CSUB20HwModuleCtrlDrv* m_pDriver;
public:
	CSUB20HwModuleCtrl(sub_handle handle,CString sUniqueID);
	virtual ~CSUB20HwModuleCtrl(void);
	virtual sub_handle	GetDriverHandle() {return m_pDriver->GetDriverHandle();}
	virtual BOOL		IsTargetConnected();
	virtual void		EnableDriverErrorMessages(BOOL bEnable);
	virtual BOOL        Reset();
	virtual BOOL		Init();
	virtual BOOL		Connect();
	virtual BOOL		Disconnect();

};
