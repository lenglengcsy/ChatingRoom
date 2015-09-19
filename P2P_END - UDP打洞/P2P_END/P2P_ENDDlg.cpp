// P2P_ENDDlg.cpp : 实现文件
//
#pragma once

#include <ctime>
#include "stdafx.h"
#include "P2P_END.h"
#include "P2P_ENDDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CP2P_ENDDlg 对话框




void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

CP2P_ENDDlg::CP2P_ENDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CP2P_ENDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CP2P_ENDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOGIN, m_UserName);
	DDX_Control(pDX, IDC_EDIT_SERVMSG, m_ServMsg);
	DDX_Control(pDX, IDC_EDIT_SEND, m_ServSend);
	DDX_Control(pDX, IDC_EDIT_SERVIP, m_ServIP);
	DDX_Control(pDX, IDC_EDIT_SERVPORT, m_ServPort);
	DDX_Control(pDX, IDC_LOGIN, m_LoginButton);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_ExitButton);
	DDX_Control(pDX, IDC_SERVSEND, m_SendButton);
	DDX_Control(pDX, IDC_LIST1, m_OLUserList);
	//DDX_Control(pDX, IDC_USERLIST, m_UserListBox);
	DDX_Control(pDX, IDC_EDIT_PSW, m_Password);
}

BEGIN_MESSAGE_MAP(CP2P_ENDDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CP2P_ENDDlg::OnLogin)
	ON_BN_CLICKED(IDC_SERVSEND, &CP2P_ENDDlg::OnServSend)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CP2P_ENDDlg::OnExit)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST1, &CP2P_ENDDlg::OnUserList)
	ON_EN_CHANGE(IDC_EDIT_SERVPORT, &CP2P_ENDDlg::OnEnChangeEditServport)
	ON_BN_CLICKED(IDC_CANEL, &CP2P_ENDDlg::OnCanel)
	ON_BN_CLICKED(IDC_SETTING, &CP2P_ENDDlg::OnSetting)
	ON_BN_CLICKED(IDC_FORGET, &CP2P_ENDDlg::OnForget)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CP2P_ENDDlg::OnClear)
	ON_BN_CLICKED(IDC_REG1, &CP2P_ENDDlg::OnReg1)
	ON_BN_CLICKED(IDC_REG2, &CP2P_ENDDlg::OnReg2)
	ON_BN_CLICKED(IDC_BACK, &CP2P_ENDDlg::OnBack)
	ON_BN_CLICKED(IDC_FORGET2, &CP2P_ENDDlg::OnForget2)
	ON_EN_CHANGE(IDC_EDIT_IDCARD, &CP2P_ENDDlg::OnEnChangeEditIdcard)
END_MESSAGE_MAP()


// CP2P_ENDDlg 消息处理程序

