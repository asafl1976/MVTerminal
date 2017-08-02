#include "StdAfx.h"
#include "SysProfilerEventPhaseFunctionOpCode.h"
#include "ProfilerEventManager.h"
#include "SysProfilerBasicEvent.h"

CSysProfilerEventPhaseFunctionSection::CSysProfilerEventPhaseFunctionSection(CSysProfilerBasicEvent* pParentEvent)
{
	m_pParentEvent = pParentEvent;
	m_pResult      = NULL;
}

CSysProfilerEventPhaseFunctionSection::~CSysProfilerEventPhaseFunctionSection(void)
{
	vector<CSysProfilerEventPhaseFunctionOpCode*>::iterator iter, endIter;

	iter = m_functionOpCodes.begin();
	endIter = m_functionOpCodes.end();

	for(; iter != endIter; iter++)
	{
		delete (*iter);
	}
	if(m_pResult != NULL)
		delete m_pResult;
}

BOOL  CSysProfilerEventPhaseFunctionSection::DeSerialize(CMarkup& xmlReader)
{
	//Load Function
	xmlReader.IntoElem();

	//Loop and load Function operations
	while(xmlReader.FindElem() == true)
	{
		if(xmlReader.GetTagName() == "Mul")
		{
			CSysProfilerEventPhaseFunctionMulOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionMulOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load Mul xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "Add")
		{
			CSysProfilerEventPhaseFunctionAddOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionAddOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load Add xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "Div")
		{
			CSysProfilerEventPhaseFunctionDivOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionDivOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load Div xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "Sub")
		{
			CSysProfilerEventPhaseFunctionSubOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionSubOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load Sub xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "And")
		{
			CSysProfilerEventPhaseFunctionAndOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionAndOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load And xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "ShiftLeft")
		{
			CSysProfilerEventPhaseFunctionShiftLeftOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionShiftLeftOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load ShiftLeft xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);

		}
		if(xmlReader.GetTagName() == "ShiftRight")
		{
			CSysProfilerEventPhaseFunctionShiftRightOpCode* pFuncOpCode = new CSysProfilerEventPhaseFunctionShiftRightOpCode(m_pParentEvent);
			if(pFuncOpCode->DeSerialize(xmlReader) == false)
			{
				delete pFuncOpCode;
				AfxMessageBox("Error - Failed to load ShiftRight xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_functionOpCodes.push_back(pFuncOpCode);
		}
		if(xmlReader.GetTagName() == "Result")
		{
			CSysProfilerEventPhaseFunctionResult* pResult = new CSysProfilerEventPhaseFunctionResult(m_pParentEvent);
			if(pResult->DeSerialize(xmlReader) == false)
			{
				delete pResult;
				AfxMessageBox("Error - Failed to load SetBit xml data!",NULL,MB_ICONERROR);
				return FALSE;
			}
			m_pResult = pResult;
		}
	}
	xmlReader.OutOfElem();
	return TRUE;
}

BOOL  CSysProfilerEventPhaseFunctionSection::Execute()
{
	vector<CSysProfilerEventPhaseFunctionOpCode*>::iterator iter, endIter;

	iter = m_functionOpCodes.begin();
	endIter = m_functionOpCodes.end();

	if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
	{
		CString s;
		s.Format("  Start executing function...",m_pParentEvent->GetName());
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
		s.Format("  End executing function...",m_pParentEvent->GetName());
		CProfilerEventManager::GetInstance().AddLogLine(s);
	}
	return TRUE;
}

double  CSysProfilerEventPhaseFunctionSection::GetResult()
{
	if(m_pResult == NULL)
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("  Result = NULL  -->  0");
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		return 0.0;
	}
	return m_pResult->GetResult();
}

CSysProfilerEventPhaseFunctionOpCode::CSysProfilerEventPhaseFunctionOpCode(CSysProfilerBasicEvent* pParentEvent)
{
	m_pParentEvent = pParentEvent;
	m_bConst = FALSE;
	
	m_nDataArrayIndex              = -1;
	m_nWithDataArrayIndex          = -1;
	m_nStoreAtDataArrayIndex       = -1;

	m_nGlobalDataArrayIndex        = -1;
	m_nWithGlobalDataArrayIndex    = -1;
	m_nStoreAtGlobalDataArrayIndex = -1;
}

