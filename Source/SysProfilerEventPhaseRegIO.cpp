#include "StdAfx.h"
#include "ProfilerEventManager.h"
#include "SysProfilerEventPhaseRegIO.h"
#include "Markup.h"

CSysProfilerEventPhaseRegIOSection::CSysProfilerEventPhaseRegIOSection(CSysProfilerBasicEvent* pParentEvent)
{
	m_pParentEvent = pParentEvent;
}

CSysProfilerEventPhaseRegIOSection::~CSysProfilerEventPhaseRegIOSection(void)
{
	vector<CSysProfilerEventPhaseRegIO*>::iterator iter, endIter;

	iter = m_regIO.begin();
	endIter = m_regIO.end();

	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
}

BOOL  CSysProfilerEventPhaseRegIOSection::DeSerialize(CMarkup& xmlReader)
{
	xmlReader.IntoElem();

	//Loop and load reg IO
	while(xmlReader.FindElem() == true)
	{
		if(xmlReader.GetTagName() == "WriteRegister")
		{
			CSysProfilerEventPhaseRegIOWrite* pRegIOWrite = new CSysProfilerEventPhaseRegIOWrite(m_pParentEvent);
			if(pRegIOWrite->DeSerialize(xmlReader) == false)
			{
				delete pRegIOWrite;
				AfxMessageBox("Error - Failed to load ReadRegister xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_regIO.push_back(pRegIOWrite);

		}
		if(xmlReader.GetTagName() == "ReadRegister")
		{
			CSysProfilerEventPhaseRegIORead* pRegIORead = new CSysProfilerEventPhaseRegIORead(m_pParentEvent);
			if(pRegIORead->DeSerialize(xmlReader) == false)
			{
				delete pRegIORead;
				AfxMessageBox("Error - Failed to load ReadRegister xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_regIO.push_back(pRegIORead);
		}
		if(xmlReader.GetTagName() == "ReadModifyWrite")
		{
			CSysProfilerEventPhaseRegIOReadModifyWrite* pRegIOReadModifyWrite = new CSysProfilerEventPhaseRegIOReadModifyWrite(m_pParentEvent);
			if(pRegIOReadModifyWrite->DeSerialize(xmlReader) == false)
			{
				delete pRegIOReadModifyWrite;
				AfxMessageBox("Error - Failed to load ReadModifyWrite xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_regIO.push_back(pRegIOReadModifyWrite);
		}
		if(xmlReader.GetTagName() == "ResetBit")
		{
			CSysProfilerEventPhaseRegIOResetBit* pRegIOResetBit = new CSysProfilerEventPhaseRegIOResetBit(m_pParentEvent);
			if(pRegIOResetBit->DeSerialize(xmlReader) == false)
			{
				delete pRegIOResetBit;
				AfxMessageBox("Error - Failed to load ResetBit xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_regIO.push_back(pRegIOResetBit);
		}
		if(xmlReader.GetTagName() == "SetBit")
		{
			CSysProfilerEventPhaseRegIOSetBit* pRegIOSetBit = new CSysProfilerEventPhaseRegIOSetBit(m_pParentEvent);
			if(pRegIOSetBit->DeSerialize(xmlReader) == false)
			{
				delete pRegIOSetBit;
				AfxMessageBox("Error - Failed to load SetBit xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_regIO.push_back(pRegIOSetBit);
		}
	}

	xmlReader.OutOfElem();

	return TRUE;
}

BOOL  CSysProfilerEventPhaseRegIOSection::Execute()
{
	vector<CSysProfilerEventPhaseRegIO*>::iterator iter, endIter;

	iter = m_regIO.begin();
	endIter = m_regIO.end();

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("  Start executing register I/O...");
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	for(; iter != endIter; iter++)
	{
		if((*iter)->Execute() == FALSE)
		{
			return FALSE;
		}
	}
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("  End executing register I/O...");
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return TRUE;
}


CSysProfilerEventPhaseRegIO::CSysProfilerEventPhaseRegIO(CSysProfilerBasicEvent* pParentEvent)
{
	m_pParentEvent = pParentEvent;
}

CSysProfilerEventPhaseRegIO::~CSysProfilerEventPhaseRegIO(void)
{
}
////// Read register
CSysProfilerEventPhaseRegIORead::CSysProfilerEventPhaseRegIORead(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseRegIO(pParentEvent)
{
	m_nStoreAtDataArrayIndex	   = -1;
	m_nStoreAtGlobalDataArrayIndex = -1;
}

CSysProfilerEventPhaseRegIORead::~CSysProfilerEventPhaseRegIORead(void)
{}
BOOL  CSysProfilerEventPhaseRegIORead::DeSerialize(CMarkup& xmlReader)
{
	CString sAddress					 = xmlReader.GetAttrib("Address");
	CString sStoreAtDataArrayIndex		 = xmlReader.GetAttrib("StoreAtDataArrayIndex"); 
	CString sStoreAtGlobalDataArrayIndex = xmlReader.GetAttrib("StoreAtGlobalDataArrayIndex"); 

	if(sStoreAtDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sStoreAtDataArrayIndex,"%d",&m_nStoreAtDataArrayIndex);
	}
	if(sStoreAtGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sStoreAtGlobalDataArrayIndex,"%d",&m_nStoreAtGlobalDataArrayIndex);
	}
	if(sAddress.Find("0x") != -1)
	{
		sAddress = sAddress.Right(sAddress.GetLength() - 2);
	}
	sscanf(sAddress,"%x",&m_nAddress);
	return TRUE;
}

BOOL CSysProfilerEventPhaseRegIORead::Execute()
{
	UINT32 nVal = 0;

	if(CProfilerEventManager::GetInstance().ReadMarvellDeviceRegister(m_nAddress,&nVal) == FALSE)
	{
		return FALSE;
	}
	if(m_nStoreAtDataArrayIndex != -1)
	{
		m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,nVal);
	}
	if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,nVal);
	}
	return TRUE;
}

////// Write register
CSysProfilerEventPhaseRegIOWrite::CSysProfilerEventPhaseRegIOWrite(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseRegIO(pParentEvent)
{
	m_bUseValue				= FALSE;
	m_nDataArrayIndex	    = -1;
	m_nGlobalDataArrayIndex = -1;
}
CSysProfilerEventPhaseRegIOWrite::~CSysProfilerEventPhaseRegIOWrite(void)
{}
BOOL  CSysProfilerEventPhaseRegIOWrite::DeSerialize(CMarkup& xmlReader)
{
	CString sAddress			  = xmlReader.GetAttrib("Address");
	CString sValue				  = xmlReader.GetAttrib("Value");
	CString sDataArrayIndex		  = xmlReader.GetAttrib("DataArrayIndex"); 
	CString sGlobalDataArrayIndex = xmlReader.GetAttrib("GlobalDataArrayIndex"); 

	if(sDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sDataArrayIndex,"%d",&m_nDataArrayIndex);
	}
	if(sGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sGlobalDataArrayIndex,"%d",&m_nGlobalDataArrayIndex);
	}
	if(sValue.IsEmpty() == FALSE)
	{
		if(sValue.Find("0x") != -1)
		{
			sValue = sValue.Right(sValue.GetLength() - 2);
		}
		sscanf(sValue,"%x",&m_nValue);
		m_bUseValue = TRUE;
	}

	if(sAddress.Find("0x") != -1)
	{
		sAddress = sAddress.Right(sAddress.GetLength() - 2);
	}

	sscanf(sAddress,"%x",&m_nAddress);

	return TRUE;
}

BOOL CSysProfilerEventPhaseRegIOWrite::Execute()
{
	if(m_bUseValue == TRUE)
	{
		return CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress,m_nValue);
	}
	if(m_nDataArrayIndex != -1)
	{
		return CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress, (UINT)m_pParentEvent->GetArrayValue(m_nDataArrayIndex) );
	}
	if(m_nGlobalDataArrayIndex != -1)
	{
		return CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress, 
					(UINT)CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) );	
	}
	return FALSE;
}

////// Read Modify Write register
CSysProfilerEventPhaseRegIOReadModifyWrite::CSysProfilerEventPhaseRegIOReadModifyWrite(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseRegIO(pParentEvent)
{
	
}
CSysProfilerEventPhaseRegIOReadModifyWrite::~CSysProfilerEventPhaseRegIOReadModifyWrite(void)
{}
BOOL  CSysProfilerEventPhaseRegIOReadModifyWrite::DeSerialize(CMarkup& xmlReader)
{
	CString sAddress			  = xmlReader.GetAttrib("Address");
	CString sMask				  = xmlReader.GetAttrib("Mask");
	CString sValue				  = xmlReader.GetAttrib("Value");

	if(sValue.Find("0x") != -1)
	{
		sValue = sValue.Right(sValue.GetLength() - 2);
	}
	if(sAddress.Find("0x") != -1)
	{
		sAddress = sAddress.Right(sAddress.GetLength() - 2);
	}
	if(sMask.Find("0x") != -1)
	{
		sMask = sMask.Right(sMask.GetLength() - 2);
	}

	sscanf(sValue,"%x",&m_nValue);
	sscanf(sMask,"%x",&m_nMask);
	sscanf(sAddress,"%x",&m_nAddress);

	return TRUE;
}

BOOL CSysProfilerEventPhaseRegIOReadModifyWrite::Execute()
{
	UINT nVal = 0;

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    Start read modify write (mask = 0x%x, value = 0x%x) for register 0x%x...",m_nMask,m_nValue,m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	if(CProfilerEventManager::GetInstance().ReadMarvellDeviceRegister(m_nAddress,&nVal) == FALSE)
	{
		return FALSE;
	}

	int nShift = 0;
	for(int bit = 0; bit < 32; bit++)
	{
		if( ((m_nMask >> nShift) & 0x1) == 0x1)
		{
			break;
		}
		nShift++;		
	}

	nVal = nVal & ~m_nMask;
	nVal = nVal | (m_nValue << nShift);
	if(CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress,nVal) == FALSE)
	{
		return FALSE;
	}
	
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    End read modify write (mask = 0x%x, value = 0x%x) for register 0x%x",m_nMask,(m_nValue << nShift),m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return TRUE;
}

////// ResetBit
CSysProfilerEventPhaseRegIOResetBit::CSysProfilerEventPhaseRegIOResetBit(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseRegIO(pParentEvent)
{}
CSysProfilerEventPhaseRegIOResetBit::~CSysProfilerEventPhaseRegIOResetBit(void)
{}
BOOL  CSysProfilerEventPhaseRegIOResetBit::DeSerialize(CMarkup& xmlReader)
{
	CString sAddress = xmlReader.GetAttrib("Address");
	CString sBit   = xmlReader.GetAttrib("Bit");

	if(sAddress.Find("0x") != -1)
	{
		sAddress = sAddress.Right(sAddress.GetLength() - 2);
	}
	
	sscanf(sAddress,"%x",&m_nAddress);
	sscanf(sBit,"%d",&m_nBit);

	return TRUE;
}

BOOL CSysProfilerEventPhaseRegIOResetBit::Execute()
{
	UINT nVal = 0;

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    Start reset bit#%d for register 0x%x",m_nBit,m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	if(CProfilerEventManager::GetInstance().ReadMarvellDeviceRegister(m_nAddress,&nVal) == FALSE)
	{
		return FALSE;
	}
	nVal = nVal & ~(0x1 << m_nBit);
	if(CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress,nVal) == FALSE)
	{
		return FALSE;
	}
	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    End reset bit#%d for register 0x%x",m_nBit,m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return TRUE;
}

////// Set Bit
CSysProfilerEventPhaseRegIOSetBit::CSysProfilerEventPhaseRegIOSetBit(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseRegIO(pParentEvent)
{}
CSysProfilerEventPhaseRegIOSetBit::~CSysProfilerEventPhaseRegIOSetBit(void)
{}
BOOL  CSysProfilerEventPhaseRegIOSetBit::DeSerialize(CMarkup& xmlReader)
{

	CString sAddress = xmlReader.GetAttrib("Address");
	CString sBit     = xmlReader.GetAttrib("Bit");

	if(sAddress.Find("0x") != -1)
	{
		sAddress = sAddress.Right(sAddress.GetLength() - 2);
	}

	sscanf(sAddress,"%x",&m_nAddress);
	sscanf(sBit,"%d",&m_nBit);


	return TRUE;
}

BOOL CSysProfilerEventPhaseRegIOSetBit::Execute()
{
	UINT nVal = 0;

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    Start set bit#%d for register 0x%x",m_nBit,m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	if(CProfilerEventManager::GetInstance().ReadMarvellDeviceRegister(m_nAddress,&nVal) == FALSE)
	{
		return FALSE;
	}
	nVal = nVal | (0x1 << m_nBit);
	if(CProfilerEventManager::GetInstance().WriteMarvellDeviceRegister(m_nAddress,nVal) == FALSE)
	{
		return FALSE;
	}

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("    End set bit#%d for register 0x%x",m_nBit,m_nAddress);
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}

	return TRUE;
}