BOOL CP2P_ENDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_UserData.strPort = "1000";
	m_UserData.strIP = "127.0.0.1";
	m_ServIP.SetWindowText("127.0.0.1");
	m_ServPort.SetWindowText("1000");
	m_ExitButton.EnableWindow(FALSE);
	m_SendButton.EnableWindow(FALSE);
	m_State = FALSE ;
	   //为对话框分配空间  否则create不出来 - -
	m_Extend=FALSE;
	this->MoveWindow(470,260,327,230,1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CP2P_ENDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CP2P_ENDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CP2P_ENDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CP2P_ENDDlg::OnLogin()
{
	CString strName;
	CString strPSW;
	///CString strChat;
	//m_State = TRUE ;
	this->GetDlgItem(IDC_EDIT_LOGIN)->GetWindowText(strName);
	this->GetDlgItem(IDC_EDIT_PSW)->GetWindowText(strPSW);
	this->GetDlgItem(IDC_EDIT_SERVIP)->GetWindowText(m_UserData.strIP);
	this->GetDlgItem(IDC_EDIT_SERVPORT)->GetWindowText(m_UserData.strPort);
	strcpy(m_UserData.userName,strName.GetBuffer());
	strName.ReleaseBuffer();
	strcpy(m_ClientMsg.m_UserName,m_UserData.userName);
	strcpy(m_ClientMsg.m_PSW,strPSW.GetBuffer());
	strPSW.ReleaseBuffer();
	strName+=" 上线了！！！";
	strcpy(m_ClientMsg.chat,strName.GetBuffer());
	strName.ReleaseBuffer();
	m_ClientMsg.sign = 1;
	m_ClientMsg.id = 1;
	m_ClientSock.SetDlg(this);
	if (strName.GetLength()==0)
	{
		MessageBox("请输入用户名");
		return;
	}
	if(m_UserData.strIP.GetLength()==0||m_UserData.strPort.GetLength()==0)
	{
		MessageBox("请输入IP和端口号");
		return;
	}
	if(!m_ClientSock.Create(0,SOCK_DGRAM))		//	0表示客户端随机端口
	{
		char szMsg[1024] = {0};
		sprintf(szMsg, "create faild: %d", m_ClientSock.GetLastError());
		AfxMessageBox(szMsg);
		return;
	}
	int nPort = atoi(m_UserData.strPort);
	m_ClientSock.SendTo((char*)&m_ClientMsg, sizeof(m_ClientMsg),nPort,m_UserData.strIP,0);
	m_LoginButton.EnableWindow(FALSE); //按过一次不能再按了，因为客户端创建的连接还没关闭
	
	SetTimer(1,10000,NULL);   //设置定时器。判断登陆超时
	SetTimer(2,40000,NULL);   //设置定时器。判断是否与服务器保持连接
	//m_ClientSock.Close();
	
	//MessageBox("连接成功");
	
}

void CP2P_ENDDlg::ReceiveData(CClientSock *sock)
{
	Message buffer;
	CString strName;
	CString strChat;
	CString str;
	CString strTime;  
	POSITION pos;
	PeerData *p_PeerData =NULL;
	int len = sizeof(SOCKADDR_IN);
	this->GetDlgItem(IDC_EDIT_LOGIN)->GetWindowText(strName);
	//CTime t=CTime::GetCurrentTime();
	//strTime.Format("%d:%d:%d",t.GetHour(),t.GetMinute(),t.GetSecond());
 	sock->ReceiveFrom((char*)&buffer,sizeof(buffer),(SOCKADDR*)&(this->m_ServAddr),&len,0);
	
	if (buffer.sign!=P2PCHATMSG)  //p2p信息不更新在线列表
	{
		UpdateUserList(&buffer);
	}
	
	switch (buffer.sign)
	{
	case LOGINMSG:
		if (buffer.servACK==-1)
		{
			MessageBox(_T("用户名或密码错误！！"));
			m_LoginButton.EnableWindow(TRUE);
			m_ClientSock.Close();
			return;
		}
		PlaySound((LPCTSTR)IDR_LOGINMSG, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音

		//if (buffer.servACK==1000)
		//{
			m_State = TRUE;
			m_UserData.id = buffer.id;
			m_UserData.servACK = buffer.servACK;
			m_LoginButton.EnableWindow(FALSE);
			m_ExitButton.EnableWindow(TRUE);
			m_SendButton.EnableWindow(TRUE);
			m_ServIP.EnableWindow(FALSE);
			m_ServPort.EnableWindow(FALSE);
			m_UserName.EnableWindow(FALSE);
			this->SetWindowText(strName+"的P2P聊天室");
			MessageBox("连接成功");
		//}
			LoginToChat();
		break;

	case MULCHATMSG:
		if (strcmp(buffer.m_UserName,m_UserData.userName))
		{
			PlaySound((LPCTSTR)IDR_MULCHATMSG, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音
		}
		

		//strChat = buffer.chat;
		/*m_UserData.ServChat+=_T(buffer.m_UserName);
		m_UserData.ServChat+="\t";
		m_UserData.ServChat+=strTime;
		m_UserData.ServChat+="\r\n";
		m_UserData.ServChat+=buffer.chat;
		m_UserData.ServChat+="\r\n";
		m_UserData.ServChat+="\r\n";*/
		AddRecvMsg(&(m_UserData.ServChat),&buffer);
		m_ServMsg.SetWindowText(m_UserData.ServChat);
		m_ServMsg.LineScroll(m_ServMsg.GetLineCount());		//	 自动滚到最底下
		break;

	case EXITMSG: 
		PlaySound((LPCTSTR)IDR_EXITMSG, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音
		if (buffer.servACK==0)
		{
			m_ClientSock.Close();
			m_State = FALSE;
			m_LoginButton.EnableWindow(TRUE);
			m_ExitButton.EnableWindow(FALSE);
			m_SendButton.EnableWindow(FALSE);
			m_ServIP.EnableWindow(TRUE);
			m_ServPort.EnableWindow(TRUE);
			m_UserName.EnableWindow(TRUE);
			m_OLUserList.ResetContent();	//移出列表中的内容

			pos = m_P2PChatList.GetHeadPosition();
			PeerData *p_PeerData = NULL;
			while(pos!=NULL)
			{
				p_PeerData = (PeerData*)m_P2PChatList.GetNext(pos);
				delete p_PeerData;
			}
			
		}
		MessageBox(_T("与服务器断开连接，程序即将退出！"));
		OnCancel();
		break;
	case ISALIVEMSG:
		//Msg buffer ;
		//buffer.sign = ISALIVEMSG;
		buffer.id = m_UserData.id;
		m_UserData.servACK = buffer.servACK;
		m_ClientSock.SendTo((char*)&buffer, sizeof(buffer),atoi(m_UserData.strPort),m_UserData.strIP,0);
		break;

	case PEERLOGINMSG:
		PlaySound((LPCTSTR)IDR_UPLINE, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音
		AddRecvMsg(&(m_UserData.ServChat),&buffer);
		m_ServMsg.SetWindowText(m_UserData.ServChat);
		m_ServMsg.LineScroll(m_ServMsg.GetLineCount());		//	 自动滚到最底下
		break;

	case P2PHOLETO:				//接受服务器传回来的peer地址
		//buffer.sign = P2PCHATMSG;
		//ShowP2PChatDlg(&buffer);
	//	MessageBox(buffer.m_UserName);
		CreateP2PDlg(&buffer);
		break;

	case P2PCHATMSG:		//接受peer发送过来的信息.
							//一来消息，对应的CLient就亮。存在则追加数据，不存在则创建对话框。用户双击了才显示
		PlaySound((LPCTSTR)IDR_PEERMSG, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音
		buffer.userAddr  =  m_ServAddr;   //p2pmsg肯定是client发过来的
		p_PeerData = FindP2PChatDlgByName(buffer.m_UserName);
		if (p_PeerData!=NULL)
		{
			
			if (p_PeerData->peerChatDlg->isActive)
			{
				ShowP2PChatDlg(&buffer);	//	窗口已存在并显示，追加内容
			}
			else
			{
				p_PeerData->isRead = FALSE;
				UserListLightUp(TRUE,buffer.m_UserName);	//窗口已存在未显示，高亮它，追加内容
				ShowP2PChatDlg(&buffer);
			}
		}
		else
		{
			UserListLightUp(TRUE,buffer.m_UserName);
			CreateP2PDlg(&buffer); //创建并追加内容
		}
		
		
		break;
	case REGMSG:
		KillTimer(1);
		if (buffer.servACK==-1)
		{
			MessageBox(_T("用户名已被注册！！"));
			this->GetDlgItem(IDC_REG2)->EnableWindow(TRUE);
			m_ClientSock.Close();
			return;
		}
		else
		{
			MessageBox(_T("注册成功！！"));
			this->GetDlgItem(IDC_REG2)->EnableWindow(TRUE);
			m_ClientSock.Close();
			GetDlgItem(IDC_LOGIN)->ShowWindow(TRUE);
			GetDlgItem(IDC_REG1)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHECK1)->ShowWindow(TRUE);
			GetDlgItem(IDC_FORGET)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHECK2)->ShowWindow(TRUE);
			GetDlgItem(IDC_CANEL)->ShowWindow(TRUE);
			GetDlgItem(IDC_REG2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_IDCARD)->ShowWindow(FALSE);
			GetDlgItem(IDC_EDIT_IDCARD)->ShowWindow(FALSE);
			GetDlgItem(IDC_BACK)->ShowWindow(FALSE);

			return;
		}
		break;
	case FORGETMSG:
		KillTimer(1);
		if (buffer.servACK==-1)
		{
			MessageBox(_T("资料填写错误！！"));
			this->GetDlgItem(IDC_FORGET2)->EnableWindow(TRUE);
			m_ClientSock.Close();
			return;
		}
		else
		{
			MessageBox(_T("密码找回成功！！"));
			m_Password.SetPasswordChar(NULL);
			GetDlgItem(IDC_EDIT_PSW)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_PSW)->SetWindowText(buffer.m_PSW);
			GetDlgItem(IDC_FORGET2)->EnableWindow(TRUE);
			m_ClientSock.Close();
			return;
		}
		break;
	default:;

	};
	KillTimer(2);				// 重置定时器时间
	SetTimer(2,40000,NULL);
}
void CP2P_ENDDlg::UserListLightUp(BOOL flag,char *userName) //flag为真时候高亮
{
	int index ;
	index = m_OLUserList.FindString(0,userName);
	m_OLUserList.SetSel(index,TRUE);
}

void CP2P_ENDDlg::ShowP2PChatDlg(Message *p_PeerBuffer)
{
	//判断对话是否存在于列表
	CString userName = p_PeerBuffer->m_UserName;	//	对方的名字
	POSITION pos = m_P2PChatList.GetHeadPosition();
	PeerData *p_PeerData = NULL;
	p_PeerData = FindP2PChatDlgByName(p_PeerBuffer->m_UserName);
	if (!strcmp(p_PeerData->peerName,userName))
	{
		//
		//if (p_PeerData->peerChatDlg->isActive==FALSE)
		//{
		//	p_PeerData->peerChatDlg->ShowWindow(SW_SHOW);	//	存在的话  show他
		//}
		AddRecvMsg(&(p_PeerData->peerMsgLog),p_PeerBuffer);
		p_PeerData->peerChatDlg->m_PeerRecv.SetWindowText(p_PeerData->peerMsgLog);
		p_PeerData->peerChatDlg->m_PeerRecv.LineScroll(p_PeerData->peerChatDlg->m_PeerRecv.GetLineCount());
		return;	
	}

	//不存在  创建新的DLG
	/*m_ChatMsgDlg = new CP2PChatDlg();
	m_ChatMsgDlg->Create(IDD_P2PCHAT,this);
	m_ChatMsgDlg->ModifyStyleEx(0,  WS_EX_APPWINDOW);//任务栏显示
	m_ChatMsgDlg->ShowWindow(SW_SHOW);
	m_ChatMsgDlg->SetWindowText("正在与"+userName+"交谈中");
	m_ChatMsgDlg->SetParentDlg(this);				//传入父窗口的指针
	m_ChatMsgDlg->m_SaveMsg = *p_PeerBuffer;
	m_ChatMsgDlg->m_SaveMsg.sign = P2PCHATMSG;

	strcpy(m_ChatMsgDlg->m_SaveMsg.m_UserName,m_UserData.userName);
	strcpy(m_ChatMsgDlg->m_PeerName,userName);

	//m_ChatMsgDlg->m_ParentMsg = *p_PeerBuffer;
	//p_PeerData = NULL;				//容器循环后经常忘记把指针重置 导致容器中都变东西
	p_PeerData = new PeerData();
	p_PeerData->peerAddr = p_PeerBuffer->userAddr;
	p_PeerData->state = true;
	strcpy(p_PeerData->peerName,userName);
	p_PeerData->peerChatDlg = m_ChatMsgDlg;
	if (p_PeerBuffer->sign==P2PHOLETO)
	{
	p_PeerData->peerMsgLog = "请求聊天成功\r\n";
	strcpy(p_PeerData->peerChatDlg->m_SaveMsg.chat,"请求聊天成功\r\n");
	p_PeerData->peerChatDlg->m_PeerRecv.SetWindowText(p_PeerData->peerMsgLog);
	}
	else
	{
	AddRecvMsg(&(p_PeerData->peerMsgLog),p_PeerBuffer);
	p_PeerData->peerChatDlg->m_PeerRecv.SetWindowText(p_PeerData->peerMsgLog);
	}
	//	追加聊天记录
	m_P2PChatList.AddTail(p_PeerData); //加入到对话链表里头
	m_ChatMsgDlg = NULL;*/
}

void CP2P_ENDDlg::OnServSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	Message buffer ;
	int nPort ;
	
	if (m_State)
	{
		this->GetDlgItem(IDC_EDIT_SEND)->GetWindowText(strText);
		if (strText=="")
		{
			MessageBox("请输入内容！");
			return;
		}
		m_ClientSock.SetDlg(this);
		strcpy(buffer.m_UserName,m_UserData.userName);
		strcpy(buffer.chat,strText.GetBuffer());	strText.ReleaseBuffer();
		buffer.sign = 2;
		buffer.id = m_UserData.id;
		m_ServSend.SetWindowText("");
		nPort = atoi(m_UserData.strPort);
		m_ClientSock.SendTo((char*)&buffer, sizeof(buffer),nPort,m_UserData.strIP,0);
	}
	else
	{
		MessageBox("请先登录！");
		return;
	}
	
	

	//m_ClientSock.Close();

	//MessageBox("连接成功");
}

void CP2P_ENDDlg::OnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	Message buffer ;
	buffer.sign = EXITMSG;
	buffer.id = m_UserData.id;
	m_ClientSock.SendTo((char*)&buffer, sizeof(buffer),atoi(m_UserData.strPort),m_UserData.strIP,0);
}

void CP2P_ENDDlg::UpdateUserList(Message *buffer)
{
	PeerData *p_PeerData=NULL;
	POSITION pos = m_P2PChatList.GetHeadPosition();
	m_OLUserList.ResetContent();
	for(int i=0 ;i<buffer->userNum ; i++ )
	{
		strcpy(m_ClientMsg.OLUserList[i],buffer->OLUserList[i]);
		m_OLUserList.AddString(m_ClientMsg.OLUserList[i]);//更新在线用户列表
	}
	while (pos!=NULL)
	{
		p_PeerData = (PeerData *)m_P2PChatList.GetNext(pos);
		if (p_PeerData->isRead==FALSE)
		{
			UserListLightUp(TRUE,p_PeerData->peerName);
		}
	}

}

void CP2P_ENDDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		KillTimer(1);
		if (!m_State)
		{
			MessageBox("连接服务器超时！");
			m_ClientSock.Close();
			m_LoginButton.EnableWindow(TRUE);
			GetDlgItem(IDC_FORGET2)->EnableWindow(TRUE);
			GetDlgItem(IDC_REG2)->EnableWindow(TRUE);
		}
		break;
	case 2:
		KillTimer(2);
		MessageBox("与服务器丢失连接！");
		//this->CloseWindow();
		m_ClientSock.Close();
		m_LoginButton.EnableWindow(TRUE);
		break;
	default:;
	}
	CDialog::OnTimer(nIDEvent);
}