CSysProfilerEventPhaseFunctionOpCode::~CSysProfilerEventPhaseFunctionOpCode(void)
{
}

BOOL  CSysProfilerEventPhaseFunctionOpCode::DeSerialize(CMarkup& xmlReader)
{
	CString sWithConst		       = xmlReader.GetAttrib("WithConst");
	CString sWithMask		       = xmlReader.GetAttrib("WithMask");
	CString sStoreAtDataArrayIndex = xmlReader.GetAttrib("StoreAtDataArrayIndex");
	CString sDataArrayIndex        = xmlReader.GetAttrib("DataArrayIndex");	
	CString sWithDataArrayIndex    = xmlReader.GetAttrib("WithDataArrayIndex");

	CString sStoreAtGlobalDataArrayIndex = xmlReader.GetAttrib("StoreAtGlobalDataArrayIndex");
	CString sGlobalDataArrayIndex        = xmlReader.GetAttrib("GlobalDataArrayIndex");	
	CString sWithGlobalDataArrayIndex    = xmlReader.GetAttrib("WithGlobalDataArrayIndex");

	if(sStoreAtDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sStoreAtDataArrayIndex,"%d",&m_nStoreAtDataArrayIndex);
	}
	if(sDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sDataArrayIndex,"%d",&m_nDataArrayIndex);
	}
	if(sWithDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sWithDataArrayIndex,"%d",&m_nWithDataArrayIndex);
	}

	if(sStoreAtGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sStoreAtGlobalDataArrayIndex,"%d",&m_nStoreAtGlobalDataArrayIndex);
	}
	if(sGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sGlobalDataArrayIndex,"%d",&m_nGlobalDataArrayIndex);
	}
	if(sWithGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sWithGlobalDataArrayIndex,"%d",&m_nWithGlobalDataArrayIndex);
	}

	if(sWithConst.IsEmpty() == FALSE)
	{
		m_bConst = TRUE;
		sscanf(sWithConst,"%d",&m_nConst);
	}

	if(sWithMask.IsEmpty() == FALSE)
	{
		sscanf(sWithMask,"%d",&m_nMask);
	}

	return TRUE;
}

// And
CSysProfilerEventPhaseFunctionAndOpCode::CSysProfilerEventPhaseFunctionAndOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionAndOpCode::~CSysProfilerEventPhaseFunctionAndOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionAndOpCode::Execute()
{	
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex * const
		if(m_nDataArrayIndex != -1)
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_nMask & (UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] & %x  -->  %0.5f = %0.5f & %x", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nMask,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nMask);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,(UINT32)fGlobalDataArrayValue & m_nMask);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] & %x  -->  %0.5f = %0.5f & %x",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nMask, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nMask);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex * const
		if(m_nDataArrayIndex != -1)
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,m_nMask & (UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] & %x  -->  %0.5f = %0.5f & %x", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nMask,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nMask);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,(UINT32)fGlobalDataArrayValue & m_nMask);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] & %x  -->  %0.5f = %0.5f & %x",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nMask, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nMask);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

// ShiftLeft
CSysProfilerEventPhaseFunctionShiftLeftOpCode::CSysProfilerEventPhaseFunctionShiftLeftOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionShiftLeftOpCode::~CSysProfilerEventPhaseFunctionShiftLeftOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionShiftLeftOpCode::Execute()
{	
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex >> const
		if(m_nDataArrayIndex != -1)
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex, (UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex) << m_nConst);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] << %d  -->  %0.5f = %0.5f << %d", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nConst);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,(UINT32)fGlobalDataArrayValue << m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] << %d  -->  %0.5f = %0.5f << %d",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nConst, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex >> m_nConst
		if(m_nDataArrayIndex != -1)
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
				(UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex) << m_nConst);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] << %d  -->  %0.5f = %0.5f << %d", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nConst);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,(UINT32)fGlobalDataArrayValue << m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] << %d  -->  %0.5f = %0.5f << %x",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nConst, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

