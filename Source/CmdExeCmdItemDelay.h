#pragma once
#include "CmdExeCmdItem.h"

typedef enum _cmdExeCmdItemDelayType 
{
	CMD_EXE_ITEM_DELAY_FIXED_VALUE_DELAY,
	CMD_EXE_ITEM_DELAY_RANDOM_VALUE_DELAY

}CMD_EXE_CMD_ITEM_DELAY_TYPE;

class CCmdExeCmdItemDelayParams
{
public:
	CMD_EXE_CMD_ITEM_DELAY_TYPE  m_type;
	int                          m_nVal;
	int                          m_nHighVal;
};

class CCmdExeCmdItemDelay : public CCmdExeCmdItem
{
	CCmdExeCmdItemDelayParams m_params;

	VOID			Delay(UINT nMiliSec);

public:
	CCmdExeCmdItemDelay();
	virtual ~CCmdExeCmdItemDelay(void);
	virtual  BOOL						Execute();
	virtual  CCmdExeItem*				Clone();
	virtual BOOL	    				Serialize(CMarkup & xmlWriter);
	virtual BOOL	    				DeSerialize(CMarkup & xmlReader);
	CCmdExeCmdItemDelayParams			GetParams() {return m_params;}
	void								SetParams(CCmdExeCmdItemDelayParams params) {m_params = params;}

};
