#include <windows.h>
#include <ntsecapi.h>

#include "../InlineHook.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include"../SELF/Dispatch_SELF.h"




void Dispatch_KERNEL32_Start_SELF_OnWxRecvMsg(PHANDLE ph)
{
	//hookÄ¿±ê³ÌÐò
	Dispatch_SELF_Start(*ph);
}