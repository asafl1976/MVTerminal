#pragma once

class CHwModuleCtrlDrv
{
protected:
	BOOL	      m_bErrMsgEnable;

public:
	CHwModuleCtrlDrv(void);
	virtual ~CHwModuleCtrlDrv(void);
	void			 EnableErrorMessages(BOOL bEnable) {m_bErrMsgEnable = bEnable;}
	virtual BOOL     Load()    = 0;
	virtual void     Unload()  = 0;
};
