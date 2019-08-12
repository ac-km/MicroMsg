#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_USER32_SendMessage.h"
#include "../../Common/DebugLog.h"
#include"../HookPort.h"



//
//Global
//
__pfnSendMessageA pfnSendMessageA = NULL;
__pfnSendMessageW pfnSendMessageW = NULL;



//
//Dispatch_USER32_SendMessage Functions
//
LRESULT
WINAPI
OnSendMessageA(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam)
{
	LRESULT lRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		lRet = pfnSendMessageA(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		lRet = FALSE;
		return lRet;
	}

	//
	//Check if is [WM_SETTEXT] command
	//
	if( Msg != WM_SETTEXT )
	{
		lRet = pfnSendMessageA(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}

	//
	//Check if includes [WS_CAPTION]&[WS_VISIBLE]&[WS_SYSMENU]&[WS_CLIPSIBLINGS] Window Styles
	//
	LONG lStylesValue = GetWindowLong(hWnd,GWL_STYLE);
	if( lStylesValue == 0 )
	{
		lRet = pfnSendMessageA(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}
	if( (lStylesValue & WS_CAPTION) == 0 ||
		(lStylesValue & WS_VISIBLE) == 0 ||
		(lStylesValue & WS_SYSMENU) == 0
		)
	{
		lRet = pfnSendMessageA(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}
	lRet = pfnSendMessageA(
		hWnd,
		Msg,
		wParam,
		lParam
		);

	return lRet;
}

LRESULT
WINAPI
OnSendMessageW(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam)
{
	LRESULT lRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		lRet = pfnSendMessageW(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}

	//
	//Check if target process in SandBox
	//
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if( IsProcessPatched(dwProcessId,FALSE) == FALSE )
	{
		lRet = FALSE;
		return lRet;
	}

	//
	//Check if is [WM_SETTEXT] command
	//
	if( Msg != WM_SETTEXT )
	{
		lRet = pfnSendMessageW(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}

	//
	//Check if includes [WS_CAPTION]&[WS_VISIBLE]&[WS_SYSMENU]&[WS_CLIPSIBLINGS] Window Styles
	//
	LONG lStylesValue = GetWindowLong(hWnd,GWL_STYLE);
	if( lStylesValue == 0 )
	{
		lRet = pfnSendMessageW(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}
	if( (lStylesValue & WS_CAPTION) == 0 ||
		(lStylesValue & WS_VISIBLE) == 0 ||
		(lStylesValue & WS_SYSMENU) == 0
		)
	{
		lRet = pfnSendMessageW(
			hWnd,
			Msg,
			wParam,
			lParam
			);

		return lRet;
	}
	lRet = pfnSendMessageW(
		hWnd,
		Msg,
		wParam,
		lParam
		);

	return lRet;
}