
// OpenQQSerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenQQSer.h"
#include "OpenQQSerDlg.h"
#include "afxdialogex.h"

#include "UserInfo.h"

#include "SendMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenQQSerDlg 对话框




COpenQQSerDlg::COpenQQSerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenQQSerDlg::IDD, pParent)
	, m_clientCnt(0)
	, m_firstRecvFile(TRUE)
	, m_recvFilePath(_T(""))
	, m_registerUserCnt(0)
	, m_tHeadNum(0)
{
	for (int i=0; i<255; i++)
	{
		m_user[i].bOnLine = FALSE;
		m_user[i].accFlag = FALSE;
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenQQSerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_userList);
}

BEGIN_MESSAGE_MAP(COpenQQSerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNBG, &COpenQQSerDlg::OnBnClickedBtnbg)
	ON_MESSAGE(WM_SOCK, OnSock)
	ON_BN_CLICKED(IDC_BTNED, &COpenQQSerDlg::OnBnClickedBtned)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &COpenQQSerDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_MIN, &COpenQQSerDlg::OnBnClickedBtnMin)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// COpenQQSerDlg 消息处理程序

BOOL COpenQQSerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	//初始化列表框
	InitListCtr();
	//载入记录用户数目
	LoadClientCnt();

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenQQSerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenQQSerDlg::OnPaint()
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
		//CDialogEx::OnPaint();

		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BTBK);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenQQSerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenQQSerDlg::OnBnClickedBtnbg()
{
	// TODO: 在此添加控件通知处理程序代码
	//开始服务之后
	
	InitSocket();
	//MessageBox(_T("服务已开启"));
	((CButton *)GetDlgItem(IDC_BTNBG))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_BTNED))->EnableWindow(TRUE);
}


BOOL COpenQQSerDlg::InitSocket(void)
{
	m_sockSrv = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6600);
	
	bind(m_sockSrv, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR));

	WSAAsyncSelect(m_sockSrv, m_hWnd, WM_SOCK, FD_READ | FD_WRITE | FD_ACCEPT | FD_CLOSE);

	listen(m_sockSrv, 5);

	return TRUE;
}

