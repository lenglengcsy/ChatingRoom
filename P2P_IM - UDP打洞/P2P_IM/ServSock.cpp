#include "stdafx.h"
#include "ServSock.h"
#include "P2P_IMDlg.h"

CServSock::CServSock()
{

}

CServSock::~CServSock()
{
}

void CServSock::SetDlg(CP2P_IMDlg *p_Dlg) 
{
	m_ServDlg = p_Dlg;
}

// void CServSock::OnAccept(int nErrorCode) 
// {
// 	m_ServDlg->AcceptConnect();
// 	CSocket::OnAccept(nErrorCode);
// }

void CServSock::OnClose(int nErrorCode) 
{
	//m_ServDlg->Stop(this);
	CSocket::OnClose(nErrorCode);
}

void CServSock::OnReceive(int nErrorCode) 
{
	m_ServDlg->ReceiveData(this);
	CSocket::OnReceive(nErrorCode);
}
