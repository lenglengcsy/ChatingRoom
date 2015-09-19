#define MAXUSERS 100
#define LOGINMSG 1
#define MULCHATMSG 2
#define P2PCHATMSG 3
#define P2PHOLETO 4
#define ISALIVEMSG 5
#define PEERLOGINMSG 6
#define REGMSG 7
#define FORGETMSG 8
#define EXITMSG 0

#ifndef FUN_H				//条件编译救我一命		结构体后头不要自定义变量 否则会重定义
#define FUN_H

class CP2PChatDlg;

typedef struct UserData
{
	int id;
	char userName[20];
	bool isAlive;
	int servACK;
	CString strIP;
	CString strPort;
	CString ServChat;
	//sockaddr_in addr;
	//struct UserData *FNode;  //取消好友机制
	int idFlag ;   // 0表示普通用户，1表示临时服务器
	bool selFlag ;   //是否已经投票

};

typedef struct Message
{
	int sign;				//消息代码       1表示登陆  2表示发送消息
	int id;
	int userNum;
	int servACK;
	int peerACK;
	char chat[100];			//用户传递的消息
	char m_UserName[20];
	char m_PSW[20];
	char m_IDCard[20];
	sockaddr_in userAddr;
	char OLUserList[MAXUSERS][20];
};

typedef struct ServerData
{
	int userNum;				//OL
	bool state;
	CString servLog;
	sockaddr_in addrServ;		//服务器的地址信息
	SOCKET ClientSock;        //客户的连接请求
	SOCKET ServerSock;                 
	UserData OLUserList[MAXUSERS];	//在线用户列表
};

typedef struct PeerData
{
	char peerName[20];				
	bool state;
	BOOL isRead;  //是否已读
	CString peerMsgLog;			//	信息记录
	sockaddr_in peerAddr;		//Peer的地址信息
	CP2PChatDlg *peerChatDlg;
	
};

#endif