LRESULT COpenQQSerDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;

	char chRecvMsg[512];
	memset(chRecvMsg, 0, 512);
	CString logInfo;
	CString logTempInfo;
	//char tempName[30];
	CString fromatMsg;
	char sendMsg[256];
	BOOL isOnce = TRUE;
	CTime currTime = CTime::GetCurrentTime();
	logTempInfo = currTime.Format("%H:%M:%S ");
	int i, j, k;
	switch (LOWORD(lParam))
	{
	case FD_READ:
		//来判断是否第一次链接
		//m_clientCnt 为所有的客户（包括在线和离线的用户）
		for (int i=0; i<m_clientCnt; i++)
		{
			if (m_user[i].userSocket == s)
			{
				//存在这个socket说明不是第一次  
				//要么是聊天  要么是将其的状态更新为在线

				isOnce = FALSE;
				break;
			}
		}

		//如果是第一次，那么就更新其信息到对应的listctrl
		if (isOnce) 
		{
			//第一次连接的时候收取用户发来的信息
			if (/*m_user[m_clientCnt].bOnLine == FALSE && */m_user[m_clientCnt].accFlag == TRUE)
			{
				recv(s, (char *)&m_user[m_clientCnt], sizeof(m_user[m_clientCnt]), 0);

				//判断是不是注册信息
				if (m_user[m_clientCnt].isRegister == TRUE)
				{
					//写入到配置文件
					if (WriteRegInfoToProfile(m_user[m_clientCnt].userName, 
						m_user[m_clientCnt].passWord, m_user[m_clientCnt].headNum) == TRUE)
					{
						send(s, "Register success!", strlen("Register success!")+1, 0);
					}
					else
					{
						send(s, "Register failure!", strlen("Register failure!")+1, 0);
					}
					closesocket(s);
					return 0;
				}


				//验证用户名和密码
				if (VerifyUser(m_user[m_clientCnt].userName, 
					m_user[m_clientCnt].passWord) == FALSE)
				{
					send(s, "failure", strlen("failure")+1, 0);
					closesocket(s);
					return 0;
				}
				else if (VerifyRepeatLogin(m_user[m_clientCnt].userName) == TRUE)
				{
					send(s, "repeat login", strlen("repeat login")+1, 0);
					closesocket(s);
					return 0;
				}
				else 
				{
					send(s, "success", strlen("success")+1, 0);
					//send(s, (char *)&m_tHeadNum, sizeof(m_tHeadNum), 0);

					SendMsg setPicMsg;
					setPicMsg.isHeadPic = TRUE;
					setPicMsg.headNum = m_tHeadNum;
					m_user[m_clientCnt].headNum = m_tHeadNum;
					send(s, (char *)&setPicMsg, sizeof(setPicMsg), 0);

				}

				//接收了姓名之后来判断是不是老客户，是老客户直接替换套接字
				int x;
				for (x=0; x<m_clientCnt; x++)
				{
					if (!_tcscmp(m_user[x].userName, m_user[m_clientCnt].userName))
					{
						//老客户
						if (m_user[x].bOnLine == TRUE)
						{
							//MessageBox(_T("已经登陆!"));
							//向该登陆界面发送消息。

							//send(s, "the same", strlen("the same")+1, 0); 
							return 0;
						}
						else
						{
							//send(s, "no the same", strlen("no the same")+1, 0); 
							m_user[x].userSocket = s;
						}
						break;
					}
				}

				//新客户
				if (x == m_clientCnt)
				{
					m_user[m_clientCnt].userSocket = s;
					m_user[m_clientCnt].bOnLine = TRUE;
					//m_user[m_clientCnt].headNum = m_tHeadNum;
				
					logInfo.Format(_T("%s 上线了！"), m_user[m_clientCnt].userName);
					logTempInfo += logInfo;

					m_userList.InsertItem(m_userList.GetItemCount(), m_user[m_clientCnt].userName);//主项
					m_userList.SetItemText(m_userList.GetItemCount()-1, 1, _T("在线"));//子项


					////将该客户的图片ID赋值
					/*VerifyUser(m_user[m_clientCnt].userName, m_user[m_clientCnt].passWord);
					m_recvMsg.headNum = this->m_tHeadNum;*/

					//暂时先加一个循环来将所发送的信息都设置为好友更新的信息
					m_recvMsg.isFriendInfo = TRUE;
					m_recvMsg.isOnLine = TRUE;
					m_recvMsg.isHeadPic = FALSE;
					m_recvMsg.headNum = m_user[m_clientCnt].headNum;
					
					_tcscpy(m_recvMsg.userName,m_user[m_clientCnt].userName);

					//向以前上线的老客户发送刚上线的新客户的信息
					for (int i=0; i<m_clientCnt; i++)
					{
						if (m_user[i].bOnLine == TRUE)
						{
							send(m_user[i].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
						}
					}

					//向上线的新客户发送老客户的信息
					for (int i=0; i<m_clientCnt; i++)
					{
						if (m_user[i].bOnLine == TRUE)
						{
							m_recvMsg.headNum = m_user[i].headNum;
							_tcscpy(m_recvMsg.userName,m_user[i].userName);
							send(m_user[m_clientCnt].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
						}
					}

					m_clientCnt++;
					((CListBox *)GetDlgItem(IDC_LIST_LOG))->InsertString(-1, logTempInfo);
				}
				else
				{
					//老客户
					//将该老客户 设置为在线
					m_user[x].bOnLine = TRUE;

					//将老客户的标志设置为在线
					for (int n=0; n<m_userList.GetItemCount(); n++)
					{
						if (!_tcscmp(m_user[x].userName, m_userList.GetItemText(n,0)))
						{
							m_userList.SetItemText(n, 1, _T("在线"));

							logInfo.Format(_T("%s 上线了！"), m_user[m_clientCnt].userName);
							logTempInfo += logInfo;
							((CListBox *)GetDlgItem(IDC_LIST_LOG))->InsertString(-1, logTempInfo);
							break;
						}
					}

					m_recvMsg.isFriendInfo = TRUE;
					m_recvMsg.isOnLine = TRUE;
					m_recvMsg.isHeadPic = FALSE;
					_tcscpy(m_recvMsg.userName,m_user[m_clientCnt].userName);
					m_recvMsg.headNum = m_user[m_clientCnt].headNum;
					for (int i=0; i<m_clientCnt; i++)
					{
						if (!_tcscmp(m_user[i].userName, m_recvMsg.userName))
						{
							m_recvMsg.headNum = m_user[i].headNum;
							break;
						}
					}


					//向以前上线的老客户发送刚上线的新客户的信息
					for (int i=0; i<m_clientCnt; i++)
					{
						if (m_user[i].bOnLine == TRUE)
							send(m_user[i].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
					}

					//向上线的新客户发送老客户的信息
					for (int i=0; i<m_clientCnt; i++)
					{
						if (m_user[i].bOnLine == TRUE)
						{
							m_recvMsg.headNum = m_user[i].headNum;
							_tcscpy(m_recvMsg.userName,m_user[i].userName);
							send(m_user[x].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
						}
					}
				}
			}
		}
		else
		{
			//可能是老客户再次上线，也有可能是聊天

			//聊天  将所有用户的标志位设置为FALSE
			//判断是群聊还是私聊			
			int rtval =recv(s, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
			//CString recvSize;
			if (SOCKET_ERROR == rtval)
			{
				MessageBox(_T("recv 失败"));
			}
			/*else
			{
				recvSize.Format(_T("recv %d"), rtval);
				MessageBox(recvSize);
			}*/

			m_recvMsg.isFriendInfo = FALSE;

			//头像更新
			if (m_recvMsg.isHeadPic == TRUE)
			{
				UpdateHeadPic(m_recvMsg.userName, m_recvMsg.headNum);
				return 0;
			}


			//取出消息并转化为char*字符串
			WideCharToMultiByte(CP_OEMCP, 0, m_recvMsg.sendMsg, -1, sendMsg, 256, 0, FALSE);

			//私聊
			fromatMsg = m_recvMsg.sendUser;
			fromatMsg += _T("   ");
			fromatMsg += currTime.Format("%H:%M:%S ");
			fromatMsg += m_recvMsg.sendMsg;

			////判断是不是窗口抖动 或者文件传输
			//if ()
			if (m_recvMsg.isWindowShake == TRUE 
				|| m_recvMsg.isFile == TRUE)
			{
				fromatMsg = m_recvMsg.sendMsg;
			}

			_tcscpy(m_recvMsg.sendMsg, fromatMsg.GetBuffer());

			//
			SendMsg tempSendMsg;
			_tcscpy(tempSendMsg.sendUser,m_recvMsg.recvUser);
			_tcscpy(tempSendMsg.recvUser,m_recvMsg.sendUser);
			_tcscpy(tempSendMsg.sendMsg, m_recvMsg.sendMsg);
			tempSendMsg.isFriendInfo = FALSE;
			tempSendMsg.isSingleSend = TRUE;
			tempSendMsg.isFile = m_recvMsg.isFile;
			tempSendMsg.isWindowShake = m_recvMsg.isWindowShake;

			if (m_recvMsg.isSingleSend == TRUE)
			{
				//判断是不是文件传输
				if (m_recvMsg.isFile == FALSE)
				{
					send(s, (char *)&tempSendMsg, sizeof(tempSendMsg), 0);
				}

				for (int i=0; i<m_clientCnt; i++)
				{
					if (!_tcscmp(m_recvMsg.recvUser, m_user[i].userName))
					{	
						//给对方发送	
						int rtval = send(m_user[i].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
						//CString sendSize;
						if (SOCKET_ERROR == rtval)
						{
							MessageBox(_T("send 失败"));
						}
						break;
					}
				}
			}
			else //群聊
			{
				for (int i=0; i<m_clientCnt; i++)
				{
					send(m_user[i].userSocket, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);
				}
			}
		}
		break;

	case FD_WRITE:

		break;

	case FD_ACCEPT://有客户端上线
		accept(s, NULL, NULL);
		m_user[m_clientCnt].accFlag = TRUE;
		//在这里判断是否是老客户

		break;

	case FD_CLOSE://有客户端下线
		for (i=0; i<m_clientCnt; i++)
		{
			if (m_user[i].userSocket == s)
			{
				m_user[i].bOnLine = FALSE;
				logInfo.Format(_T("%s 下线了！"), m_user[i].userName);
				logTempInfo += logInfo;
				((CListBox *)GetDlgItem(IDC_LIST_LOG))->InsertString(-1, logTempInfo);

				//m_userList.DeleteItem(i);
				for (int m=0; m<m_userList.GetItemCount(); m++)
				{
					if (!_tcscmp(m_user[i].userName, m_userList.GetItemText(m, 0)))
					{
						//m_userList.DeleteItem(m);
						m_userList.SetItemText(m, 1, _T("离线"));
						break;
					}
				}
				
				//向所有客户端发送下线信息
				SendMsg outOfLineMsg;
				outOfLineMsg.isFriendInfo = TRUE;
				_tcscpy(outOfLineMsg.userName, m_user[i].userName);
				outOfLineMsg.isOnLine = FALSE;

				for (k=0; k<m_clientCnt; k++)
				{
					if (k!=i)
						send(m_user[k].userSocket, (char *)&outOfLineMsg, sizeof(outOfLineMsg), 0);
				}
				break;
			}
		}
	}
	return 0;
}

void COpenQQSerDlg::OnBnClickedBtned()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0; i<m_clientCnt; i++)
		closesocket(m_sockCli[i]);
	closesocket(m_sockSrv);

	((CButton *)GetDlgItem(IDC_BTNBG))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_BTNED))->EnableWindow(FALSE);
	//MessageBox(_T("服务已停止"));
}

void COpenQQSerDlg::InitListCtr(void)
{
	DWORD dwStyle = m_userList.GetExtendedStyle(); //显示网格线
	dwStyle |= LVS_EX_FULLROWSELECT;            
    dwStyle |= LVS_EX_GRIDLINES;     
	m_userList.SetExtendedStyle(dwStyle);

	m_userList.InsertColumn(0, _T("用户名"), LVCFMT_CENTER, 80);
	m_userList.InsertColumn(1, _T("在线状态"), LVCFMT_CENTER, 80);
}


void COpenQQSerDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}


void COpenQQSerDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}


void COpenQQSerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL COpenQQSerDlg::VerifyUser(const CString& vUserName, const CString& vUserPasswd)
{
	//载入配置文件
	//加载保存密码的配置文件
	CString lpPath = _T(".\\userNameAndPasswd.ini");
	CFileFind finder;
	BOOL ifFind = finder.FindFile(lpPath);

	if (ifFind)
	{
		//自动从ini文件载入保存的用户名和密码
		TCHAR userName[20];
		TCHAR passWord[20];
		for (int i=0; i<m_registerUserCnt; i++)
		{
			CString szCnt;
			szCnt.Format(_T("%d"), i);
			::GetPrivateProfileString(szCnt, _T("UserName"), NULL,  userName, 
					20, lpPath);

			if (!_tcscmp(vUserName, userName))
			{
				TCHAR tcHeadNum[3];
				char chHeadNum[3];
				::GetPrivateProfileString(userName, _T("Password"), NULL,  passWord, 
						20, lpPath);
				::GetPrivateProfileString(userName, _T("HeadNum"), NULL,  tcHeadNum, 
						3, lpPath);

				WideCharToMultiByte(CP_OEMCP, 0, tcHeadNum, -1, chHeadNum, 3, 0, FALSE);

				m_tHeadNum = (UINT)(chHeadNum[0] - 48);

				if (!_tcscmp(vUserPasswd, passWord))
				{
					//用户名，密码验证通过
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


BOOL COpenQQSerDlg::WriteRegInfoToProfile(const CString &wUserName, const CString &wPasswd, const UINT & headNum)
{

	CString lpPath = _T(".\\userNameAndPasswd.ini");

	CString szClientCnt;
	szClientCnt.Format(_T("%d"), m_registerUserCnt);

	//先循环遍历整个userNameAndPasswd文件，看该用户名存在不存在
	for (int i=0; i<m_registerUserCnt; i++)
	{
		TCHAR userName[20];
		CString szCnt;
		szCnt.Format(_T("%d"), i);
		::GetPrivateProfileString(szCnt, _T("UserName"), NULL,  userName, 
			20, lpPath);
		//该用户名存在，写入失败
		if (!_tcscmp(userName, wUserName))
		{
			return FALSE;
		}
	}

	CString szHeadNum;
	szHeadNum.Format(_T("%d"), headNum);

	::WritePrivateProfileString(szClientCnt, _T("UserName"), wUserName, lpPath);
	::WritePrivateProfileString(wUserName, _T("Password"), wPasswd, lpPath);
	::WritePrivateProfileString(wUserName, _T("HeadNum"), szHeadNum, lpPath);//写入头像
	m_registerUserCnt++;

	//将这个计数也写入一个配置文件
	lpPath = _T(".\\UserCnt.ini");
	szClientCnt.Format(_T("%d"), m_registerUserCnt);
	::WritePrivateProfileString(_T("注册用户的个数"), _T("RegisterUserCnt"), 
		szClientCnt, lpPath);

	return TRUE;
}


void COpenQQSerDlg::LoadClientCnt(void)
{
	//加载一共有多少个用户
	CString lpPath = _T(".\\UserCnt.ini");
	TCHAR szClientCnt[3];
	char chClientCnt[3];

	::GetPrivateProfileString(_T("注册用户的个数"), _T("RegisterUserCnt"),
				NULL, szClientCnt, 3, lpPath);
	WideCharToMultiByte(CP_OEMCP, 0, szClientCnt, -1, chClientCnt, 3, 0, FALSE);
	m_registerUserCnt = atoi(chClientCnt);
}


BOOL COpenQQSerDlg::VerifyRepeatLogin(const CString & loginUserName)
{
	for (int i=0; i<this->m_clientCnt; i++)
	{
		if (!_tcscmp(m_user[i].userName, loginUserName))
		{
			if (m_user[i].bOnLine == TRUE)
				return TRUE;
		}
	}
	return FALSE;
}


void COpenQQSerDlg::UpdateHeadPic(const CString & userName, const UINT & headNum)
{
	CString lpPath = _T(".\\userNameAndPasswd.ini");

	CString szClientCnt;
	szClientCnt.Format(_T("%d"), m_registerUserCnt);

	//先循环遍历整个userNameAndPasswd文件，看该用户名存在不存在
	for (int i=0; i<m_registerUserCnt; i++)
	{
		TCHAR chUserName[20];
		CString szCnt;
		szCnt.Format(_T("%d"), i);
		::GetPrivateProfileString(szCnt, _T("UserName"), NULL,  chUserName, 
			20, lpPath);

		if (!_tcscmp(userName, chUserName))
		{
			CString szHeadNum;
			szHeadNum.Format(_T("%d"), headNum);
			::WritePrivateProfileString(chUserName, _T("HeadNum"), szHeadNum, lpPath);//写入头像
			break;
		}
	}

	//将该用户的头像id也修改掉
	for (int i=0; i<m_clientCnt; i++)
	{
		if (!_tcscmp(userName, m_user[i].userName))
		{
			m_user[i].headNum = headNum;
			break;
		}
	}
}


HBRUSH COpenQQSerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(CTLCOLOR_STATIC == nCtlColor)  
	{ 
		pDC->SetTextColor(RGB(255, 255, 255));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
