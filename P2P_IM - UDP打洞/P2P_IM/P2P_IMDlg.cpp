// P2P_IMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "P2P_IM.h"
#include "P2P_IMDlg.h"


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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CP2P_IMDlg 对话框




CP2P_IMDlg::CP2P_IMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CP2P_IMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CP2P_IMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERLIST, m_OLUserList);
	DDX_Control(pDX, IDC_STATIC_NUM, m_OLNum);
	DDX_Control(pDX, IDC_STATIC_STATE, m_State);
	DDX_Control(pDX, IDC_EDIT_LOG, m_ServerLog);
}

BEGIN_MESSAGE_MAP(CP2P_IMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_START, &CP2P_IMDlg::OnStart)
	ON_BN_CLICKED(ID_STOP, &CP2P_IMDlg::OnStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CP2P_IMDlg 消息处理程序

BOOL CP2P_IMDlg::OnInitDialog()
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
	m_OLUserList.SetItemHeight(0,18);
	m_ServData.state = false; //设置服务器状态
	m_ServData.userNum = 0;
	m_ServData.userID = 1000;
	m_ServData.servACK = 1000; //记录第几次轮询
	m_State.SetWindowText("服务器关闭");
	m_OLNum.SetWindowText("当前在线人数：0");
	for (int i = 0;i<MAXUSERS;i++)		//避免登陆时候列表显示出乱码
	{
		m_ServBaseMsg.OLUserList[i][0] = '\0';
	}
	
	refresh();			//更新界面信息
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CP2P_IMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CP2P_IMDlg::OnPaint()
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
HCURSOR CP2P_IMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CP2P_IMDlg::refresh()	//刷新界面
{
	CString strNum;
	/*POSITION pos=m_SockUserList.GetHeadPosition();
	//UserData *p_Data;
	m_OLUserList.ResetContent();
	//POSITION posBuffer = NULL;
	for(int i=0 ; pos!=NULL ; i++ )
	{
		UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
		if (p_User->id>1000)
		{
			strcpy(m_ServBaseMsg.OLUserList[i],p_User->userName);
			m_OLUserList.AddString(p_User->userName);//更新在线用户列表
		}
		
	}*/
	UpdateUserList(&m_ServBaseMsg);
	strNum.Format("当前在线人数:%4d",m_ServData.userNum);
	m_OLNum.SetWindowText(strNum);
	m_ServerLog.SetWindowText(m_ServData.servLog);
	if (m_ServData.state)
	{
		m_State.SetWindowText("服务器开启");
	} 
	else
	{
		m_State.SetWindowText("服务器关闭");
	}
	m_ServerLog.LineScroll(m_ServerLog.GetLineCount());

}
void CP2P_IMDlg::OnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ServData.state)
	{
		MessageBox(_T("服务器已经打开!"));
	}
		//设置监听端口，把设置类型为UDP
		m_ServSock.Create(1000,SOCK_DGRAM); //这一步好像跳过了套接字的好多步骤
		m_ServSock.SetDlg(this);
		m_ServData.state = true;
		m_ServData.servLog +="服务器成功开启\r\n";	//回车加换行
		refresh();							//更新界面信息
		MessageBox(_T("服务器开启成功"));
		GetDlgItem(ID_START)->EnableWindow(FALSE);
		GetDlgItem(ID_STOP)->EnableWindow(TRUE);
		SetTimer(1,30000,NULL);				//通过定时器检查客户端是否掉线
		SetTimer(2,10000,NULL);				//清理过期客户端
	
}

void CP2P_IMDlg::AcceptConnect()
{
	/*CServSock* pClientSock=new CServSock();	//这个变量用来保存客户端的连接信息
	pClientSock->SetDlg(this);				//与对话框相关联
	if(m_ServSock.Accept(*pClientSock))   //CSOCKET类 处理消息
	{
		//MessageBox("成功接受");
	}
	else
	{
		pClientSock->Close();
		delete pClientSock;
	}*/
}

