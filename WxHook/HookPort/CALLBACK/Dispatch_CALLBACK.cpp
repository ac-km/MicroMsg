#include <windows.h>
#include <ntsecapi.h>

#include "../InlineHook.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"
#include"../SELF/Dispatch_SELF.h"




void Dispatch_KERNEL32_Start_SELF_OnWxRecvMsg(PHANDLE ph)
{
	//hookĿ�����
	Dispatch_SELF_Start(*ph);
}