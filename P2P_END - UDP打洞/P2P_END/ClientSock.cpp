#include "StdAfx.h"
#include "ClientSock.h"
#include "P2P_ENDDlg.h"

CClientSock::CClientSock(void)
{
}

CClientSock::~CClientSock(void)
{
}
void CClientSock::SetDlg(CP2P_ENDDlg *p_Dlg) 
{
	m_ClientDlg = p_Dlg;
}
void CClientSock::OnReceive(int nErrorCode)
{
	m_ClientDlg->ReceiveData(this);
	CSocket::OnReceive(nErrorCode);
}

void CClientSock::OnClose(int nErrorCode)
{
	if(this->m_ClientDlg)
	{
		MessageBox(NULL,"服务器断开连接!","系统",MB_OK);
		this->m_ClientDlg->SendMessage(WM_CLOSE,0,0);
	}
	this->Close();
	CSocket::OnClose(nErrorCode);
}