void CP2P_ENDDlg::OnUserList()
{
	// TODO: 在此添加控件通知处理程序代码

	//是否向服务器发一条P2P请求
	Message buffer;
	int listCount;
	int nPort = atoi(m_UserData.strPort);
	PeerData *p_PeerData;
	buffer.sign = P2PHOLETO;
	buffer.id = m_UserData.id;
	listCount = m_OLUserList.GetCurSel();
	m_OLUserList.GetText(listCount,buffer.m_UserName);
	p_PeerData=FindP2PChatDlgByName(buffer.m_UserName);	//	查找是否已存在对话框
	if (p_PeerData!=NULL)		//存在了，因为被双击，直接SHOW他
	{
		p_PeerData->isRead = TRUE;
		p_PeerData->peerChatDlg->ShowWindow(SW_SHOW);
		m_OLUserList.SetSel(listCount,FALSE);
		return;
	}
	m_ClientSock.SendTo((char*)&buffer, sizeof(buffer),nPort,m_UserData.strIP,0);//不存在发送打洞请求
	
}


void CP2P_ENDDlg::RecvP2PMsg(Message *p_PeerBuffer)
{

}

PeerData* CP2P_ENDDlg::FindP2PChatDlgByName(char *userName)
{													//	对方的名字
	POSITION pos = m_P2PChatList.GetHeadPosition();
	PeerData *p_PeerData = NULL;
	p_PeerData = new PeerData();		//	不分配空间就无法给里面的变量赋值；
	while(pos!=NULL)
	{
		p_PeerData = (PeerData*)m_P2PChatList.GetNext(pos);
		if (!strcmp(p_PeerData->peerName ,userName))
		{
			return p_PeerData;	
		}
		//isPeerExist = true;
	}
	return NULL;
}

