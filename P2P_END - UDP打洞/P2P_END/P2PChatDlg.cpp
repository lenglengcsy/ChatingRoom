// P2PChatDlg.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "P2P_END.h"
#include "P2PChatDlg.h"



// CP2PChatDlg 对话框

IMPLEMENT_DYNAMIC(CP2PChatDlg, CDialog)

CP2PChatDlg::CP2PChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CP2PChatDlg::IDD, pParent)
{
	
}

CP2PChatDlg::~CP2PChatDlg()
{
}

void CP2PChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RECV, m_PeerRecv);
	DDX_Control(pDX, IDC_EDIT_P2PSEND, m_PeerSend);
}

void CP2PChatDlg::SetParentDlg(CP2P_ENDDlg* p_Parent)
{
	m_pParent = p_Parent;
}
BEGIN_MESSAGE_MAP(CP2PChatDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CP2PChatDlg::OnHide)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CP2PChatDlg::OnPeerSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CP2PChatDlg::OnClear)
END_MESSAGE_MAP()


// CP2PChatDlg 消息处理程序

void CP2PChatDlg::OnHide()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
	isActive = false;
}


void CP2PChatDlg::OnPeerSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSendMsg;
	POSITION pos = m_pParent->m_P2PChatList.GetHeadPosition();
	this->GetDlgItem(IDC_EDIT_P2PSEND)->GetWindowText(strSendMsg);
	while(pos!=NULL)
	{
		PeerData *p_Peer = (PeerData*)m_pParent->m_P2PChatList.GetNext(pos);
		if (!strcmp(p_Peer->peerName,m_PeerName))
		{
			AddRecvMsg(&(p_Peer->peerMsgLog),m_pParent->m_UserData.userName,&strSendMsg);//自己发送的信息先显示
			m_PeerRecv.SetWindowText(p_Peer->peerMsgLog);		//	更新的peer日志
			strcpy(m_SaveMsg.m_UserName,m_pParent->m_UserData.userName);
			strcpy(m_SaveMsg.chat,strSendMsg.GetBuffer());//	保存等待发送
			strSendMsg.ReleaseBuffer();

			m_pParent->m_ClientSock.SendTo((char*)&m_SaveMsg, sizeof(m_SaveMsg),(SOCKADDR*)&(p_Peer->peerAddr),sizeof(SOCKADDR_IN),0);
		}
	}
	m_PeerSend.SetWindowText("");
	m_PeerRecv.LineScroll(m_PeerRecv.GetLineCount());

}

void CP2PChatDlg::AddRecvMsg(CString *p_MsgLog,char *userName,CString *chat)
{
	CTime t=CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%d:%d:%d",t.GetHour(),t.GetMinute(),t.GetSecond());

	*p_MsgLog+=userName;
	*p_MsgLog+="\t";
	*p_MsgLog+=strTime;
	*p_MsgLog+="\r\n";
	*p_MsgLog+=*chat;
	*p_MsgLog+="\r\n";
	*p_MsgLog+="\r\n";
}

void CP2PChatDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_pParent->m_P2PChatList.GetHeadPosition();
	while(pos!=NULL)
	{
		PeerData *p_Peer = (PeerData*)m_pParent->m_P2PChatList.GetNext(pos);
		if (!strcmp(p_Peer->peerName,m_PeerName))
		{
			p_Peer->peerMsgLog = "";
			m_PeerRecv.SetWindowText(p_Peer->peerMsgLog);		//	更新的peer日志
		}
	}
	

}
