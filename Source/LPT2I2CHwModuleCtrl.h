#pragma once
#include "HwModuleCtrl.h"
#include "LPT2I2CHwModuleCtrlDrv.h"

class CLPT2I2CHwModuleCtrl : 	public CHwModuleCtrl 
{
	int						 m_nLptNumber;
	CLPT2I2CHwModuleCtrlDrv* m_pDriver;

	CLPT2I2CHwModuleCtrlDrv::LPT2I2C_HIGHDRV_ERROR_CODE  LPTToI2CConnect();

public:
	CLPT2I2CHwModuleCtrl();
	virtual ~CLPT2I2CHwModuleCtrl(void);
	virtual void  EnableDriverErrorMessages(BOOL bEnable);
	virtual BOOL  Init();
	virtual BOOL  Connect();
	virtual BOOL  Disconnect();
	virtual BOOL  IsTargetConnected();


};
