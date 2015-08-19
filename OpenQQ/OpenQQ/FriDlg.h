#pragma once

#include "afxcmn.h"
#include "userinfo.h"

#include "WhiWnd.h"
#include "sendmsg.h"
#include "afx.h"
#include "afxwin.h"

#define WM_SOCK WM_USER+1
#define WM_SUBTOMAIN WM_USER+5
#define WM_TRANSICON WM_USER+10
 #define WM_NC WM_USER+15

// CFriDlg 对话框
class CChatDlg;

class CFriDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriDlg)

public:
	CFriDlg(CWnd* pParent = NULL);   // 标准构造函数
	BOOL InitSocket(void);
	virtual ~CFriDlg();

// 对话框数据
	enum { IDD = IDD_FRIEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnSock(WPARAM, LPARAM);
	afx_msg LRESULT OnSubToMain(WPARAM, LPARAM);
	afx_msg LRESULT OnTransIcon(WPARAM, LPARAM);

	afx_msg LRESULT OnNotifyIcon(WPARAM wParam,LPARAM IParam);

	DECLARE_MESSAGE_MAP()
public:
	void InitListCtr(void);
	virtual BOOL OnInitDialog();
	CListCtrl m_friendList;
	afx_msg void OnNMClickListFriend(NMHDR *pNMHDR, LRESULT *pResult);
private:
	SOCKET m_sockCli;
	UserInfo m_user[200];
	int m_clientCnt;
public:
	SOCKET GetSockCli(void);
public:
	CChatDlg *m_chatDlg[MAX_WINDOW_COUNT];
private:
	WhiWnd m_whichWnd[20];
	SendMsg m_recvMsg;
	SendMsg m_sendMsg;
	int m_wndCnt;
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
private:
	BOOL m_firstRecvFile;
	CString m_recvFilePath;
	CFile m_recvFile;
public:
	void CreateChatWindow(const CString sendUser, 
		const CString recvUser, 
			const UINT wndNum);
	void WindowShake(int wndIndex);
	void ReceiveFile(int wndIndex);
private:
	CImageList m_imageList;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnHead();
private:
	// //主窗口传来的图标句柄
	HICON m_tHIcon;

private: // 托盘实现
	NOTIFYICONDATA NotifyIcon;
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
