// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include"HookPort.h"
#include"Common.h"
#include"../Common/DebugLog.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hinstDLL = hModule;

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (HookPort_Start_DllMain() == FALSE)
			{
				ExitProcess(0);
			}
		}break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

