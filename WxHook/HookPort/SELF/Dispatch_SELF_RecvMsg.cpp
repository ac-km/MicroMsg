#include <windows.h>
#include <ntsecapi.h>
#include<string>

#include "Dispatch_SELF_RecvMsg.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include "../Inject/RemoteInjection.h"
#include"../Common.h"



//
//Global
//
__pfnWxRecvMsg pfnWxRecvMsg = NULL;



//
//Dispatch_SELF_WxRecvMsg Functions
//
VOID
WINAPI
OnWxRecvMsg()
{
#ifdef Dbg
	DebugLog(DbgInfo, "OnWxRecvMsg...");
#endif

	DWORD* saveEBP = 0/*(DWORD*)OnWxRecvMsg*/;
	__asm
	{
		mov saveEBP,ebp
	}
	saveEBP = (DWORD*)*saveEBP;

	BOOL isGroupMsg = FALSE;//Ⱥ��Ϣ���Ǹ�����Ϣ
	BOOL isSysMsg = FALSE;//ϵͳ��Ϣ�����շ�Ϊ"weixin"
	BOOL isChatMsg = FALSE;//˽��
	std::string sender = "";
	std::string receiver = "";
	std::string msg = "";

	//���ͷ�:Ⱥ�ĵĻ�Ϊ����ţ�˽��Ϊwxid,ϵͳ��ϢΪ"weixin"
	{
		//0x554�洢Ϊ����
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x558);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			sender = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog(DbgInfo, (char*)sender.c_str());
#endif
		}
	}

	//���շ�������¼��wxid
	{
		//0x598�洢Ϊ����
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x59c);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			receiver = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog(DbgInfo, (char*)receiver.c_str());
#endif
		}
	}

	//�ж���Ϣ���ͣ����ݷ�����
	if (sender.find("@chatroom") != std::string::npos)
	{
		isGroupMsg = TRUE;
#ifdef Dbg
		DebugLog(DbgInfo, "group msg");
#endif
	}
	else if (sender.find("wxid_") != std::string::npos)
	{
		isChatMsg = TRUE;
#ifdef Dbg
		DebugLog(DbgInfo, "chat msg");
#endif
	}
	else if (strcmp(sender.c_str(),"weixin")==0)
	{
		isSysMsg = TRUE;
#ifdef Dbg
		DebugLog(DbgInfo, "sys msg");
#endif
	}
	else
	{
		isChatMsg = TRUE;
#ifdef Dbg
		DebugLog(DbgInfo, "chat msg");
#endif
	}

	//��Ϣ����:
	if (isSysMsg)
	{
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x734);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			msg = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog(DbgInfo, (char*)msg.c_str());
#endif
		}
	}
	else if (isChatMsg)
	{
//		CHAR *wxBuf = (CHAR*)*(DWORD*)((char*)saveEBP - 0x204);
//		DWORD len = strlen(wxBuf);
//		if (len> 0)
//		{
//			msg.append(wxBuf, len);
//#ifdef Dbg
//			DebugLog(DbgInfo, (char*)msg.c_str());
//#endif
//		}
	}
	else if (isGroupMsg)
	{
//		CHAR *wxBuf = (CHAR*)*(DWORD*)((char*)saveEBP - 0x204);
//		DWORD len = strlen(wxBuf);
//		if (len> 0)
//		{
//			msg.append(wxBuf, len);
//#ifdef Dbg
//			DebugLog(DbgInfo, (char*)msg.c_str());
//#endif
//		}
//		wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x734);
//		len = GetWBufferLen(wxBuf);
//		if (len> 0)
//		{
//			msg.push_back('(');
//			msg.append(MyWideCharToMultiByte((BYTE*)wxBuf, len));
//			msg.push_back(')');
//#ifdef Dbg
//			DebugLog(DbgInfo, (char*)msg.c_str());
//#endif
//		}
	}
	pipe_start_thread((LPVOID)((sender+","+ receiver).c_str()));

	__asm
	{
		mov esp,ebp
		pop ebp
		jmp pfnWxRecvMsg;
	}
}