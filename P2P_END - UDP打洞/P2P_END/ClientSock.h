#pragma once
#include "afxsock.h"
//#include "P2P_ENDDlg.h"

class CP2P_ENDDlg;
class CClientSock :	public CSocket
{
public:
	CP2P_ENDDlg *m_ClientDlg;

public:
	CClientSock(void);
	~CClientSock(void);
	

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void SetDlg(CP2P_ENDDlg* m_ClientDlg);
};
