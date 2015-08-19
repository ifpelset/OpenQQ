
// OpenQQSerDlg.h : 头文件
//

#pragma once
#include "userinfo.h"
#include "afxcmn.h"
#include "sendmsg.h"
#include "afx.h"

#define WM_SOCK WM_USER+1

// COpenQQSerDlg 对话框
class COpenQQSerDlg : public CDialogEx
{
// 构造
public:
	COpenQQSerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OPENQQSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg LRESULT OnSock(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedCancel();
private:
	// //服务器记录在线客户端的数量
	int m_clientCnt;
public:
	afx_msg void OnBnClickedBtnbg();
	BOOL InitSocket(void);
private:
	SOCKET m_sockSrv;
	SOCKET m_sockCli[256];
public:
	afx_msg void OnBnClickedBtned();
private:
	UserInfo m_user[255];
	CListCtrl m_userList;
public:
	void InitListCtr(void);
private:
	SendMsg m_recvMsg;
public:
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	CFile m_recvFile;
	BOOL m_firstRecvFile;
	CString m_recvFilePath;
public:
	BOOL VerifyUser(const CString& vUserName, const CString& vUserPasswd);
	BOOL WriteRegInfoToProfile(const CString &wUserName, const CString &wPasswd, const UINT & headNum);
private:
	int m_registerUserCnt;
public:
	void LoadClientCnt(void);
	BOOL VerifyRepeatLogin(const CString & loginUserName);
	void UpdateHeadPic(const CString & userName, const UINT & headNum);
private:
	UINT m_tHeadNum;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
