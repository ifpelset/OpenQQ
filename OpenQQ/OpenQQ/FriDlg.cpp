// FriDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenQQ.h"
#include "FriDlg.h"
#include "afxdialogex.h"

#include "OpenQQDlg.h"

#include "UserInfo.h"

#include "ChatDlg.h"

#include "SubWindowPar.h"
#include "CgHeadDlg.h"

// CFriDlg 对话框

IMPLEMENT_DYNAMIC(CFriDlg, CDialogEx)

CFriDlg::CFriDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriDlg::IDD, pParent)
	, m_clientCnt(1)
	, m_wndCnt(0)
	, m_firstRecvFile(TRUE)
	, m_recvFilePath(_T(""))
{
	for (int i=0; i<MAX_WINDOW_COUNT; i++)
		m_chatDlg[i] = NULL;
}

CFriDlg::~CFriDlg()
{
	//清空list中的所有项
	/*for (int i=0; i<m_friendList.GetItemCount(); i++)
		m_friendList.DeleteItem(i);*/

	Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
}

void CFriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_friendList);
}


BEGIN_MESSAGE_MAP(CFriDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FRIEND, &CFriDlg::OnNMClickListFriend)
	ON_MESSAGE(WM_SOCK, OnSock)
	ON_MESSAGE(WM_SUBTOMAIN, OnSubToMain)
	ON_MESSAGE(WM_TRANSICON, OnTransIcon)
	ON_BN_CLICKED(IDC_BTN_MIN, &CFriDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CFriDlg::OnBnClickedBtnClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_HEAD, &CFriDlg::OnBnClickedBtnHead)
	ON_MESSAGE(WM_NC,OnNotifyIcon)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CFriDlg 消息处理程序


void CFriDlg::InitListCtr(void)
{
	DWORD dwStyle = m_friendList.GetExtendedStyle(); //显示网格线
	dwStyle |= LVS_EX_FULLROWSELECT;            
    dwStyle |= LVS_EX_GRIDLINES;     
	m_friendList.SetExtendedStyle(dwStyle);

	

	//加入头像
	m_imageList.Create(40, 40, ILC_MASK | ILC_COLOR32, 1, 1);

	m_imageList.Add( AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD1));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD2));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD3));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD4));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD5));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_HEAD6));

	//m_friendList.SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);
	m_friendList.SetImageList(&m_imageList, LVSIL_SMALL);

	m_friendList.InsertColumn(0, _T("在线用户"), LVCFMT_CENTER, 120);

	//增加一项，群的显示
	m_friendList.InsertItem(0, _T("QQ群"), 0); //第三个参数为图标的索引
}


BOOL CFriDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//设置标题栏为自己的用户名，以标识哪个是自己，自己是哪个对话框
	CString userNameTitle;
	((COpenQQDlg *)AfxGetMainWnd())->GetDlgItemText(IDC_USERNAME, userNameTitle);
	SetWindowText(userNameTitle);

	InitSocket();

	InitListCtr();

	HBITMAP hBitmap;
	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BTBK2));
	((CListCtrl *)GetDlgItem(IDC_LIST_FRIEND))->SetBkImage(hBitmap);

	/*hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BTHEAD_S));
	((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetBitmap(hBitmap);*/
	//((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetIcon(this->m_tHIcon);

	CString windowText;
	GetWindowText(windowText);
	windowText += _T(" 在线");
	GetDlgItem(IDC_ONLINEMSG)->SetWindowText(windowText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//单击该好友，然后就与该好友聊天，打开聊天对话框
void CFriDlg::OnNMClickListFriend(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		//获取该用户名
		CString chatUserName = m_friendList.GetItemText(pNMListView->iItem,0);

		//先检测窗口是否创建
		if (m_chatDlg[pNMListView->iItem] == NULL)
		{
			CString windowText;
			GetWindowText(windowText);
			CreateChatWindow(windowText, chatUserName, pNMListView->iItem);
		}
	}

	*pResult = 0;
}

BOOL CFriDlg::InitSocket(void)
{
	m_sockCli = ((COpenQQDlg *)AfxGetMainWnd())->GetSockCli();
	WSAAsyncSelect(m_sockCli,m_hWnd,WM_SOCK,FD_READ);
	return 0;
}

