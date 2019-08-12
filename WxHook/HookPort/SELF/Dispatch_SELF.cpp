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

	//WxLogin
	InlineHookFunc(
		(__pfnWxLogin)((DWORD)hWeChatWin + 0x2934F4),
		OnWxLogin,
		(void **)&pfnWxLogin
		);

	//WxRecvMsg
	InlineHookFunc(
		(__pfnWxRecvMsg)((DWORD)hWeChatWin + 0x2D2C1D),
		OnWxRecvMsg,
		(void **)&pfnWxRecvMsg
		);

	//·½·¨2
	
	return 1;
}