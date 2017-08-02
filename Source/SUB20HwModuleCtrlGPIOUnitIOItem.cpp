#include "StdAfx.h"
#include ".\sub20hwmodulectrlgpiounitioitem.h"
#include "HwModuleCtrlGPIOUnitDrv.h"
#include "SUB20HwModuleCtrlGPIOUnit.h"

CSUB20HwModuleCtrlGPIOUnitIOItem::CSUB20HwModuleCtrlGPIOUnitIOItem(int nIndex, CSUB20HwModuleCtrlGPIOUnit *pUnit)
:CSUB20HwModuleCtrlGPIOUnitItem(pUnit)
{
	m_nIndex        = nIndex;
	m_sName         = "GPIO";
	m_sDisplayName  = "GPIO";
	m_bEnable       = TRUE;
	m_sID.Format("%d",m_nIndex);
}

CSUB20HwModuleCtrlGPIOUnitIOItem::~CSUB20HwModuleCtrlGPIOUnitIOItem(void)
{
}

BOOL CSUB20HwModuleCtrlGPIOUnitIOItem::GetState(int& nState)
{
	CHwModuleCtrlGPIOUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlGPIOUnitDrv*>(m_pUnit->GetDriver());

	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->GetGPIOState(m_nIndex,nState);
}

BOOL CSUB20HwModuleCtrlGPIOUnitIOItem::SetState(int  nState)
{
	CHwModuleCtrlGPIOUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlGPIOUnitDrv*>(m_pUnit->GetDriver());

	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->SetGPIOState(m_nIndex,nState);
}

BOOL CSUB20HwModuleCtrlGPIOUnitIOItem::SetType(int  nType)
{
	CHwModuleCtrlGPIOUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlGPIOUnitDrv*>(m_pUnit->GetDriver());

	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->SetGPIOType(m_nIndex,nType);
}

BOOL CSUB20HwModuleCtrlGPIOUnitIOItem::GetType(int&  nType)
{
	CHwModuleCtrlGPIOUnitDrv* pDrv = dynamic_cast<CHwModuleCtrlGPIOUnitDrv*>(m_pUnit->GetDriver());

	if(pDrv == NULL)
	{
		return FALSE;
	}
	return pDrv->GetGPIOType(m_nIndex,nType);
}

BOOL CSUB20HwModuleCtrlGPIOUnitIOItem::SetConfig(CGPIOConfig cfg)
{
	if(SetType(cfg.m_nType) == FALSE)
	{
		return FALSE;
	}
	if(cfg.m_nType == GPIO_TYPE_OUTPUT)
	{
		if(SetState(cfg.m_nState) == FALSE)
		{
			return FALSE;
		}
	}
	SetDisplayName( cfg.m_sDisplayName );
	m_bEnable = cfg.m_bEnable;
	m_sDisplayName = cfg.m_sDisplayName;
	return TRUE;
}

BOOL  CSUB20HwModuleCtrlGPIOUnitIOItem::GetConfig(CGPIOConfig& cfg)
{
	cfg.m_sDisplayName = m_sDisplayName;
	if(GetType(cfg.m_nType) == FALSE)
	{
		return FALSE;
	}
	if(GetState(cfg.m_nState) == FALSE)
	{
		return FALSE;
	}
	cfg.m_bEnable = m_bEnable;
	return TRUE;
}

CString	CSUB20HwModuleCtrlGPIOUnitIOItem::Serialize()
{
	CMarkup			xmlWriter;
	CGPIOConfig     cfg;

	if(GetConfig(cfg) == FALSE)
		return FALSE;

	xmlWriter.AddElem( "GPIO");
	xmlWriter.AddAttrib( "Index",m_nIndex);
	xmlWriter.AddAttrib( "Name",m_sName);
	xmlWriter.AddAttrib( "DisplayName",cfg.m_sDisplayName);
	xmlWriter.AddAttrib( "Type",cfg.m_nType);
	xmlWriter.AddAttrib( "State",cfg.m_nState);
	xmlWriter.AddAttrib( "Enable",(cfg.m_bEnable==TRUE)? "TRUE":"FALSE");

	return xmlWriter.GetDoc();
}

BOOL	CSUB20HwModuleCtrlGPIOUnitIOItem::DeSerialize(CMarkup& xmlReader)
{
	CGPIOConfig     cfg;
	CString         sName,sDisplayName,sType,sState, sEnable;

	sName        = xmlReader.GetAttrib("Name");
	sDisplayName = xmlReader.GetAttrib("DisplayName");
	sType        = xmlReader.GetAttrib("Type");
	sState       = xmlReader.GetAttrib("State");
	sEnable      = xmlReader.GetAttrib("Enable");
	
	sscanf(sType,"%d",&cfg.m_nType);
	sscanf(sState,"%d",&cfg.m_nState);
	cfg.m_sDisplayName = sDisplayName;

	cfg.m_bEnable = FALSE;
	if(sEnable == "TRUE")
		cfg.m_bEnable = TRUE;

	return SetConfig(cfg);
}