//响应函数，接受服务器发过来的好友信息
LRESULT CFriDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	CString szRecvMsg;
	char chRecvMsg[256];
	memset(chRecvMsg, 0, 256);

	switch (LOWORD(lParam))
	{
	case FD_READ:
		//recv(s, (char *)&m_user[m_clientCnt], sizeof(m_user[m_clientCnt]), 0);

		int rtval = recv(s, (char *)&m_recvMsg, sizeof(m_recvMsg), 0);

		//CString recvSize;
		if (SOCKET_ERROR == rtval)
		{
			MessageBox(_T("recv 失败"));
		}

		//是设置头像的消息
		if (m_recvMsg.isHeadPic == TRUE)
		{
			//;
			HICON hIcon;
			hIcon = ((COpenQQDlg *)GetParent())->GetHICONByHeadNum(m_recvMsg.headNum);
			((CButton *)GetDlgItem(IDC_BTN_HEAD))->SetIcon(hIcon);
			return 0;
		}


		//是好友更新的信息
		if (m_recvMsg.isFriendInfo == TRUE)
		{
			if (m_recvMsg.isOnLine == TRUE)
			{
				//要判断下是不是自己。。。
				CString myself;
				GetWindowText(myself);
				int i;
				for (i=0; i<m_friendList.GetItemCount();)
				{
					if (_tcscmp(m_friendList.GetItemText(i, 0), m_recvMsg.userName))
					{
						i++;
					}
					else
					{
						break;
					}
				}

				if (i == m_friendList.GetItemCount())
				{
					if (_tcscmp(myself.GetBuffer(), m_recvMsg.userName))
					{
					/*	CString csHeadNum;
						csHeadNum.Format(_T("%d"), m_recvMsg.headNum);
						MessageBox(csHeadNum);*/
						m_friendList.InsertItem(m_friendList.GetItemCount(), m_recvMsg.userName, m_recvMsg.headNum);
			
						_tcscpy(m_user[m_clientCnt].userName, m_recvMsg.userName);
						m_clientCnt++;
					}
				}
				break;
			}
			else
			{
				////删除该好友那项
				for (int m=0; m<m_friendList.GetItemCount(); m++)
				{
					if (!_tcscmp(m_recvMsg.userName, m_friendList.GetItemText(m,0)))
					{
						m_friendList.DeleteItem(m);
					}
				}

				for (int i=0; i<m_clientCnt; i++)
				{
					if (!_tcscmp(m_recvMsg.userName, m_user[i].userName))
					{
						//m_friendList.SetItemText(i, 0, _T(""));
						//m_friendList.DeleteItem(i);
						
						for (int j=i; j<m_clientCnt; j++)
						{
							_tcscpy(m_user[j].userName, m_user[j+1].userName);
						}
						m_clientCnt--;
						break;
					}
				}
			}
		}
		else
		{
			if (m_recvMsg.isSingleSend == TRUE)
			{
				//让哪个窗口显示消息
				int i;
				for (i=0; i<MAX_WINDOW_COUNT; i++)
				{
					if (!_tcscmp(m_whichWnd[i].recvUser, m_recvMsg.sendUser)
						&& !_tcscmp(m_whichWnd[i].sendUser, m_recvMsg.recvUser))
					{
						//判断是否是窗口抖动
						if (m_recvMsg.isWindowShake == TRUE)
						{
							if (m_chatDlg[m_whichWnd[i].wndNum] != NULL)
								WindowShake(m_whichWnd[i].wndNum);
							else
								i = MAX_WINDOW_COUNT;
						}
						else if (m_recvMsg.isFile == TRUE)//判断文件传输
						{
							if (m_chatDlg[m_whichWnd[i].wndNum] != NULL)
								ReceiveFile(m_whichWnd[i].wndNum);
							else
								i = MAX_WINDOW_COUNT;
						}
						else 
						{
							if (m_chatDlg[m_whichWnd[i].wndNum] != NULL)
								((CListBox *)m_chatDlg[m_whichWnd[i].wndNum]->GetDlgItem(IDC_LIST_RECV))->InsertString(-1, m_recvMsg.sendMsg);
							else
								i = MAX_WINDOW_COUNT;
						}
						break;
					}
				}

				if (i == MAX_WINDOW_COUNT)
				{
					//没有创建这个窗口
					//通过弹出框将该信息弹出来
					//if (m_recvMsg.isFile == FALSE || m_recvMsg.isWindowShake == FALSE)
					//	MessageBox(m_recvMsg.sendMsg, _T("你有新的消息，请注意查收"), MB_OK);

					CString sendUser;
					
					for (int y=0; y<m_friendList.GetItemCount(); y++)
					{
						if (!_tcscmp(m_recvMsg.sendUser, 
							m_friendList.GetItemText(y, 0).GetBuffer()))
						{
							CreateChatWindow(m_recvMsg.recvUser, m_recvMsg.sendUser, y);
							//判断是否是窗口抖动
							if (m_recvMsg.isWindowShake == TRUE)
							{
								WindowShake(y);
							}
							else if (m_recvMsg.isFile == TRUE)
							{
								ReceiveFile(y);
							}
							else
								((CListBox *)m_chatDlg[y]->GetDlgItem(IDC_LIST_RECV))->InsertString(-1, m_recvMsg.sendMsg);

							break;
						}
					}
				}
			}
			else
			{
				if (m_chatDlg[0] == NULL)
				{
					//没有创建这个窗口
					//通过弹出框将该信息弹出来
					MessageBox(m_recvMsg.sendMsg, _T("你有新的消息，请注意查收"), MB_OK);
					CString windowText;
					GetWindowText(windowText);
					CreateChatWindow(windowText, _T("QQ群"), 0);
				}
				
				((CListBox *)m_chatDlg[0]->GetDlgItem(IDC_LIST_RECV))->InsertString(-1, m_recvMsg.sendMsg);
			}
		}
		break;
	}

	return 0;
}