void CP2P_ENDDlg::AddRecvMsg(CString *p_MsgLog,Message *p_MsgBuffer)
{
	CTime t=CTime::GetCurrentTime();
	CString strTime;
	strTime.Format("%d:%d:%d",t.GetHour(),t.GetMinute(),t.GetSecond());

	*p_MsgLog+=p_MsgBuffer->m_UserName;
	*p_MsgLog+="\t";
	*p_MsgLog+=strTime;
	*p_MsgLog+="\r\n";
	*p_MsgLog+=p_MsgBuffer->chat;
	*p_MsgLog+="\r\n";
	*p_MsgLog+="\r\n";

}

void CP2P_ENDDlg::CreateP2PDlg(Message *p_PeerBuffer)
{
	CString userName = p_PeerBuffer->m_UserName;	//	对方的名字
	POSITION pos = m_P2PChatList.GetHeadPosition();
	PeerData *p_PeerData = NULL;
	m_ChatMsgDlg = new CP2PChatDlg();
	m_ChatMsgDlg->Create(IDD_P2PCHAT,this);
	m_ChatMsgDlg->ModifyStyleEx(0,  WS_EX_APPWINDOW);//任务栏显示
	
	m_ChatMsgDlg->SetWindowText("正在与"+userName+"交谈中");
	m_ChatMsgDlg->SetParentDlg(this);				//传入父窗口的指针
	m_ChatMsgDlg->m_SaveMsg = *p_PeerBuffer;
	m_ChatMsgDlg->m_SaveMsg.sign = P2PCHATMSG;
	m_ChatMsgDlg->isActive = FALSE;
	strcpy(m_ChatMsgDlg->m_SaveMsg.m_UserName,m_UserData.userName);
	strcpy(m_ChatMsgDlg->m_PeerName,userName);

	//m_ChatMsgDlg->m_ParentMsg = *p_PeerBuffer;
	//p_PeerData = NULL;				//容器循环后经常忘记把指针重置 导致容器中都变东西
	p_PeerData = new PeerData();
	p_PeerData->peerAddr = p_PeerBuffer->userAddr;
	p_PeerData->state = true;
	p_PeerData->isRead = FALSE;
	strcpy(p_PeerData->peerName,userName);
	p_PeerData->peerChatDlg = m_ChatMsgDlg;
	if (p_PeerBuffer->sign==P2PHOLETO)
	{
		m_ChatMsgDlg->ShowWindow(SW_SHOW);
		m_ChatMsgDlg->isActive = TRUE;
		p_PeerData->isRead = TRUE;
		p_PeerData->peerMsgLog = "请求聊天成功\r\n";
		strcpy(p_PeerData->peerChatDlg->m_SaveMsg.chat,"请求聊天成功\r\n");
		p_PeerData->peerChatDlg->m_PeerRecv.SetWindowText(p_PeerData->peerMsgLog);
	}
	else
	{
		AddRecvMsg(&(p_PeerData->peerMsgLog),p_PeerBuffer);
		p_PeerData->peerChatDlg->m_PeerRecv.SetWindowText(p_PeerData->peerMsgLog);
	}
	//	追加聊天记录
	m_P2PChatList.AddTail(p_PeerData); //加入到对话链表里头
	m_ChatMsgDlg = NULL;
}

