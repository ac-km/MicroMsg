
// WxHookDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MyListCtrl.h"
#include"ParseProto.h"
#include<vector>

// CWxHookDlg 对话框
class CWxHookDlg : public CDialogEx
{
// 构造
public:
	CWxHookDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WXHOOK_DIALOG };

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
	afx_msg LRESULT OnWndMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
private:
	CString strWxPath;
	CListBox mViewList;
	MyListCtrl mListView;
	ParseProto mParseProto;
	std::vector<std::string> mMessageList;
private:
	CString GetWxPath();
	void start_pipe();
	void LoadProto(CString &mProtoFile);
	std::string ParseData(std::string &message_name,unsigned char *data, unsigned int len);
};