void CP2P_IMDlg::ReceiveData(CServSock *sock)
{
	Msg buffer;
	CString str;
	sockaddr_in addrBuffer;
	UserData *userBuffer;	//用来保存地址等数据
	userBuffer =new UserData();	//	这哥们要被塞入容器里头
	int len = sizeof(SOCKADDR_IN);
	POSITION pos = m_SockUserList.GetHeadPosition();
	sock->ReceiveFrom((char*)&buffer,sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),&len,0);	//设置缓冲
	if (abs(buffer.id)>m_ServData.userID)  //服务器的ID不断的增加 相当于全局时间
	{
		buffer.sign = EXITMSG;
		buffer.servACK = 0;
		sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
	}
	UpdateUserList(&buffer); //更新BUFFER中的在线列表
	switch (buffer.sign)
	{
	case LOGINMSG: 	//第一次登陆消息
		if (!ADOCheckUser(&buffer,buffer.sign))
		{
			buffer.servACK = -1; //用户名或密码错误
			sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
			return;
		}
		strcpy(userBuffer->userName,buffer.m_UserName);	//把用户名保存到套接字对象里
		userBuffer->userState = true ;
		m_ServData.userID++;
		userBuffer->id = m_ServData.userID;
		userBuffer->servACK = m_ServData.servACK+2;
		m_SockUserList.AddTail(userBuffer);				//将用户信息添加到容器里	
		m_ServData.userNum++;
		m_ServBaseMsg.sign=LOGINMSG;
		m_ServBaseMsg.userNum = m_ServData.userNum;
		m_ServBaseMsg.servACK=m_ServData.servACK;	//	服务器确认登陆
		m_ServBaseMsg.id = m_ServData.userID;
		//sock->m_id++;		//给予ID  用于选举算法
		UpdateUserList(&m_ServBaseMsg);					//	之前登入时候送出的buffer和更新的buffer不一致 导致登入时候获得的列表不准确
		sock->SendTo((char*)&m_ServBaseMsg, sizeof(m_ServBaseMsg),(SOCKADDR*)&(userBuffer->userAddr),len,0);
		str.Format("%s上线\r\n",userBuffer->userName);//显示上线信息

		//////上线通知/////
		UpdateUserList(&buffer);
		strcpy(buffer.m_UserName,"服务器通知：");
		buffer.sign = PEERLOGINMSG;
		
		while(pos!=NULL)
		{
			UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
			if(p_User->userState)
			{
				sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(p_User->userAddr),sizeof(SOCKADDR),0);
			}
		}
		///////
		m_ServData.servLog+=str;
		m_ServerLog.SetWindowText(str);
		m_ServerLog.LineScroll(m_ServerLog.GetLineCount());	

		break;

	case MULCHATMSG: //群发消息
		
		while(pos!=NULL)
		{
			UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
			if(p_User->userState)
			{
				sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(p_User->userAddr),sizeof(SOCKADDR),0);
			}
		}
		break;
	case EXITMSG:
		Stop(buffer.id);  //踢客户端下线
		break;
	case ISALIVEMSG:  //心跳信息
		pos = m_SockUserList.GetHeadPosition();
		while(pos!=NULL)
		{
			UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
			if(p_User->id == buffer.id)  //之前这里少了个等号  导致ID全部相等
			{
				p_User->isAlive = true ;
				p_User->servACK = buffer.servACK+1;   //所以servACK初始值设多大都没有用
			}
		}
		break;
	case P2PHOLETO:
		addrBuffer = userBuffer->userAddr;		//请求P2P方的地址缓存
		userBuffer = FindClientByName(buffer.m_UserName);	//找到需要发送的客户端
		if(!userBuffer)
		{
			return;
		}
		buffer.userAddr = userBuffer->userAddr;  //把找到的用户信息发送回去
		sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(addrBuffer),len,0);
		break;
	case REGMSG:
		if (!ADOCheckUser(&buffer,buffer.sign))
		{
			buffer.servACK = -1; //用户名或密码错误
			sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
			return;
		}
		else
		{
			CString sql;
			sql.Format("INSERT INTO Users ([UserName],[PassWord],[IDNumber]) VALUES(\"%s\",\"%s\",\"%s\")",buffer.m_UserName,buffer.m_PSW,buffer.m_IDCard);
			if(0 != AdoAccess.Open("test.mdb","","",""))
			{
				MessageBox((LPCTSTR)AdoAccess.GetLastErrCode().c_str());
				
			}
			if (!AdoAccess.Exectue((string)sql)==0)
			{
				//MessageBox((LPCTSTR)AdoAccess.GetLastErrCode().c_str());
				//::AfxMessageBox(_T("数据添加失败!"));
			}
			buffer.servACK = 0;
			sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
			return;
		}
		break;
	case FORGETMSG:
		if (!ADOCheckUser(&buffer,buffer.sign))
		{
			buffer.servACK = -1; //用户名或密码错误
			sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
			return;
		}
		else
		{
			buffer.servACK = 0;
			sock->SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(userBuffer->userAddr),len,0);
			return;
		}
		break;
	default:;
	};
	
//	send(*sock,data,sizeof(data),0);//传输好友列表//
	refresh();
}

void CP2P_IMDlg::Stop(int id)		//	踢下客户端
{
	CString st;
	POSITION pos = m_SockUserList.GetHeadPosition();
	POSITION posBuffer = NULL;
	Msg buffer;
	buffer.sign = EXITMSG;
	buffer.servACK = 0;
	while (pos!=NULL)
	{
		posBuffer = pos;
		UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
		if(p_User->id==id)
		{
			st.Format("%s下线\r\n",p_User->userName);	
			m_ServData.servLog+=st;
			m_ServData.userNum--;
			m_ServerLog.SetWindowText(m_ServData.servLog);
			m_ServSock.SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(p_User->userAddr),sizeof(SOCKADDR),0);
			m_SockUserList.RemoveAt(posBuffer);
		}
	}
	m_ServerLog.LineScroll(m_ServerLog.GetLineCount());
	//delete sock;
	this->refresh();
}