// ShiftRight
CSysProfilerEventPhaseFunctionShiftRightOpCode::CSysProfilerEventPhaseFunctionShiftRightOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionShiftRightOpCode::~CSysProfilerEventPhaseFunctionShiftRightOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionShiftRightOpCode::Execute()
{	
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex >> const
		if(m_nDataArrayIndex != -1)
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex, (UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex) >> m_nConst);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] >> %d  -->  %0.5f = %0.5f >> %d", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nConst);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,(UINT32)fGlobalDataArrayValue >> m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] >> %d  -->  %0.5f = %0.5f >> %d",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nConst, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex >> m_nConst
		if(m_nDataArrayIndex != -1)
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
				(UINT32)m_pParentEvent->GetArrayValue(m_nDataArrayIndex) >> m_nConst);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] >> %d  -->  %0.5f = %0.5f >> %d", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nConst);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		else if(m_nGlobalDataArrayIndex != -1)
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,(UINT32)fGlobalDataArrayValue << m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] >> %d  -->  %0.5f = %0.5f >> %x",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nConst, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

// Mul
CSysProfilerEventPhaseFunctionMulOpCode::CSysProfilerEventPhaseFunctionMulOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionMulOpCode::~CSysProfilerEventPhaseFunctionMulOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionMulOpCode::Execute()
{	
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex * const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_nConst*m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] x %d  -->  %0.5f = %0.5f x %d", m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
					m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
					m_pParentEvent->GetArrayValue(m_nDataArrayIndex),m_nConst);
					CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex * WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) * 			
																   CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] x WithGlobalDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex * WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) * 			
																   m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] x WithDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// GlobalDataArrayIndex * const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			double fGlobalDataArrayValue = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,fGlobalDataArrayValue * m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				double fStoreAtDataArrayValue = m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex);
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] x %d  -->  %0.5f = %0.5f x %d",
					m_nStoreAtDataArrayIndex, m_nGlobalDataArrayIndex, m_nConst, 
					fStoreAtDataArrayValue, fGlobalDataArrayValue, m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex * WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) * 			
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] x WithGlobalDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex * const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
										    							 m_nConst*m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] x %d  -->  %0.5f = %0.5f x %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex * WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 m_pParentEvent->GetArrayValue(m_nDataArrayIndex) * 			
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] x WithGlobalDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex * WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 m_pParentEvent->GetArrayValue(m_nDataArrayIndex) * 			
																		 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] x WithDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// GlobalDataArrayIndex * const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
											     m_nConst *  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] x %d  -->  %0.5f = %0.5f x %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex * WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{			
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) * 			
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] x WithGlobalDataArray[%d]  -->  %0.5f = %0.5f x %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

// Add
CSysProfilerEventPhaseFunctionAddOpCode::CSysProfilerEventPhaseFunctionAddOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionAddOpCode::~CSysProfilerEventPhaseFunctionAddOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionAddOpCode::Execute()
{
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex + const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_nConst+m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] + %d  -->  %0.5f = %0.5f + %d",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex + WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) + 			
																   CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] + WithGlobalDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex + WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) + 			
																   m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = DataArray[%d] + WithDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// GlobalDataArrayIndex + const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_nConst + 			
								          CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] + %d  -->  %0.5f = %0.5f + %d",
						 m_nStoreAtDataArrayIndex,m_nGlobalDataArrayIndex,m_nConst,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// GlobalDataArrayIndex + WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) + 			
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       DataArray[%d] = GlobalDataArray[%d] + WithGlobalDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex + const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
										    		m_nConst + m_pParentEvent->GetArrayValue(m_nDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] + %d  -->  %0.5f = %0.5f + %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex + WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
														m_pParentEvent->GetArrayValue(m_nDataArrayIndex) + 			
														CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] + GlobalDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex + WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 m_pParentEvent->GetArrayValue(m_nDataArrayIndex) + 			
																		 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] + WithDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex + const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,m_nConst + 
														 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] + %d  -->  %0.5f = %0.5f + %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_nConst);
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex + WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{			
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
													    CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) + 			
														CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] + WithGlobalDataArray[%d]  -->  %0.5f = %0.5f + %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

//Div

