// P2P_IMDlg.h : 头文件
//
#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "SQL.h"
#include "ServSock.h"
#include "P2PBase.h"
#include "P2P_IM.h"


#include "afxwin.h"

// CP2P_IMDlg 对话框
class CP2P_IMDlg : public CDialog
{
// 构造
public:
	CP2P_IMDlg(CWnd* pParent = NULL);	// 标准构造函数
	void refresh();  //刷新服务器界面
	void UpdateUserList(Msg *buffer);	//更新在线用户列表发给用户
	void ReceiveData(CServSock* sock);
	void Stop(int id);
	void AcceptConnect();
	UserData *FindClientByName(char *);
	BOOL ADOCheckUser(Message *p_Userbuffer,int sign);
	ServerData m_ServData;
	CServSock m_ServSock;
	Msg m_ServBaseMsg;				//用来储存在线用户列表，方便发送
	SOCKADDR_IN m_clientAddr;
	CPtrList m_SockUserList;
	CDataBaseAcess AdoAccess;
	
// 对话框数据
	enum { IDD = IDD_P2P_IM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_OLUserList;
	CStatic m_OLNum;
	CStatic m_State;
	CEdit m_ServerLog;
	afx_msg void OnStart();
	
	afx_msg void OnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
