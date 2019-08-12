#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_SetWindowText.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"



//
//Global
//
__pfnSetWindowTextA pfnSetWindowTextA = NULL;
__pfnSetWindowTextW pfnSetWindowTextW = NULL;



//
//Dispatch_USER32_SetWindowText Functions
//
BOOL
WINAPI
OnSetWindowTextA(
    IN HWND hWnd,
    IN LPCSTR lpString)
{
	BOOL bRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		bRet = pfnSetWindowTextA(
			hWnd,
			lpString
			);

		return bRet;
	}
	bRet = pfnSetWindowTextA(
		hWnd,
		lpString
		);
	return bRet;
}

BOOL
WINAPI
OnSetWindowTextW(
    IN HWND hWnd,
    IN LPCWSTR lpString)
{
	BOOL bRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		bRet = FALSE;
		return bRet;
	}

	//
	//Check if includes [WS_CAPTION] Window Styles
	//
	LONG lStylesValue = GetWindowLong(hWnd,GWL_STYLE);
	if( lStylesValue == 0 )
	{
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}
	if( (lStylesValue & WS_CAPTION) == 0 )
	{
		bRet = pfnSetWindowTextW(
			hWnd,
			lpString
			);

		return bRet;
	}
	bRet = pfnSetWindowTextW(
		hWnd,
		lpString
		);
	return bRet;
}