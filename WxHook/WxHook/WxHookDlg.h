
// WxHookDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MyListCtrl.h"
#include"ParseProto.h"
#include<vector>

// CWxHookDlg �Ի���
class CWxHookDlg : public CDialogEx
{
// ����
public:
	CWxHookDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WXHOOK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
