// CgHeadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenQQ.h"
#include "CgHeadDlg.h"
#include "afxdialogex.h"

#include "OpenQQDlg.h"

#include "FriDlg.h"

// CCgHeadDlg 对话框

IMPLEMENT_DYNAMIC(CCgHeadDlg, CDialogEx)

CCgHeadDlg::CCgHeadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCgHeadDlg::IDD, pParent)
{

}

CCgHeadDlg::~CCgHeadDlg()
{
}

void CCgHeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCgHeadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNHEAD1, &CCgHeadDlg::OnBnClickedBtnhead1)
	ON_BN_CLICKED(IDC_BTNHEAD2, &CCgHeadDlg::OnBnClickedBtnhead2)
	ON_BN_CLICKED(IDC_BTNHEAD3, &CCgHeadDlg::OnBnClickedBtnhead3)
	ON_BN_CLICKED(IDC_BTNHEAD4, &CCgHeadDlg::OnBnClickedBtnhead4)
	ON_BN_CLICKED(IDC_BTNHEAD5, &CCgHeadDlg::OnBnClickedBtnhead5)
	ON_BN_CLICKED(IDC_BTNHEAD6, &CCgHeadDlg::OnBnClickedBtnhead6)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CCgHeadDlg::OnBnClickedBtnClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCgHeadDlg 消息处理程序


BOOL CCgHeadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//添加头像
	HBITMAP hBmp;
	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD1));
	((CButton *)GetDlgItem(IDC_BTNHEAD1))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD2));
	((CButton *)GetDlgItem(IDC_BTNHEAD2))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD3));
	((CButton *)GetDlgItem(IDC_BTNHEAD3))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD4));
	((CButton *)GetDlgItem(IDC_BTNHEAD4))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD5));
	((CButton *)GetDlgItem(IDC_BTNHEAD5))->SetBitmap(hBmp);

	hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BTHEAD6));
	((CButton *)GetDlgItem(IDC_BTNHEAD6))->SetBitmap(hBmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCgHeadDlg::OnBnClickedBtnhead1()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxGetMainWnd  获取主窗口
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD1)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD1));

	UpdateHeadPic(1);
}


void CCgHeadDlg::OnBnClickedBtnhead2()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD2)));
	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD2));

	UpdateHeadPic(2);
}

void CCgHeadDlg::OnBnClickedBtnhead3()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD3)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD3));

	UpdateHeadPic(3);
}


void CCgHeadDlg::OnBnClickedBtnhead4()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD4)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD4));

	UpdateHeadPic(4);
}


void CCgHeadDlg::OnBnClickedBtnhead5()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD5)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD5));

	UpdateHeadPic(5);
}


void CCgHeadDlg::OnBnClickedBtnhead6()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton *)AfxGetMainWnd()->GetDlgItem(IDC_BTNHEAD))->SetBitmap(LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BTHEAD6)));

	((CButton *)GetParent()->GetDlgItem(IDC_BTN_HEAD))->SetIcon(AfxGetApp()->LoadIcon(IDI_HEAD6));

	UpdateHeadPic(6);
}


void CCgHeadDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	//向服务器同步头像

	CDialogEx::OnCancel();
}


void CCgHeadDlg::UpdateHeadPic(UINT headNum)
{
	SendMsg sendMsg;
	CString windowText;
	((CFriDlg *)GetParent())->GetWindowText(windowText);
	sendMsg.isHeadPic = TRUE;
	_tcscpy(sendMsg.userName, windowText);
	sendMsg.headNum = headNum;
	send(((CFriDlg *)GetParent())->GetSockCli(), (char *)&sendMsg, sizeof(sendMsg), 0);
}

void CCgHeadDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}


void CCgHeadDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND,0xF012,0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCgHeadDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BTBK4);

	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH CCgHeadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
