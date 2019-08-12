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
	DebugLog(DbgInfo, L"receive text msg");
#endif

	DWORD* saveEBP = 0/*(DWORD*)OnWxRecvMsg*/;
	__asm
	{
		mov saveEBP,ebp
	}
	saveEBP = (DWORD*)*saveEBP;

	BOOL isGroupMsg = FALSE;

	//0x294：消息类型

	//0x2E8:群聊的话为房间号，私聊为发送方
	WCHAR *wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x2E8);
	DWORD len = GetWBufferLen(wxBuf);
	if (len> 0)
	{
		WCHAR *buf = (WCHAR*)malloc(len);
		memcpy(buf, wxBuf, len);
		//判断是否为群聊
		std::wstring str;
		str.append(buf, len / 2);
		if (str.find(L"@chatroom")!=std::string::npos)
		{
			isGroupMsg = TRUE;
		}
		OutputDebugStringW(buf);
		free(buf);
	}

	//接收方
	wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x2FC);
	len = GetWBufferLen(wxBuf);
	if (len> 0)
	{
		WCHAR *buf = (WCHAR*)malloc(len);
		memcpy(buf, wxBuf, len);
		OutputDebugStringW(buf);
		free(buf);
	}

	//消息内容
	wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x460);
	len = GetWBufferLen(wxBuf);
	if (len> 0)
	{
		WCHAR *buf = (WCHAR*)malloc(len);
		memcpy(buf, wxBuf, len);
		OutputDebugStringW(buf);
		free(buf);
	}

	//0x3AC：未知的串

	//0x3C0:如果是群聊，此处为发送方wxid
	if (isGroupMsg)
	{
		wxBuf = (WCHAR*)*(DWORD*)((char*)saveEBP - 0x3C0);
		len = GetWBufferLen(wxBuf);
		if (len> 0)
		{
			WCHAR *buf = (WCHAR*)malloc(len);
			memcpy(buf, wxBuf, len);
			OutputDebugStringW(buf);
			free(buf);
		}
	}

	__asm
	{
		mov esp,ebp
		pop ebp
		jmp pfnWxRecvMsg;
	}
}