void CP2P_IMDlg::OnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MessageBox("确定要关闭服务吗?","系统",MB_YESNO)==IDYES)
	{
		
		POSITION pos = m_SockUserList.GetHeadPosition();
		while(pos!=NULL)
		{
			UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
			Stop(p_User->id);
		}
		//m_SockUserList.RemoveAll();
		m_ServData.state=FALSE;
		this->refresh();
		m_ServData.servLog += "服务器成功关闭\r\n";
		m_ServerLog.SetWindowText(m_ServData.servLog);
		m_ServSock.Close();
		MessageBox("服务关闭服务");
		GetDlgItem(ID_START)->EnableWindow(TRUE);
		GetDlgItem(ID_STOP)->EnableWindow(FALSE);
	}
}

void CP2P_IMDlg::UpdateUserList(Msg *buffer)
{
	POSITION pos=m_SockUserList.GetHeadPosition();
	//UserData *p_Data;
	m_OLUserList.ResetContent();
	//POSITION posBuffer = NULL;
	buffer->userNum = m_ServData.userNum;
	for(int i=0 ; pos!=NULL ; i++ )
	{
		UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
		if (p_User->id>1000)
		{
			strcpy(buffer->OLUserList[i],p_User->userName);
			m_OLUserList.AddString(p_User->userName);//更新在线用户列表
		}

	}
}

UserData* CP2P_IMDlg::FindClientByName(char *userName)
{
	POSITION pos = m_SockUserList.GetHeadPosition();
	while(pos!=NULL)
	{
		UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
		if(!strcmp(p_User->userName,userName))
		{
			return p_User;
		}
	}
	return NULL;
}
BOOL CP2P_IMDlg::ADOCheckUser(Message *p_Userbuffer,int sign)
{
	vector<ST_TABLE> list;
	int fieldsize=0;
	CString userName ;
	CString password ;
	CString IDNumber ;
	CString bff ;
	//CString password = p_Userbuffer->
	userName.Format("UserName = '%s'",p_Userbuffer->m_UserName);
	password = p_Userbuffer->m_PSW;
	IDNumber = p_Userbuffer->m_IDCard;
	CString sql=_T("select * from Users where "+userName);
	if(0 != AdoAccess.Open("test.mdb","","",""))
	{
		MessageBox((LPCTSTR)AdoAccess.GetLastErrCode().c_str());
		return FALSE;
	}

	if(0!=AdoAccess.Exectue((string)sql, list))
	{
		MessageBox((LPCTSTR)AdoAccess.GetLastErrCode().c_str());
	}
	fieldsize = list[0].value.size();
	if (sign==REGMSG)		//	注册信息验证
	{
		if(fieldsize==0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	if(fieldsize<1)		//登录信息验证
	{
		return FALSE;
		
	}
	else
	{
		if (sign==LOGINMSG)
		{
			bff = list[1].value[0].c_str();
			if (bff==password)
			{
			//	password.ReleaseBuffer();
				return TRUE;
			}
			//password.ReleaseBuffer();
		}
		if (sign==FORGETMSG)
		{
			password = list[1].value[0].c_str();//获取密码
			bff = list[2].value[0].c_str();	//获取身份信息
			if (bff==IDNumber)
			{
				//	password.ReleaseBuffer();
				strcpy(p_Userbuffer->m_PSW,password.GetBuffer());
				password.ReleaseBuffer();
				return TRUE;
			}
			//password.ReleaseBuffer();
		}
		
	}
	return FALSE;
}

void CP2P_IMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POSITION pos = m_SockUserList.GetHeadPosition();
	POSITION posBuffer = NULL;
	Msg buffer;
	buffer.sign = ISALIVEMSG;
	buffer.servACK = m_ServData.servACK;	//	ACK赋值并自增

	switch(nIDEvent)
	{
	case 1:
			while(pos!=NULL)		//发送轮询信息
			{
				UserData* p_User = (UserData*)m_SockUserList.GetNext(pos);
				//if(p_User->userState)
				//{	
					p_User->servACK = m_ServData.servACK;
					UpdateUserList(&buffer);
					m_ServSock.SendTo((char*)&buffer, sizeof(buffer),(SOCKADDR*)&(p_User->userAddr),sizeof(SOCKADDR),0);
				//}
			}
			m_ServData.servACK++;
		break;
	case 2:			
			pos=m_SockUserList.GetHeadPosition();
			while(pos!=NULL)		//清理掉线的客户端
			{	
				posBuffer = pos;
				UserData* p_User=(UserData*)m_SockUserList.GetNext(pos);
				if (p_User->servACK < m_ServData.servACK)
				{
					Stop(p_User->id);
				}
			}
		break;

	}
	
	refresh();
	CDialog::OnTimer(nIDEvent);
}
