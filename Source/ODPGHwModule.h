#pragma once
#include "HwModule.h"
#include "HWModuleAccessStartegy.h"
#include <map>
using namespace std;

class CODPGHwModuleModelessDlg;
class CIODPGConfigStrategy;
class CODPGPattern;
class CODPGHwModuleAccessStartegy;
class CHwModuleCtrlI2CUnitDrv;

typedef enum _OdpgBusType
{
	ODPG_DATA_BUS,
	ODPG_CTRL_BUS
} ODPG_BUS_TYPE;

typedef enum _OdpgStopCondition
{
	ODPG_STOP_COND_CONTINUOUS,
	ODPG_STOP_COND_SINGLE_PATTERN,
	ODPG_STOP_COND_PRE_DEF_ADDR,
	ODPG_STOP_COND_ERR_DATA_ADDR,
	ODPG_STOP_COND_PHASE_COUNT
} ODPG_STOP_CONDITION;

class CCEODPGRWRegTransCmdItemParams
{
public:
	CCEODPGRWRegTransCmdItemParams()
	{
		m_sOffset = "0";
		m_sValue  = "0";
		m_bRead   = TRUE;
	}

	CString      m_sOffset;
	CString      m_sValue;
	BOOL	     m_bRead;
};

class CODPGParams
{
public:
	CODPGParams()
	{
		m_eBusType            = ODPG_DATA_BUS;
		m_eStopCond           = ODPG_STOP_COND_CONTINUOUS;
		m_bRead		          = FALSE;
		m_bWrite              = TRUE;
		m_bCompareData        = FALSE;
		m_nAddress			  = 0;
		m_nBusWidth		      = 32;
		m_nStopCondPhaseCount = 0;
		m_nDramCS			  = 0;
		m_pPattern			  = NULL;
	}
	ODPG_BUS_TYPE		    m_eBusType;
	ODPG_STOP_CONDITION     m_eStopCond;
	BOOL			        m_bRead;
	BOOL                    m_bWrite;
	BOOL                    m_bCompareData;
	UINT                    m_nAddress;
	UINT				    m_nDramCS;
	int                     m_nBusWidth;
	int						m_nStopCondPhaseCount;
	CODPGPattern*			m_pPattern;
};


class CODPGHwModule : public CHwModule
{
	map<CString,CIODPGConfigStrategy*>  m_configStrategy;
	CString								m_sSelectedDevice;
	CODPGParams							m_params;
	CODPGHwModuleAccessStartegy*        m_pAccessStrategy;

	CIODPGConfigStrategy*  GetConfigStrategy();
public:
	CODPGHwModule();
	CODPGHwModule(CHwModuleInfo* pInfo);
	virtual ~CODPGHwModule(void);
	virtual CHwModule*    Clone(CHwModuleInfo* pInfo);
	virtual int     	  GetBufferLineByteCount();
	virtual int     	  GetBufferLineCount();
	virtual BOOL    	  WriteRegister32(UINT nRegOffset, UINT nRegValue, BOOL bReadModifyWrite = FALSE);
	virtual BOOL    	  ReadRegister32(UINT nRegOffset,UINT *pRegValue);
	virtual void    	  SetDevice(CString sDevice);
	virtual BOOL    	  Set();
	virtual BOOL    	  Start();
	virtual BOOL    	  Stop();
	virtual void    	  GetDeviceList(vector<CString> & devList);
	virtual CString 	  GetDeviceBusWidth(CString sDevice);
	CODPGParams     	  GetParams()						{return m_params;}
	void            	  SetParams(CODPGParams params) {m_params = params;}

};

