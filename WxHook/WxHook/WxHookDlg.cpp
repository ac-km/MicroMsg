
// WxHookDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WxHook.h"
#include "WxHookDlg.h"
#include "afxdialogex.h"
#include"../Common/Config.h"
#include"../Common/Common.h"
#include"pipe.h"
#include"Lock.h"

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
	, mEditValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWxHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, strWxPath);
	DDX_Control(pDX, IDC_LIST2, mListView);
	DDX_Text(pDX, IDC_EDIT1, mEditValue);
}

BEGIN_MESSAGE_MAP(CWxHookDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWxHookDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWxHookDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_WND_MSG, &CWxHookDlg::OnWndMsg)
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

	// ��ʼ������·��
	//strWxPath = "C:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe";
	GetWxPath();

	//listcontrol�ؼ���ʼ��
	char  column[][MAX_HEADLENGTH] = { "���","��Ϣ����","��Ϣ����" };
	mListView.SetHeaders(column, sizeof(column) / sizeof(*column));
	mListView.MoveWindow(10, 75, 200, 265,TRUE);

	//hexview�ؼ���ʼ��
	pHexView = HexEditControl::ShowHexControl(this->m_hWnd, 215, 75, 495, 265);

	//�����ܵ�
	start_pipe();

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
void CWxHookDlg::GetWxPath()
{
	HKEY hKey;
	TCHAR szProductType[MAX_PATH];
	memset(szProductType, 0, sizeof(szProductType));
	DWORD dwBufLen = MAX_PATH;
	LONG lRet;

	// �����Ǵ�ע���, ֻ�д򿪺�������������� 
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,  // Ҫ�򿪵ĸ��� 
		TEXT("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\WeChat"), // Ҫ�򿪵����Ӽ� 
		0,        // ���һ��ҪΪ0 
		KEY_QUERY_VALUE,  //  ָ���򿪷�ʽ,��Ϊ�� 
		&hKey);    // �������ؾ�� 

	if (lRet != ERROR_SUCCESS)   // �ж��Ƿ�򿪳ɹ� 
		return;

	lRet = RegQueryValueEx(hKey,  // ��ע���ʱ���صľ�� 
		TEXT("InstallLocation"),  //Ҫ��ѯ������,qq��װĿ¼��¼��������� 
		NULL,   // һ��ΪNULL����0 
		NULL,
		(LPBYTE)szProductType, // ����Ҫ�Ķ����������� 
		&dwBufLen);
	if (lRet != ERROR_SUCCESS)  // �ж��Ƿ��ѯ�ɹ� 
		return;

	char *exe = "\\WeChat.exe";
	memcpy(szProductType + strlen(szProductType), exe, strlen(exe)+1);
	strWxPath = szProductType;
	UpdateData(FALSE);
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
		strWxPath
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
		//start_pipe();
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
	unsigned char buffer[] = {
		0x08,0x02,0x12,0x27,0x08,0x01,0x10,0xCB,0xB1,0xB0,0xBD,0x03,0x18,0x04,0x22,0x1B,0x08,0x17,0x12,0x17,0x0A,0x13,0x77,0x78,0x69,0x64,0x5F,0x76,0x34,0x78,0x33,0x74,0x76,0x36,0x70,0x68,0x63,0x77,0x69,0x32,0x31,0x10,0x07,0x12,0x18,0x08,0x03,0x10,0xCB,0xB1,0xB0,0xBD,0x03,0x18,0x05,0x22,0x0C,0x08,0x08,0x12,0x08,0x08,0x04,0x12,0x04,0x05,0x00,0x00,0x00,0x18,0x06,
	};
	mParseProto.PrintDebugString(buffer, sizeof(buffer));
}

void CWxHookDlg::start_pipe()
{
	DWORD dwThreadId = 0;
	CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		pipe_server,//thread function
		&m_hWnd,//argument to thread function
		0,//use default creation flags
		&dwThreadId);//returns the thread identifier
	
	//Check there turn value for success.
	if (dwThreadId == NULL)
	{
		return;
	}

}

