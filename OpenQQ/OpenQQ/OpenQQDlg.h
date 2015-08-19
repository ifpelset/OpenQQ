
// OpenQQDlg.h : 头文件
//

#pragma once
#include "atltypes.h"

// COpenQQDlg 对话框
class COpenQQDlg : public CDialogEx
{
// 构造
public:
	COpenQQDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COpenQQDlg();
// 对话框数据
	enum { IDD = IDD_OPENQQ_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnlogin();
	BOOL InitSocket(void);
private:
	SOCKET m_sockCli;
public:
	SOCKET GetSockCli(void);
	afx_msg void OnBnClickedBtnSet();
	void InitWindowSize(void);
private:
	CRect m_largeRect;
	CRect m_smallRect;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnNMClickReg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickReturn(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAutologin();
	afx_msg void OnBnClickedRempasswd();
	void LoadProfiles(void);
private:
	int m_profileAppnameCnt;
public:
	afx_msg void OnCbnSelchangeUsername();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnAbout();
//	afx_msg void OnBnClickedBtnhead();
	void LoadHeadPic(UINT headNum);
	UINT WhichHeadPic(void);
	HICON GetHICONByHeadNum(UINT headNum);
};
