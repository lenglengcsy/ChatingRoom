#pragma once
#ifndef TESTCLASS_H
#define TESTCLASS_H

#include "P2PBase.h"
#include "P2P_ENDDlg.h"
#include "afxwin.h"
// CP2PChatDlg 对话框
class CP2P_ENDDlg;
class CP2PChatDlg : public CDialog
{
	DECLARE_DYNAMIC(CP2PChatDlg)

public:
	CP2PChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PChatDlg();
	char m_PeerName[20] ;  //用于回父窗口检索数据
	CP2P_ENDDlg *m_pParent;	
	//CString m_PeerRecv;
	//CString m_PeerSend;
	Message m_SaveMsg;
	BOOL isActive;
	void SetParentDlg(CP2P_ENDDlg*);
	void AddRecvMsg(CString *p_MsgLog,char *userName,CString *chat);
// 对话框数据
	enum { IDD = IDD_P2PCHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHide();

	afx_msg void OnPeerSend();
	CEdit m_PeerRecv;
	CEdit m_PeerSend;
	afx_msg void OnClear();
};
#endif