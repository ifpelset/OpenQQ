// RegDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenQQ.h"
#include "RegDlg.h"
#include "afxdialogex.h"
#include "OpenQQDlg.h"
#include "UserInfo.h"

// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialogEx)

CRegDlg::CRegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegDlg::IDD, pParent)
{

}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialogEx)
	ON_WM_PAINT()

ON_BN_CLICKED(IDC_REG, &CRegDlg::OnBnClickedReg)
ON_BN_CLICKED(IDC_BTN_CLOSE, &CRegDlg::OnBnClickedBtnClose)
ON_WM_LBUTTONDOWN()
ON_WM_PAINT()
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRegDlg 消息处理程序

void CRegDlg::OnBnClickedReg()
{
	// TODO: 在此添加控件通知处理程序代码

	 //MessageBox(_T("注册"));
	if (FALSE == ((COpenQQDlg *)GetParent())->InitSocket())
	{
		return ;
	}

	//创建一个用户结构
	UserInfo user;
	user.bOnLine = FALSE;
	user.accFlag = FALSE;
	user.isRegister = TRUE;
	user.headNum = 0; //注册默认就用头像0

	//获取用户信息
	GetDlgItemText(IDC_USERNAME, user.userName, 20);
	GetDlgItemText(IDC_PASSWD, user.passWord, 20);
	CString passWord2;
	GetDlgItemText(IDC_PASSWD2, passWord2);

	//判断用户名和密码不能为空
	if (_tcscmp(user.userName, _T("")) 
		&&	_tcscmp(user.passWord, _T("")))
	{
		if (_tcscmp(user.passWord, passWord2))
		{
			MessageBox(_T("两次密码输入不匹配，请重新输入！"));
			return ;
		}

		send(((COpenQQDlg *)GetParent())->GetSockCli(), (char *)&user, sizeof(user), 0);

		char chReg[40];

		recv(((COpenQQDlg *)GetParent())->GetSockCli(), chReg, 40, 0);
		if (!strcmp(chReg, "Register success!"))
		{
			MessageBox(_T("注册成功！"));
		}
		else
		{
			MessageBox(_T("该用户名已经存在，注册失败！"));
		}
	}
	else
	{
		MessageBox(_T("用户名或密码不能为空！"), _T("警告"), MB_ICONWARNING);
	}
}


void CRegDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}


void CRegDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRegDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BTREGBK);

	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
}


HBRUSH CRegDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(CTLCOLOR_STATIC == nCtlColor)
	{ 
		//pDC->SetBkColor(RGB(255,0,0));  //需要指定背景色的时候用 
		pDC->SetTextColor(RGB(0,   0,   0));    //文字前景色
		pDC->SetBkMode(TRANSPARENT);    //设置透明
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);  //获取画笔颜色混合后的画笔，完成透明 
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
