#pragma once
#include "hwmodulectrldrv.h"

class CUSB2I2CHwModuleCtrlDrv : public CHwModuleCtrlDrv
{
	CString       m_sName;
	int           m_nDelay;
	int			  m_nSerialNumber;
public:
	typedef enum __USBToI2cLowDrvErrorCode 
	{   
		USB2I2C_NO_ERROR				 = 0x0,
		USB2I2C_ADDR_NOT_ACK			 = 0x1,
		USB2I2C_DATA_NOT_ACK			 = 0x2,
		USB2I2C_ARB_LOST				 = 0x7,
		USB2I2C_I2C_TIMEOUT				 = 0x8,
		USB2I2C_TRANS_ABORTED			 = 0xA,
		USB2I2C_MSG_SENT_NACK_ENC		 = 0xB,
		USB2I2C_UNSUPP_FUNC			     = 0x80,
		USB2I2C_HW_NOT_DETECT_OR_USB_ERR = 0xFF,
	}  USB2I2C_ERROR_CODE;

	USB2I2C_ERROR_CODE  m_eErrorCode;
public:

	CUSB2I2CHwModuleCtrlDrv(int nSerialNumber);
	virtual ~CUSB2I2CHwModuleCtrlDrv(void);
	virtual USB2I2C_ERROR_CODE   GetLastErrorCode() {return m_eErrorCode;}
	virtual BOOL   Load();
	virtual void   Unload();
	virtual int    OpenDevice();
	virtual void   ShowErrorMessage(USB2I2C_ERROR_CODE errCode);
	virtual BOOL   WriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData);
    virtual BOOL   ReadData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData);
	virtual BOOL   WriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit = TRUE);
    virtual BOOL   ReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit = TRUE);


};
