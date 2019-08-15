#include <windows.h>
#include<Psapi.h>

#include"../HookPort.h"
#include "../InlineHook.h"
#include "../../Common/DebugLog.h"
#include"../SELF/Dispatch_SELF_Login.h"
#include"Dispatch_SELF_RecvMsg.h"




//
//Global
//
BOOL bSELFPatched = FALSE;


//
//Dispatch WeChatWin.dll Functions
//
int Dispatch_SELF_Start(HANDLE hWeChatWin)
{
	//Return Value:
	//-1 = error
	//0 = patched
	//1 = succeed

	if (bSELFPatched == TRUE)
	{
		return 0;
	}

	//
	//Patch API
	//

	//WxLoginForHeadUrl
	InlineHookFunc(
		(__pfnWxLogin)((DWORD)hWeChatWin + 0x1F1D6B),
		OnWxLoginForHeadUrl,
		(void **)&pfnWxLoginForHeadUrl
		);

	//WxLoginForWxid
	InlineHookFunc(
		(__pfnWxLogin)((DWORD)hWeChatWin + 0x2B770F),
		OnWxLoginForWxid,
		(void **)&pfnWxLoginForWxid
	);

	//WxRecvMsg
	InlineHookFunc(
		(__pfnWxRecvMsg)((DWORD)hWeChatWin + 0x3114DB),
		OnWxRecvMsg,
		(void **)&pfnWxRecvMsg
		);
	
	return 1;
}