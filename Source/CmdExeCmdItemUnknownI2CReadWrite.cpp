#include "StdAfx.h"
#include "CmdExeCmdItemUnknownI2CReadWrite.h"
#include "MVTerminal.h"
#include "HwModuleModel.h"
#include "HwModuleCtrlUnit.h"

CCmdExeCmdItemUnknownI2CReadWrite::CCmdExeCmdItemUnknownI2CReadWrite()
{
	m_params.m_bRead    = FALSE;
	m_params.m_bStopBit = TRUE;
	m_params.m_nByteCount = 0;

	m_sName        = "Unknown I2C - Read/Write";
	m_sDisplayName = "Unknown I2C - Read/Write";
}

CCmdExeCmdItemUnknownI2CReadWrite::~CCmdExeCmdItemUnknownI2CReadWrite(void)
{
	
}
BOOL	CCmdExeCmdItemUnknownI2CReadWrite::OnStartExecution()
{
	CUnknownI2CHwModule *pUnknownI2C = dynamic_cast<CUnknownI2CHwModule*>(m_pHwModuleItem);

	if(pUnknownI2C == NULL)
	{
		OnError("Can't access Unknown I2C module!");
		return FALSE;
	}
	if(CHwModuleModel::GetInstance().SelectHwModule(pUnknownI2C) == FALSE)
	{
		AfxMessageBox("Unable to activate " + pUnknownI2C->GetDisplayName(),NULL,MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

BOOL	CCmdExeCmdItemUnknownI2CReadWrite::OnEndExecution()
{
	CUnknownI2CHwModule *pUnknownI2C = dynamic_cast<CUnknownI2CHwModule*>(m_pHwModuleItem);

	if(pUnknownI2C == NULL)
	{
		OnError("Can't access Unknown I2C module!");
		return FALSE;
	}
	if(CHwModuleModel::GetInstance().DeSelectHwModule(pUnknownI2C) == FALSE)
	{
		AfxMessageBox("Unable to de-activate " + pUnknownI2C->GetDisplayName(),NULL,MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

BOOL    CCmdExeCmdItemUnknownI2CReadWrite::Execute()
{
	CUnknownI2CHwModule *pUnknownI2C = dynamic_cast<CUnknownI2CHwModule*>(m_pHwModuleItem);

	if(pUnknownI2C == NULL)
	{
		AfxMessageBox("Can't access Unknown I2C module!",NULL,MB_ICONERROR);
		return FALSE;
	}

	OnStartExecution();

	if(m_params.m_bRead == TRUE)
	{
		if(m_params.m_nByteCount == 0)
		{
			Log(m_sDisplayName + " - Invalid byte count!!!");
			OnEndExecution();
			OnError(m_sDisplayName + " - Invalid byte count!!!");
			return FALSE;
		}
		if(pUnknownI2C->ReadData(m_params.m_nByteCount,(UCHAR*)m_params.m_data,m_params.m_bStopBit) == FALSE)
		{
			Log(m_sDisplayName + " - Read ERROR!!!");
			OnEndExecution();
			OnError(m_sDisplayName + " - Read ERROR!!!");
			return FALSE;
		}
		m_params.m_data[m_params.m_nByteCount] = NULL;
		OnEndExecution();
		int nByte = 0;
        CString sData;
		while(m_params.m_data[nByte] != NULL)
		{
			CString sByte;
			sByte.Format("0x%x ", m_params.m_data[nByte]);
			sData += sByte;
			nByte++;
		}
		Log(m_sDisplayName + " - Read data = " + sData);
	}
	else //write
	{
		// Set end of data
		m_params.m_data[m_params.m_nByteCount] = NULL;
		if(pUnknownI2C->WriteData(m_params.m_nByteCount,(UCHAR*)m_params.m_data,m_params.m_bStopBit) == FALSE)
		{
			Log(m_sDisplayName + " - Write ERROR!!!");
			OnEndExecution();
			OnError(m_sDisplayName + " - Write ERROR!!!");
			return FALSE;
		}
		OnEndExecution();
		Log(m_sDisplayName + " - Write OK!");
	}	
	return TRUE;
}

BOOL  CCmdExeCmdItemUnknownI2CReadWrite::Serialize(CMarkup & xmlWriter)
{
	xmlWriter.AddElem( "CmdExeCmdItem");
	xmlWriter.AddAttrib( "Class","CCmdExeCmdItemUnknownI2CReadWrite");
	CCmdExeCmdItem::Serialize(xmlWriter);

	CString sData((LPCSTR)(m_params.m_data));
	xmlWriter.AddAttrib( "Data",sData);//Fix
	xmlWriter.AddAttrib( "StopBit",m_params.m_bStopBit);
	xmlWriter.AddAttrib( "Read",m_params.m_bRead);

	return SerializeHwModuleItem(xmlWriter);
}

BOOL  CCmdExeCmdItemUnknownI2CReadWrite::DeSerialize(CMarkup & xmlReader)
{
	if(CCmdExeCmdItem::DeSerialize(xmlReader) == FALSE)
		return FALSE;

	CString sData;
	sData = xmlReader.GetAttrib("Data");
	memcpy(m_params.m_data, (VOID*)LPCTSTR(sData), sData.GetLength());
	m_params.m_bStopBit = (xmlReader.GetAttrib("StopBit") == "0")? FALSE:TRUE;
	m_params.m_bRead = (xmlReader.GetAttrib("Read") == "0")? FALSE:TRUE;

	return DeSerializeHwModuleItem(xmlReader);
}

void    CCmdExeCmdItemUnknownI2CReadWrite::SetHwModuleItem(CHwModuleItem* pUnknownI2C)
{
	m_pHwModuleItem = pUnknownI2C;
}


CCmdExeCmdItem*    CCmdExeCmdItemUnknownI2CReadWrite::Clone()
{
	CCmdExeCmdItemUnknownI2CReadWrite *pClone = new CCmdExeCmdItemUnknownI2CReadWrite();
	pClone->SetHwModuleItem(m_pHwModuleItem);
	pClone->SetParams(GetParams());
	return pClone;
}

void    CCmdExeCmdItemUnknownI2CReadWrite::OnDeleteObject() 
{
	m_pHwModuleItem = NULL;
}