void CP2P_ENDDlg::OnEnChangeEditServport()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CP2P_ENDDlg::OnCanel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCancel();
}

void CP2P_ENDDlg::OnSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	PlaySound((LPCTSTR)IDR_FOLDER, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);		//播放声音

	if(m_Extend)
	{
		this->MoveWindow(470,260,327,230,1);
		GetDlgItem(IDC_SETTING)->SetWindowText("高级设置↓");		
		m_Extend=FALSE;
	}
	else
	{
		this->MoveWindow(470,260,327,360,1);
		GetDlgItem(IDC_SETTING)->SetWindowText("高级设置↑");
		m_Extend=TRUE;
	}
}

void CP2P_ENDDlg::OnForget()
{
	GetDlgItem(IDC_LOGIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_REG1)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(FALSE);
	GetDlgItem(IDC_FORGET)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->ShowWindow(FALSE);
	GetDlgItem(IDC_CANEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_FORGET2)->ShowWindow(TRUE);
	GetDlgItem(IDC_BACK)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_IDCARD)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_IDCARD)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_PSW)->EnableWindow(FALSE);
}

void CP2P_ENDDlg::LoginToChat()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_SETTING)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_LOGIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_PSW)->ShowWindow(FALSE);
	GetDlgItem(IDC_LOGIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_REG1)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(FALSE);
	GetDlgItem(IDC_FORGET)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->ShowWindow(FALSE);
	GetDlgItem(IDC_CANEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_BOX1)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_PSW)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVIP)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SERVPORT)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_BOX2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_IP)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_PORT)->ShowWindow(FALSE);
	MoveWindow(350,200,600,400,1);
	GetDlgItem(IDC_BUTTON_EXIT)->SetWindowPos( NULL,500,10,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_EDIT_SERVMSG)->SetWindowPos( NULL,10,60,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_EDIT_SEND)->SetWindowPos( NULL,10,280,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_LIST1)->SetWindowPos( NULL,450,80,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_BUTTON_CLEAR)->SetWindowPos( NULL,340,255,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_STATIC_LIST)->SetWindowPos( NULL,450,60,0,0,SWP_NOZORDER | SWP_NOSIZE ); 
	GetDlgItem(IDC_SERVSEND)->SetWindowPos( NULL,420,340,0,0,SWP_NOZORDER | SWP_NOSIZE ); 

}
void CP2P_ENDDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_UserData.ServChat = "";
	GetDlgItem(IDC_EDIT_SERVMSG)->SetWindowText("");

}

