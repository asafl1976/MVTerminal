//   ========================================================================
//   =================    LPT-to-I2Cpro  DLL interface   ====================
//   ===========                Version 2.x                       ===========
//   ===========         Copyright (c) 2003 SB Solutions          ===========
//   ========================================================================
//   ===========                                                  ===========
//   ===========            http://www.i2ctools.com               ===========
//   ========================================================================


#if defined(WIN32)

#define lpt __stdcall
#else
#define lpt __pascal far
#endif // defined(WIN32)

//#ifdef __cplusplus
//    extern "C" {
//#endif

extern "C" short int     __stdcall CheckDriverStatus(void);
extern "C" short int     __stdcall GetAccessMode(void);
extern "C" int           __stdcall GetI2CFrequency(void);
extern "C" short int     __stdcall GetLPTAddress(void);
extern "C" unsigned char __stdcall GetLPTNumber(void);
extern "C" int           __stdcall GetMaxFrequency(void);
extern "C" short int     __stdcall HardwareDetect(void);
extern "C" unsigned char __stdcall I2CRead(short int LastByte, unsigned char *Data);
extern "C" unsigned char __stdcall I2CReadArray(unsigned char address, unsigned char subaddress, int nBytes, unsigned char *ReadData);
extern "C" unsigned char __stdcall I2C10ReadArray(short int address, unsigned char subaddress, int nBytes, unsigned char *ReadData);
extern "C" unsigned char __stdcall I2CReadArrayNS(unsigned char address, int nBytes, unsigned char *ReadData);
extern "C" unsigned char __stdcall I2CReadByte(unsigned char address,unsigned char *ReadData);
extern "C" unsigned char __stdcall I2CStart(void);
extern "C" unsigned char __stdcall I2CStop(void);
extern "C" unsigned char __stdcall I2CWrite(unsigned char DataByte);
extern "C" unsigned char __stdcall I2CWriteArray(unsigned char address, unsigned char subaddress, int nBytes, unsigned char *ReadData);
extern "C" unsigned char __stdcall I2C10WriteArray(short int address, unsigned char subaddress, int nBytes, unsigned char *ReadData);
extern "C" unsigned char __stdcall I2CWriteByte(unsigned char address,unsigned char dataByte);
extern "C" void          __stdcall milliseconds(int data);
extern "C" short int     __stdcall SetNormalMode(void);
extern "C" int           __stdcall SetI2CFrequency(int frequency);
extern "C" short int     __stdcall SetLPTNumber(unsigned char LPT);
extern "C" short int     __stdcall SetSlowMode(void);
extern "C" int           __stdcall SetWaitTime(int NewWaitTime);
extern "C" short int     __stdcall StartI2CDriver(int device);
extern "C" void          __stdcall StopI2CDriver(void);

//#ifdef __cplusplus
//	}
//#endif