CSysProfilerEventPhaseFunctionDivOpCode::CSysProfilerEventPhaseFunctionDivOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionDivOpCode::~CSysProfilerEventPhaseFunctionDivOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionDivOpCode::Execute()
{
	//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex / const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			if(m_nConst == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  Const is 0");
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): const value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): const value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex)/m_nConst);
			
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] / %d  -->  %0.5f = %0.5f / %d",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex / WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			double fVal = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  Global Data Array Value at index %d is 0",m_nWithGlobalDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Global Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Global Data Array value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) / fVal);
			
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] / WithGlobalDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex / WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			double fVal = m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  DataArray[%d] = 0",m_nWithDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Data Array value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) / fVal);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] / WithDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			if(m_nConst == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  Const = 0");
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): const value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): const value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / m_nConst);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] / %d  -->  %0.5f = %0.5f / %d",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			double fVal = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  GlobalDataArray[%d] = 0",m_nWithGlobalDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Data Array value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / fVal);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = GlobalDataArray[%d] / WithGlobalDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / DataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			double fVal = m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  DataArray[%d] = 0",m_nWithDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Data Array value is 0!");		
			}
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex, CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / fVal);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = GlobalDataArray[%d] / WithDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex / const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			if(m_nConst == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  Const = 0");
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): const value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): const value is 0!");		
			}
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 m_pParentEvent->GetArrayValue(m_nDataArrayIndex)/m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] / %d  -->  %0.5f = %0.5f / %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex / WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			double fVal = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  GlobalDataArray[%d] = 0",m_nWithGlobalDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Data Array value is 0!");		
			}

			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
												m_pParentEvent->GetArrayValue(m_nDataArrayIndex) / fVal);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;
				s.Format("       GlobalDataArray[%d] = DataArray[%d] / WithGlobalDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex / WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			double fVal = m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  DataArray[%d] = 0",m_nWithDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Data Array value is 0!");		
			}

			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
				m_pParentEvent->GetArrayValue(m_nDataArrayIndex) / 	fVal);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] / WithDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			if(m_nConst == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  Const = 0",m_nWithDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): const value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): const value is 0!");		
			}
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] / WithDataArray[%d]  -->  %0.5f = %0.5f / %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			double fVal = CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  GlobalDataArray[%d] = 0",m_nWithGlobalDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Global Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Global Data Array value is 0!");		
			}
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
				CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / fVal);

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] / WithGlobalDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex / DataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			double fVal = m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex);
			if(fVal == 0)
			{
				if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
				{
					CString s;
					s.Format("       *** Zero devision error:  DataArray[%d] = 0",m_nWithDataArrayIndex);
					CProfilerEventManager::GetInstance().AddLogLine(s);
				}
				//AfxMessageBox("Error while executing function (div): Global Data Array value is 0!",NULL,MB_ICONERROR);
				throw string("Error while executing function (div): Global Data Array value is 0!");		
			}

			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
				CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) / fVal);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] / WithDataArray[%d]  -->  %0.5f = %0.5f / %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 fVal);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	return TRUE;
}

//Sub
CSysProfilerEventPhaseFunctionSubOpCode::CSysProfilerEventPhaseFunctionSubOpCode(CSysProfilerBasicEvent* pParentEvent): CSysProfilerEventPhaseFunctionOpCode(pParentEvent)
{}
CSysProfilerEventPhaseFunctionSubOpCode::~CSysProfilerEventPhaseFunctionSubOpCode(void)
{}

