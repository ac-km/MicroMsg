
// WxHookDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WxHook.h"
#include "WxHookDlg.h"
#include "afxdialogex.h"
#include"../Common/Config.h"
#include"pipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CWxHookDlg �Ի���



CWxHookDlg::CWxHookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWxHookDlg::IDD, pParent)
	, strWxPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWxHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, strWxPath);
	DDX_Control(pDX, IDC_LIST1, mViewList);
}

BEGIN_MESSAGE_MAP(CWxHookDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWxHookDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWxHookDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWxHookDlg ��Ϣ�������

BOOL CWxHookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������c
	strWxPath = "C:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe";
	//strWxPath = "E:\\�������\\xsearch_CHS.exe";

	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWxHookDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWxHookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWxHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ȡ΢��Ŀ¼
CString CWxHookDlg::GetWxPath()
{
	//HKEY hKey;
	//TCHAR szProductType[MY_BUFSIZE];
	//memset(szProductType, 0, sizeof(szProductType));
	//DWORD dwBufLen = MY_BUFSIZE;
	//LONG lRet;

	//// �����Ǵ�ע���, ֻ�д򿪺�������������� 
	//lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,  // Ҫ�򿪵ĸ��� 
	//	TEXT("SOFTWARE\\TENCENT\\QQ2009"), // Ҫ�򿪵����Ӽ� 
	//	0,        // ���һ��ҪΪ0 
	//	KEY_QUERY_VALUE,  //  ָ���򿪷�ʽ,��Ϊ�� 
	//	&hKey);    // �������ؾ�� 

	//if (lRet != ERROR_SUCCESS)   // �ж��Ƿ�򿪳ɹ� 
	//	return 1;
	UpdateData(TRUE);
	return strWxPath;
}
//����΢��
void CWxHookDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CHAR szLauncherPath[MAX_PATH] = { NULL };
	GetModuleFileName(NULL, szLauncherPath, MAX_PATH);
	PathRemoveFileSpec(szLauncherPath);
	lstrcat(szLauncherPath, "\\");
	lstrcat(szLauncherPath, CONF_Launcher_FileName);

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	StartupInfo.lpDesktop = NULL;
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOWNORMAL;

	//
	//Command Line
	//
	//(Launcher.exe -exe [exe Name])
	CHAR szCommandLine[512] = { NULL };
	wsprintf(
		szCommandLine,
		"%s",
		GetWxPath()
		);
	//�ж�szCommandLine�Ƿ��пո�
	CString szDest = szCommandLine;
	// �ж��Ƿ��пո�
	if (szDest.Find(' ') != -1)
	{
		// �пո�,��ת�������
		CString szSpaceFormat = " \"%s\"";
		szDest.Format(szSpaceFormat, szCommandLine);
	}

	BOOL bIsLauncherCalled = FALSE;
	if (CreateProcess(
		szLauncherPath,
		szDest.GetBuffer(),
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo
		))
	{
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);

		bIsLauncherCalled = TRUE;
		start_pipe();
	}
	else
	{
		bIsLauncherCalled = FALSE;
		AfxMessageBox("����Ŀ�����ʧ��");
	}
}

//ˢ���б�
void CWxHookDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mViewList.AddString("11111111111111111111");
}

void CWxHookDlg::start_pipe()
{
	DWORD dwThreadId = 0;
	CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		pipe_server,//thread function
		&mViewList.m_hWnd,//argument to thread function
		0,//use default creation flags
		&dwThreadId);//returns the thread identifier
						//Check there turn value for success.
	if (dwThreadId == NULL)
	{
		return;
	}

}