afx_msg LRESULT CWxHookDlg::OnWndMsg(WPARAM wParam, LPARAM lParam)
{
	{
		AutoLock auto_lock;
		DWORD code = wParam;
		unsigned char *p = (unsigned char*)lParam;
		switch (code)
		{
		case 1:
		{
			DWORD custom_type;
			DWORD msg_type;
			std::string out;
			if (syncunpack(p,custom_type, msg_type,out))
			{
				if (out.size() > 0)
				{
					switch (custom_type)
					{
					case MSG_CUSTOM:
					case MSG_CUSTOM01:
					case MSG_CUSTOM02:
					case MSG_CUSTOM03:
					{
						int i = items.size();
						ViewItem item;
						item.index = i + 1;
						item.custom_type = custom_type;
						item.msg_type = msg_type;
						item.data = out;
						items.push_back(item);

						CString strFormat;
						strFormat.Format("%d", item.index);
						mListView.InsertItem(i, strFormat);
						strFormat.Format("%s", "custom");
						mListView.SetItemText(i, 1, strFormat);
						strFormat.Format("%s", item.data.c_str());
						mListView.SetItemText(i, 2, strFormat);
						mListView.SetItemData(i, (DWORD_PTR)&items.back());
					}break;
					case MSG_CUSTOMFF:
					{
						switch (msg_type)
						{
						case MSG_01:
						//{
						//	CString mProtoFile = "proto\\MicroMsg01.proto";
						//	LoadProto(mProtoFile);
						//	std::string msg_name = "";
						//	std::string ret=ParseData(msg_name, (unsigned char*)out.c_str(), out.size());
						//	mViewList.AddString(ret.c_str());
						//}break;
						default:
						{
							int i = items.size();
							ViewItem item;
							item.index = i + 1;
							item.custom_type = custom_type;
							item.msg_type = msg_type;
							item.data = out;
							items.push_back(item);
							
							CString strFormat;
							strFormat.Format("%d", item.index);
							mListView.InsertItem(i, strFormat);
							strFormat.Format("%s", "proto");
							mListView.SetItemText(i, 1, strFormat);
							mListView.SetItemText(i, 2, "...");
							mListView.SetItemData(i, (DWORD_PTR)&items.back());
						}break;
						}
					}break;
					default:
						return false;
						break;
					}
				}
			}
		}break;
		case 2:
		{
			int index = lParam;
			PViewItem p=(PViewItem)(mListView.GetItemData(index));
			HexEditControl::SetData(pHexView, (unsigned char*)p->data.c_str(), p->data.size());
			UpdateData(TRUE);
			//protobuf�ṹ����ת��
			if (p->custom_type == MSG_CUSTOMFF)
			{
				//ԭʼprotobuf��
				std::string ret = mParseProto.PrintDebugString((unsigned char*)p->data.c_str(), p->data.size());
				mEditValue=(ret.c_str());
			}
			else
			{
				mEditValue=(p->data.c_str());
			}
			mEditValue.Replace("\n", "\r\n");
			UpdateData(FALSE);
		}break;
		default:
			UpdateData(TRUE);
			mEditValue=("ERROR\n");
			UpdateData(FALSE);
			break;
		}
	}
	Sleep(1);
	return 0;
}

void CWxHookDlg::LoadProto(CString &mProtoFile)
{
	//����
	mMessageList.clear();
	unsigned int len1 = mProtoFile.GetLength();//proto�ļ�·������
	if (len1)
	{
		std::string str = mProtoFile.GetString();
		mParseProto.LoadProtoFromFile(str);
		std::vector<std::string> Messages = mParseProto.GetMessageList();
		for (int i = 0; i < Messages.size(); i++)
		{
			mMessageList.push_back(Messages[i]);
		}
	}
}

std::string CWxHookDlg::ParseData(std::string &message_name,unsigned char *data, unsigned int len)
{
	std::string strResult = "";
	//��̬����
	if (len)
	{
		CString strMessage = "";
		strResult = mParseProto.PrintDataFile(message_name, data, len);
		if (strResult == "")
		{
			strResult = "��������...";
		}
		else
		{
		}
	}
	return strResult;
}