SOCKET CFriDlg::GetSockCli(void)
{
	return m_sockCli;
}

LRESULT CFriDlg::OnSubToMain(WPARAM wParam, LPARAM)
{
	m_sendMsg = *((SendMsg *)wParam);

	//send
	int rtval = send(m_sockCli, (char *)&m_sendMsg, sizeof(m_sendMsg), 0);
	//CString sendSize;
	if (SOCKET_ERROR == rtval)
	{
		MessageBox(_T("send 失败"));
	}
	/*else
	{
		sendSize.Format(_T("send %d"), rtval);
		MessageBox(sendSize);
	}*/

	return 0;
}



void CFriDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	//ShowWindow(SW_MINIMIZE);
	ShowWindow(SW_HIDE);
	NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hWnd=m_hWnd;
	CString onLineMsg;
	GetDlgItemText(IDC_ONLINEMSG, onLineMsg);
	lstrcpy(NotifyIcon.szTip, onLineMsg);
	NotifyIcon.uCallbackMessage=WM_NC;
	NotifyIcon.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&NotifyIcon);
}


void CFriDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	//DestroyWindow();
	EndDialog(0);
}


void CFriDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CFriDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BTBK2);

	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CFriDlg::CreateChatWindow(const CString sendUser, 
	const CString recvUser, 
		const UINT wndNum)
{
	m_wndCnt++;
	SubWindowPar subWindowPar;
	subWindowPar.userName = recvUser;
	_tcscpy(m_whichWnd[wndNum].sendUser, sendUser);
	_tcscpy(m_whichWnd[wndNum].recvUser, recvUser);
	m_whichWnd[wndNum].wndNum = wndNum;

	m_chatDlg[wndNum] = new CChatDlg();
	m_chatDlg[wndNum]->Create(IDD_CHATDLG, this);

	m_chatDlg[wndNum]->ShowWindow(SW_SHOW);

	//群聊的话
	if (wndNum == 0)
	{
		m_chatDlg[wndNum]->GetDlgItem(IDC_WNDSHAKE)->ShowWindow(SW_HIDE);
		m_chatDlg[wndNum]->GetDlgItem(IDC_FILE)->ShowWindow(SW_HIDE);
		m_chatDlg[wndNum]->GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
		m_chatDlg[wndNum]->GetDlgItem(IDC_PRO_SEND)->ShowWindow(SW_HIDE);
		subWindowPar.windowTitle = _T("群聊");
		m_chatDlg[wndNum]->SetWindowText(_T("群聊"));
	}
	else
	{
		m_chatDlg[wndNum]->GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
		m_chatDlg[wndNum]->GetDlgItem(IDC_PRO_SEND)->ShowWindow(SW_HIDE);
		m_chatDlg[wndNum]->SetWindowTextW(_T("与 ")+recvUser+_T(" 聊天"));
		subWindowPar.windowTitle = _T("与 ")+recvUser+_T(" 聊天");
	}
	
	m_chatDlg[wndNum]->SendMessage(WM_SUBWINDOW, (WPARAM)&subWindowPar);
}