void CP2P_ENDDlg::OnReg1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_LOGIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_REG1)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(FALSE);
	GetDlgItem(IDC_FORGET)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK2)->ShowWindow(FALSE);
	GetDlgItem(IDC_CANEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_REG2)->ShowWindow(TRUE);
	GetDlgItem(IDC_BACK)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_IDCARD)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_IDCARD)->ShowWindow(TRUE);
}

void CP2P_ENDDlg::OnReg2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	CString strPSW;
	CString strIDCard;
	
	this->GetDlgItem(IDC_EDIT_LOGIN)->GetWindowText(strName);
	this->GetDlgItem(IDC_EDIT_PSW)->GetWindowText(strPSW);
	this->GetDlgItem(IDC_EDIT_IDCARD)->GetWindowText(strIDCard);
	this->GetDlgItem(IDC_EDIT_SERVIP)->GetWindowText(m_UserData.strIP);
	this->GetDlgItem(IDC_EDIT_SERVPORT)->GetWindowText(m_UserData.strPort);
	if (strName.GetLength()<8)
	{
		MessageBox("用户名必须大于四个汉字或者8个字符！");
		return;
	}
	if (strPSW.GetLength()<5)
	{
		MessageBox("密码必须大于六位！");
		return;
	}
	if (strIDCard.GetLength()==0)
	{
		MessageBox("请输入身份证号码！");
		return;
	}
	if (strIDCard.GetLength()!=18)
	{
		MessageBox("请输入正确的身份证号码！");
		return;
	}
	if(m_UserData.strIP.GetLength()==0||m_UserData.strPort.GetLength()==0)
	{
		MessageBox("请输入IP和端口号");
		return;
	}
	strcpy(m_ClientMsg.m_UserName,strName.GetBuffer());
	strcpy(m_ClientMsg.m_PSW,strPSW.GetBuffer());
	strcpy(m_ClientMsg.m_IDCard,strIDCard.GetBuffer());
	strName.ReleaseBuffer();
	strPSW.ReleaseBuffer();
	strIDCard.ReleaseBuffer();
	m_ClientMsg.sign = REGMSG;
	m_ClientMsg.id = 1;
	m_ClientSock.SetDlg(this);
	if(!m_ClientSock.Create(0,SOCK_DGRAM))		//	0表示客户端随机端口
	{
		char szMsg[1024] = {0};
		sprintf(szMsg, "create faild: %d", m_ClientSock.GetLastError());
		AfxMessageBox(szMsg);
		return;
	}
	int nPort = atoi(m_UserData.strPort);
	m_ClientSock.SendTo((char*)&m_ClientMsg, sizeof(m_ClientMsg),nPort,m_UserData.strIP,0);
	this->GetDlgItem(IDC_REG2)->EnableWindow(FALSE);
	SetTimer(1,10000,NULL);   //设置定时器。判断登陆超时
	//按过一次不能再按了，因为客户端创建的连接还没关闭

}

