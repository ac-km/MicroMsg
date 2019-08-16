#include <windows.h>
#include <ntsecapi.h>
#include<string>

#include "Dispatch_SELF_RecvMsg.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include "../Inject/RemoteInjection.h"
#include"../../Common/Common.h"
#include"../pipe.h"



//
//Global
//
__pfnWxRecvMsg pfnWxRecvMsg = NULL;
__pfnWxRecvMsg pfnWxRecvMsgForStream = NULL;



//
//Dispatch_SELF_WxRecvMsg Functions
//
VOID
WINAPI
OnWxRecvMsg()
{
#ifdef Dbg
	DebugLog("HookCallBack", "OnWxRecvMsg...");
#endif

	DWORD* saveEBP = 0;
	__asm
	{
		mov saveEBP,ebp
	}
	saveEBP = (DWORD*)*saveEBP;

	BOOL isGroupMsg = FALSE;//群消息还是个人消息
	BOOL isSysMsg = FALSE;//系统消息，接收方为"weixin"
	BOOL isChatMsg = FALSE;//私聊
	BOOL isGHMsg = FALSE;//公众号推送
	BOOL isUnknow = FALSE;
	std::string sender = "";
	std::string receiver = "";
	std::string msg = "";

	//发送方:群聊的话为房间号，私聊为wxid,系统消息为"weixin"
	{
		//0x554存储为长度
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x558);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			sender = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog("sender", (char*)sender.c_str());
#endif
		}
	}

	//接收方：即登录者wxid
	{
		//0x598存储为长度
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x59c);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			receiver = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog("receiver", (char*)receiver.c_str());
#endif
		}
	}

	//判断消息类型：根据发送者
	if (sender.find("@chatroom") != std::string::npos)
	{
		isGroupMsg = TRUE;
#ifdef Dbg
		DebugLog("group msg");
#endif
	}
	else if (sender.find("wxid_") != std::string::npos)
	{
		if (strcmp(sender.c_str(),receiver.c_str())!=0)
			isChatMsg = TRUE;
		else
			isSysMsg = TRUE;
#ifdef Dbg
		DebugLog("chat msg");
#endif
	}
	else if (strcmp(sender.c_str(),"weixin")==0)
	{
		isSysMsg = TRUE;
#ifdef Dbg
		DebugLog("sys msg");
#endif
	}
	else if (sender.find("gh_") != std::string::npos)
	{
		isGHMsg = TRUE;
#ifdef Dbg
		DebugLog("gh msg");
#endif
	}else
	{
		isUnknow = TRUE;
#ifdef Dbg
		DebugLog("chat msg");
#endif
	}

	//消息内容:
	if (isSysMsg)
	{
		WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x734);
		DWORD len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			msg = MyWideCharToMultiByte((BYTE*)wxBuf, len);
#ifdef Dbg
			DebugLog("msg", (char*)msg.c_str());
#endif
		}
	}
	else if (isChatMsg)
	{
		CHAR *wxBuf = (CHAR*)*(DWORD*)((char*)saveEBP - 0x204);
		DWORD len = strlen(wxBuf);
		if (len> 0)
		{
			msg.append(wxBuf, len);
#ifdef Dbg
			DebugLog("msg", (char*)msg.c_str());
#endif
		}
	}
	else if (isGroupMsg)
	{
		CHAR *wxBuf = (CHAR*)*(DWORD*)((char*)saveEBP - 0x204);
		DWORD len = strlen(wxBuf);
		if (len> 0)
		{
			msg.append(wxBuf, len);
#ifdef Dbg
			DebugLog("msg", (char*)msg.c_str());
#endif
		}
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
	else
	{
		//isUnknow
		msg.append("unknow msg");
	}
	std::string allmsg = "";
	allmsg.append(sender.c_str());
	allmsg.append(receiver.c_str());
	allmsg.append(msg.c_str());
	pipe_start_thread(allmsg);

	__asm
	{
		mov esp,ebp
		pop ebp
		jmp pfnWxRecvMsg;
	}
}

//原始数据流（protobuf结构）
VOID 
WINAPI 
OnWxRecvMsgForStream()
{
#ifdef Dbg
	DebugLog("HookCallBack", "OnWxRecvMsgForStream...");
#endif

	DWORD* saveEBP = 0;
	__asm
	{
		mov saveEBP, ebp
	}
	saveEBP = (DWORD*)*saveEBP;

	//hook
	CHAR *wxBuf = (CHAR*)*(DWORD*)((char*)saveEBP - 0x794);
	DWORD len = *((DWORD*)(*((DWORD*)((char*)saveEBP - 0x798))) + 2);
	if (len> 0)
	{
#ifdef Dbg
		DebugLogHex("msg",wxBuf, len);
#endif
		std::string msg = "";
		msg.append(wxBuf, len);
		pipe_start_thread(msg);
	}
	__asm
	{
		mov esp, ebp
		pop ebp
		jmp pfnWxRecvMsgForStream;
	}
}