void CFriDlg::WindowShake(int wndIndex)
{
	Sleep(100);
	CRect rect;
	m_chatDlg[wndIndex]->GetWindowRect(&rect);
	int move=10;
	PlaySound(_T(".\\shake.wav"),NULL,SND_FILENAME | SND_ASYNC);

	for(int z=1;z<9;z++)
	{
		rect.OffsetRect(0,move);
		m_chatDlg[wndIndex]->MoveWindow(&rect);
		Sleep(50);
		rect.OffsetRect(move,0);
		m_chatDlg[wndIndex]->MoveWindow(&rect);
		Sleep(50);
		if (10==move)
		{
			move=-10;
		}
		else
		{
			move=10;
		}
	}
}


void CFriDlg::ReceiveFile(int wndIndex)
{
	if (m_firstRecvFile == TRUE)
	{
		m_chatDlg[wndIndex]->GetDlgItem(IDC_SEND)->ShowWindow(TRUE);
		m_chatDlg[wndIndex]->GetDlgItem(IDC_PRO_SEND)->ShowWindow(TRUE);

		m_firstRecvFile = FALSE;
		CString filePath;
		TCHAR szPath[MAX_PATH];   
		GetModuleFileName(NULL, szPath, MAX_PATH);
		CString PathName(szPath);

		filePath.Format(PathName.Left(PathName.ReverseFind(_T('\\')) + 1)+_T("%s"), m_recvMsg.fileName);
		m_recvFilePath = filePath;
		m_recvFile.Open(m_recvFilePath, CFile::modeCreate | CFile::modeWrite);

	}

	//文件是否传输完毕
	CString recvInfo;
	if (m_recvMsg.isFinished == TRUE)
	{
		m_firstRecvFile = TRUE;

		m_recvFile.Seek(m_recvMsg.fileOffset, CFile::begin);
		m_recvFile.Write(m_recvMsg.sendFile, m_recvMsg.transSize);
		m_recvFile.Flush();
		m_recvFile.Close();
		memset(m_recvMsg.sendFile, 0, MAX_FILE_SIZE);
		CString finishedRecv;
		recvInfo.Format(_T("文件接收进度:%d%%"), 100);
		m_chatDlg[wndIndex]->GetDlgItem(IDC_SEND)->SetWindowText(recvInfo);
		((CProgressCtrl *)m_chatDlg[wndIndex]->GetDlgItem(IDC_PRO_SEND))->SetPos(100);//m_transFilePro.SetPos(100);
		finishedRecv.Format(_T("文件传输完毕，文件存放在%s"), m_recvFilePath);

		MessageBox(finishedRecv, _T("提示"));

		m_chatDlg[wndIndex]->GetDlgItem(IDC_SEND)->ShowWindow(FALSE);
		m_chatDlg[wndIndex]->GetDlgItem(IDC_PRO_SEND)->ShowWindow(FALSE);

		return ;
	}

	m_recvFile.Seek(m_recvMsg.fileOffset, CFile::begin);
	m_recvFile.Write(m_recvMsg.sendFile, m_recvMsg.transSize);
	m_recvFile.Flush();

	recvInfo.Format(_T("文件接收进度:%d%%"), int(((double)m_recvMsg.fileOffset/m_recvMsg.fileLen)*100));
	m_chatDlg[wndIndex]->GetDlgItem(IDC_SEND)->SetWindowText(recvInfo);
	m_chatDlg[wndIndex]->m_transFilePro.SetPos(int(((double)m_recvMsg.fileOffset/m_recvMsg.fileLen)*100));
	memset(m_recvMsg.sendFile, 0, MAX_FILE_SIZE);
}


HBRUSH CFriDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(CTLCOLOR_STATIC == nCtlColor)  
	{ 
		pDC->SetTextColor(RGB(0, 0, 0));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 	
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CFriDlg::OnBnClickedBtnHead()
{
	// TODO: 在此添加控件通知处理程序代码
	CCgHeadDlg headDlg;
	headDlg.DoModal();
}

LRESULT CFriDlg::OnTransIcon(WPARAM wParam, LPARAM lParam)
{
	m_tHIcon = *((HICON*)&wParam);
	return 0;
}

void CFriDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//拦截最小化事件
	if(nID==SC_MINIMIZE) 
	{ 
	   //ToTray();

		//用我们自己的消息响应最小化事件
	   OnBnClickedBtnMin();
	} 
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

LRESULT CFriDlg::OnNotifyIcon(WPARAM wParam,LPARAM IParam)
{
	if ((IParam == WM_LBUTTONDOWN) || (IParam == WM_RBUTTONDOWN))
	{ 
		ModifyStyleEx(0,WS_EX_TOPMOST);
		ShowWindow(SW_SHOW);
	}

	return 0;
}
