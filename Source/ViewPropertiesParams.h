#pragma once

#include "ConnectionParams.h"
#include "BootLoaderParams.h"
#include "CmdHistoryParams.h"
#include "SendMailParams.h"
#include "ViewMessagingParams.h"

class CViewPropertiesParams
{
public:

	CConnectionParams             m_connectionParams;
	CBootLoaderParams	          m_bootLoaderParams;
	CCmdHistoryParams             m_cmdHistoryParams;
	CSendMailParams               m_sendMailParams;
	CViewMessagingParams          m_viewMessagingParams;

	CViewPropertiesParams(void);
	virtual ~CViewPropertiesParams(void);
};