void CP2P_ENDDlg::OnBack()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetDlgItem(IDC_FORGET2)->ShowWindow(FALSE);
	this->GetDlgItem(IDC_EDIT_PSW)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_EDIT_PSW)->SetWindowText("");
	this->GetDlgItem(IDC_EDIT_IDCARD)->SetWindowText("");
	m_Password.SetPasswordChar('*');
	if (m_ClientSock)
	{
		m_ClientSock.Close();
	}
	
	GetDlgItem(IDC_LOGIN)->ShowWindow(TRUE);
	GetDlgItem(IDC_REG1)->ShowWindow(TRUE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(TRUE);
	GetDlgItem(IDC_FORGET)->ShowWindow(TRUE);
	GetDlgItem(IDC_CHECK2)->ShowWindow(TRUE);
	GetDlgItem(IDC_CANEL)->ShowWindow(TRUE);
	GetDlgItem(IDC_REG2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_IDCARD)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_IDCARD)->ShowWindow(FALSE);
	GetDlgItem(IDC_BACK)->ShowWindow(FALSE);
}

void CP2P_ENDDlg::OnForget2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	CString strPSW;
	CString strIDCard;

	this->GetDlgItem(IDC_EDIT_LOGIN)->GetWindowText(strName);
	this->GetDlgItem(IDC_EDIT_PSW)->GetWindowText(strPSW);
	this->GetDlgItem(IDC_EDIT_IDCARD)->GetWindowText(strIDCard);
	this->GetDlgItem(IDC_EDIT_SERVIP)->GetWindowText(m_UserData.strIP);
	this->GetDlgItem(IDC_EDIT_SERVPORT)->GetWindowText(m_UserData.strPort);
	if (strName.GetLength()==0)
	{
		MessageBox("请输入用户名");
		return;
	}
	if (strIDCard.GetLength()==0)
	{
		MessageBox("请输入身份证号码");
		return;
	}
	if(m_UserData.strIP.GetLength()==0||m_UserData.strPort.GetLength()==0)
	{
		MessageBox("请输入IP和端口号");
		return;
	}
	strcpy(m_ClientMsg.m_UserName,strName.GetBuffer());
	strcpy(m_ClientMsg.m_PSW,strPSW.GetBuffer());
	strcpy(m_ClientMsg.m_IDCard,strIDCard.GetBuffer());
	strName.ReleaseBuffer();
	strPSW.ReleaseBuffer();
	strIDCard.ReleaseBuffer();
	m_ClientMsg.sign = FORGETMSG;
	m_ClientMsg.id = 1;
	m_ClientSock.SetDlg(this);
	if(!m_ClientSock.Create(0,SOCK_DGRAM))		//	0表示客户端随机端口
	{
		char szMsg[1024] = {0};
		sprintf(szMsg, "create faild: %d", m_ClientSock.GetLastError());
		AfxMessageBox(szMsg);
		return;
	}
	int nPort = atoi(m_UserData.strPort);
	m_ClientSock.SendTo((char*)&m_ClientMsg, sizeof(m_ClientMsg),nPort,m_UserData.strIP,0);
	this->GetDlgItem(IDC_FORGET2)->EnableWindow(FALSE);
	SetTimer(1,10000,NULL);   //设置定时器。判断登陆超时
	//按过一次不能再按了，因为客户端创建的连接还没关闭
}

void CP2P_ENDDlg::OnEnChangeEditIdcard()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