BOOL CSysProfilerEventPhaseFunctionSubOpCode::Execute()
{
		//Store at local data array index
	if(m_nStoreAtDataArrayIndex != -1)
	{
		// DataArrayIndex - const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) - m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] - %d  -->  %0.5f = %0.5f - %d",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_bConst,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_bConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex - WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) - 			
														CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] - WithGlobalDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex - WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) - 			
																   m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = DataArray[%d] - WithDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// GlobalDataArrayIndex - const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,
				CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = GlobalDataArray[%d] - %d  -->  %0.5f = %0.5f - %d",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex - WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - 			
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = GlobalDataArray[%d] - WithGlobalDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex - DataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			m_pParentEvent->SetArrayValue(m_nStoreAtDataArrayIndex,
										  CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - 			
										  m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       DataArray[%d] = GlobalDataArray[%d] - WithDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtDataArrayIndex,m_nDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 m_pParentEvent->GetArrayValue(m_nStoreAtDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
	}
	else if(m_nStoreAtGlobalDataArrayIndex != -1)
	{
		// DataArrayIndex - const
		if((m_bConst == TRUE) && (m_nDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
												                         m_pParentEvent->GetArrayValue(m_nDataArrayIndex)- m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] - %d  -->  %0.5f = %0.5f - %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
		// DataArrayIndex - WithGlobalDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) - 			
																	 	CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] - WithGlobalDataArray[%d]  -->  %0.5f = %0.5f - %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// DataArrayIndex - WithDataArrayIndex
		else if((m_nDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,m_pParentEvent->GetArrayValue(m_nDataArrayIndex) - 			
																			m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = DataArray[%d] - WithDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nWithDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex - const
		else if((m_bConst == TRUE) && (m_nGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - m_nConst);
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] - %d  -->  %0.5f = %0.5f - %d",
						 m_nStoreAtGlobalDataArrayIndex,m_nDataArrayIndex,m_nConst,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nDataArrayIndex),
						 m_nConst);

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex - WithGlobalDataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithGlobalDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - 			
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] - WithGlobalDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nWithGlobalDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}
		}
		// GlobalDataArrayIndex - DataArrayIndex
		else if((m_nGlobalDataArrayIndex != -1) && (m_nWithDataArrayIndex != -1))
		{
			CProfilerEventManager::GetInstance().SetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex,
																		 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex) - 			
																		 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));
			if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
			{
				CString s;

				s.Format("       GlobalDataArray[%d] = GlobalDataArray[%d] - WithDataArray[%d]  -->  %0.5f = %0.5f - %0.5f",
						 m_nStoreAtGlobalDataArrayIndex,m_nGlobalDataArrayIndex,m_nWithGlobalDataArrayIndex,
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nStoreAtGlobalDataArrayIndex),
						 CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex),
						 m_pParentEvent->GetArrayValue(m_nWithDataArrayIndex));

				CProfilerEventManager::GetInstance().AddLogLine(s);
			}

		}
	}
	return TRUE;
}

//Result
CSysProfilerEventPhaseFunctionResult::CSysProfilerEventPhaseFunctionResult(CSysProfilerBasicEvent* pParentEvent)
{
	m_pParentEvent			= pParentEvent;
	m_nDataArrayIndex		= -1;
	m_nGlobalDataArrayIndex = -1;

}
CSysProfilerEventPhaseFunctionResult::~CSysProfilerEventPhaseFunctionResult(void)
{}

double CSysProfilerEventPhaseFunctionResult::GetResult()
{
	if(m_nDataArrayIndex != -1)
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("       Result = DataArray[%d]  -->  %0.5f",
					 m_nDataArrayIndex, m_pParentEvent->GetArrayValue(m_nDataArrayIndex));
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		return m_pParentEvent->GetArrayValue(m_nDataArrayIndex);
	}
	else if(m_nGlobalDataArrayIndex != -1)
	{
		if(CProfilerEventManager::GetInstance().GetEnableLogging() == TRUE)
		{
			CString s;
			s.Format("       Result = GlobalDataArray[%d]  -->  %0.5f",
					 m_nGlobalDataArrayIndex, CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex));
			CProfilerEventManager::GetInstance().AddLogLine(s);
		}
		return CProfilerEventManager::GetInstance().GetGlobalDataArrayValue(m_nGlobalDataArrayIndex);
	}
	return -1.0;
}

BOOL  CSysProfilerEventPhaseFunctionResult::DeSerialize(CMarkup& xmlReader)
{
	CString sDataArrayIndex  = xmlReader.GetAttrib("DataArrayIndex");
	CString sGlobalDataArrayIndex  = xmlReader.GetAttrib("GlobalDataArrayIndex");

	if(sDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sDataArrayIndex,"%d",&m_nDataArrayIndex);
	}
	if(sGlobalDataArrayIndex.IsEmpty() == FALSE)
	{
		sscanf(sGlobalDataArrayIndex,"%d",&m_nGlobalDataArrayIndex);
	}

	return TRUE;
}