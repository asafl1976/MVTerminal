#include "StdAfx.h"
#include ".\USB2I2CHwModuleCtrlDrv.h"
#include "USBtoI2Cpro.h"

CUSB2I2CHwModuleCtrlDrv::CUSB2I2CHwModuleCtrlDrv(int nSerialNumber)
{
	m_nDelay		= 50;
	m_nSerialNumber = nSerialNumber;
}

CUSB2I2CHwModuleCtrlDrv::~CUSB2I2CHwModuleCtrlDrv(void)
{
}
void  CUSB2I2CHwModuleCtrlDrv::Unload()
{
	ShutdownProcedure();
}

BOOL  CUSB2I2CHwModuleCtrlDrv::Load()
{
	return TRUE;
}

int   CUSB2I2CHwModuleCtrlDrv::OpenDevice()
{
	return SelectBySerialNumber(m_nSerialNumber);
}

void CUSB2I2CHwModuleCtrlDrv::ShowErrorMessage(USB2I2C_ERROR_CODE code)
{
	if(m_bErrMsgEnable == FALSE)
		return;

	switch(code)
	{
	case USB2I2C_ADDR_NOT_ACK:
		AfxMessageBox("USB-2-I2C: Address not Acknowledged!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_DATA_NOT_ACK:
		AfxMessageBox("USB-2-I2C: Data not Acknowledged!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_ARB_LOST:
		AfxMessageBox("USB-2-I2C: Arbitration lost!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_I2C_TIMEOUT:
		AfxMessageBox("USB-2-I2C: I2C Time Out!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_TRANS_ABORTED:
		AfxMessageBox("USB-2-I2C: Transmission aborted!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_MSG_SENT_NACK_ENC:
		AfxMessageBox("USB-2-I2C: Message sent but a Nack was encountered!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_UNSUPP_FUNC:
		AfxMessageBox("USB-2-I2C: Unsupported function (make sure you have the latest firmware)!",NULL,MB_ICONERROR);
		break;
	case USB2I2C_HW_NOT_DETECT_OR_USB_ERR:
		AfxMessageBox("USB-2-I2C: Hardware not detected or USB error!",NULL,MB_ICONERROR);
		break;
	default:
	    AfxMessageBox("USB-2-I2C: Unknown error code!",NULL,MB_ICONERROR);
		break;
	}
}

BOOL  CUSB2I2CHwModuleCtrlDrv::WriteData(UCHAR nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pWriteData)
{
    nDevAddr <<= 1;//Shift the address --> read/write bit.

    m_eErrorCode = (USB2I2C_ERROR_CODE)I2CWriteArray(nDevAddr, nDevSubAddress, nBytes, pWriteData);
    if ((m_eErrorCode != USB2I2C_NO_ERROR))
    {
		ShowErrorMessage(m_eErrorCode);
        return FALSE;
    }
	Sleep(m_nDelay);
    return TRUE;
}

BOOL   CUSB2I2CHwModuleCtrlDrv::ReadData(UCHAR  nDevAddr, UCHAR nDevSubAddress, int nBytes, UCHAR *pReadData)
{
    nDevAddr <<= 1;//Shift the address to make a room for read/write bit.

    m_eErrorCode = (USB2I2C_ERROR_CODE)I2CReadArray(nDevAddr, nDevSubAddress, nBytes, pReadData);

    if (m_eErrorCode != USB2I2C_NO_ERROR)
    {
		ShowErrorMessage(m_eErrorCode);
        return FALSE;
    }	
	Sleep(m_nDelay);
    return TRUE;
}

BOOL   CUSB2I2CHwModuleCtrlDrv::WriteData(UCHAR nDevAddr, int nBytes, UCHAR *pWriteData, BOOL bSendStopBit)
{
	//Workaround for sending a data without subaddress.
    //The first data's byte is sent as a subaddress, and the 
    //sent data is the original without the first byte.
	return WriteData(nDevAddr,pWriteData[0],nBytes - 1,&pWriteData[1]);
}
BOOL   CUSB2I2CHwModuleCtrlDrv::ReadData(UCHAR nDevAddr, int nBytes, UCHAR *pReadData, BOOL bSendStopBit)
{
    nDevAddr <<= 1;//Shift the address to make a room for read/write bit.

    m_eErrorCode = (USB2I2C_ERROR_CODE)I2CRead(nDevAddr, nBytes, pReadData,bSendStopBit);
    if (m_eErrorCode != USB2I2C_NO_ERROR)
    {
		ShowErrorMessage(m_eErrorCode);
        return FALSE;
    }	
	Sleep(m_nDelay);
	return TRUE;

}