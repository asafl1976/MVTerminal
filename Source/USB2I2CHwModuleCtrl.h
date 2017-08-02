#pragma once
#include "HwModuleCtrl.h"
#include "USB2I2CHwModuleCtrlDrv.h"

#define USB_ID_USB_2_I2C "USB\\VID_18E4&PID_2000\\"


class CUSB2I2CHwModuleCtrl : public CHwModuleCtrl 
{
	CUSB2I2CHwModuleCtrlDrv* m_pDriver;

public:
	CUSB2I2CHwModuleCtrl(CString sUniqueID);
	virtual ~CUSB2I2CHwModuleCtrl(void);
	virtual void  EnableDriverErrorMessages(BOOL bEnable);
	virtual BOOL  Init();
	virtual BOOL  Connect();
	virtual BOOL  Disconnect();
	virtual BOOL  IsTargetConnected();


};
