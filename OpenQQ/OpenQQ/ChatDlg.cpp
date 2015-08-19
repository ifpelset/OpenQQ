// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenQQ.h"
#include "ChatDlg.h"
#include "afxdialogex.h"

#include "OpenQQDlg.h"

#include "FriDlg.h"

#include "SubWindowPar.h"
// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
{
	m_sendMsg.isSingleSend = FALSE;
	m_sendMsg.isFile = FALSE;
}

CChatDlg::~CChatDlg()
{
	//closesocket(m_sockCli);
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRO_SEND, m_transFilePro);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNSEND, &CChatDlg::OnBnClickedBtnsend)
	ON_MESSAGE(WM_SUBWINDOW, OnSubWindow)
	ON_WM_LBUTTONDOWN()
//	ON_BN_CLICKED(IDC_BTN_MIN, &CChatDlg::OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CChatDlg::OnBnClickedBtnClose)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_WNDSHAKE, &CChatDlg::OnBnClickedWndshake)
	ON_BN_CLICKED(IDC_FILE, &CChatDlg::OnBnClickedFile)
END_MESSAGE_MAP()


// CChatDlg 消息处理程序


void CChatDlg::OnBnClickedBtnsend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sendMsg;
	char sendBuff[256];
	memset(sendBuff, 0, 256);
	GetDlgItemText(IDC_EDIT_SEND, sendMsg);
	m_sendMsg.isFile = FALSE;
	m_sendMsg.isWindowShake = FALSE;

	_tcscpy(m_sendMsg.sendMsg, sendMsg.GetBuffer());

	((CFriDlg *)GetParent())->GetWindowText(m_sendMsg.sendUser, 20);
	CString recvUser;
	GetWindowText(recvUser);
	int begin = recvUser.Find(_T("聊"));
	_tcscpy(m_sendMsg.recvUser, recvUser.Mid(2, begin-3));

	//发送到主窗口
	//WM_SUBTOMAIN	WM_USER+5
	((CFriDlg *)GetParent())->SendMessage(WM_SUBTOMAIN, (WPARAM)&m_sendMsg);
	SetDlgItemText(IDC_EDIT_SEND, _T(""));
}

LRESULT CChatDlg::OnSubWindow(WPARAM wParam, LPARAM lParam)
{
	//CString strName;// = *((CString*)wParam);
	SubWindowPar subWindowPar = *((SubWindowPar *)wParam);

	GetDlgItem(IDC_CHATWITH)->SetWindowText(subWindowPar.windowTitle);
	//群聊
	if (subWindowPar.userName == _T("QQ群"))
	{
		m_sendMsg.isSingleSend = FALSE;
	}
	else //私聊
	{
		m_sendMsg.isSingleSend = TRUE;
		_tcscpy(m_sendMsg.recvUser, subWindowPar.userName.GetBuffer());
	}

	//WSAAsyncSelect(m_sockCli,m_recvHwnd,WM_SOCK,FD_READ);
	return 0;
}

BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	//InitSocket();

	HBITMAP   hBitmap; 
	hBitmap = LoadBitmap(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_BTWNDSHAKE)); // IDB_BITMAP_TEST为资源图片ID
	((CButton *)GetDlgItem(IDC_WNDSHAKE))->SetBitmap(hBitmap); 

	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTFILE));
	((CButton *)GetDlgItem(IDC_FILE))->SetBitmap(hBitmap); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChatDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	for (int i=0; i<MAX_WINDOW_COUNT;)
	{
		if (((CFriDlg *)GetParent())->m_chatDlg[i] == NULL)
		{
			i++;
		}
		else
		{
			if (((CFriDlg *)GetParent())->m_chatDlg[i]->m_hWnd == m_hWnd)
			{
				((CFriDlg *)GetParent())->m_chatDlg[i] = NULL;
				break;
			}
			else
			{
				i++;
			}
		}
	}
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void CChatDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::PostNcDestroy();
	
	delete this;
}

void CChatDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


//void CChatDlg::OnBnClickedBtnMin()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	ShowWindow(SW_MINIMIZE);
//}


void CChatDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	//DestroyWindow();
	OnCancel();
}


void CChatDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BTBK3);

	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CChatDlg::OnBnClickedWndshake()
{
	// TODO: 在此添加控件通知处理程序代码

	//发送一个窗口抖动的消息
	//_tcscpy(m_sendMsg.sendMsg, _T("~w&i*n(d)o$w#s@h^a_k+e~"));
	m_sendMsg.isWindowShake = TRUE;//窗口抖动消息
	m_sendMsg.isFile = FALSE;
	//m_sendMsg.isMessage = FALSE;
	
	((CFriDlg *)GetParent())->GetWindowText(m_sendMsg.sendUser, 20);
	CString recvUser;
	GetWindowText(recvUser);
	int begin = recvUser.Find(_T("聊"));
	_tcscpy(m_sendMsg.recvUser, recvUser.Mid(2, begin-3));
	
	//发送到主窗口
	//WM_SUBTOMAIN	WM_USER+5
	((CFriDlg *)GetParent())->SendMessage(WM_SUBTOMAIN, (WPARAM)&m_sendMsg);
}


void CChatDlg::OnBnClickedFile()
{
	// TODO: 在此添加控件通知处理程序代码
	//这里需要弹出一个选择文件的窗口
	CFileDialog fileDlg(TRUE);
	fileDlg.m_ofn.lpstrTitle = _T("请选择你要发送的文件");
	fileDlg.m_ofn.lpstrFilter = _T("All Files(*.*)\0*.*\0\0");

	if (IDOK == fileDlg.DoModal())
	{
		GetDlgItem(IDC_SEND)->ShowWindow(TRUE);
		GetDlgItem(IDC_PRO_SEND)->ShowWindow(TRUE);

		CFile file;
		file.Open(fileDlg.GetPathName(), CFile::modeRead);
		int bytesRead = 0;
		long fileOffset = 0;

		ULONGLONG fileLen = file.GetLength();
		m_sendMsg.fileLen = fileLen;
		((CFriDlg *)GetParent())->GetWindowText(m_sendMsg.sendUser, 20);
		CString recvUser;
		GetWindowText(recvUser);
		int begin = recvUser.Find(_T("聊"));
		_tcscpy(m_sendMsg.recvUser, recvUser.Mid(2, begin-3));
		m_sendMsg.isFile = TRUE;
		_tcscpy(m_sendMsg.fileName, fileDlg.GetFileName().GetBuffer());

		CString sendInfo;
		
		while (fileLen > MAX_FILE_SIZE)
		{
			m_sendMsg.isFinished = FALSE;
			m_sendMsg.isFile = TRUE;
			m_sendMsg.isWindowShake = FALSE;
			//m_sendMsg.isMessage = FALSE;

			file.Seek(fileOffset, CFile::begin);
			
			bytesRead = MAX_FILE_SIZE;

			file.Read(m_sendMsg.sendFile, bytesRead);
			m_sendMsg.isFile = TRUE;
			//发送到主窗口
			//WM_SUBTOMAIN	WM_USER+5
			m_sendMsg.fileOffset = fileOffset;
			m_sendMsg.transSize = bytesRead;
			((CFriDlg *)GetParent())->SendMessage(WM_SUBTOMAIN, (WPARAM)&m_sendMsg);
			
			sendInfo.Format(_T("文件发送进度:%d%%"), int(((double)fileOffset/m_sendMsg.fileLen)*100));
			m_transFilePro.SetPos(int(((double)fileOffset/m_sendMsg.fileLen)*100));
			GetDlgItem(IDC_SEND)->SetWindowText(sendInfo);

			fileOffset += bytesRead;
			fileLen -= MAX_FILE_SIZE;

			Sleep(70);
		}
		
		file.Seek(fileOffset, CFile::begin);
		
		if (fileLen <= MAX_FILE_SIZE)
		{
			m_sendMsg.isFinished = TRUE;
			m_sendMsg.isFile = TRUE;
			m_sendMsg.isWindowShake = FALSE;
			//m_sendMsg.isMessage = FALSE;

			file.Read(m_sendMsg.sendFile, fileLen);
	
			m_sendMsg.fileOffset = fileOffset;
			m_sendMsg.transSize = bytesRead;

			

			((CFriDlg *)GetParent())->SendMessage(WM_SUBTOMAIN, (WPARAM)&m_sendMsg);
			sendInfo.Format(_T("文件发送进度:%d%%"), 100);
			m_transFilePro.SetPos(100);
			GetDlgItem(IDC_SEND)->SetWindowText(sendInfo);

			MessageBox(_T("文件发送完毕！"), _T("提示"));

			GetDlgItem(IDC_SEND)->ShowWindow(FALSE);
			GetDlgItem(IDC_PRO_SEND)->ShowWindow(FALSE);
		}

		file.Close();				
	}
}
