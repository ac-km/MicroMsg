#include <windows.h>
#include <ntsecapi.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "./Dispatch_NTDLL_NtCreateKey.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"
#include "../NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtOpenKey.h"
#include "./Dispatch_NTDLL_NtClose.h"



//
//Global
//
__pfnNtCreateKey pfnNtCreateKey = NULL;



//
//Dispatch_NTDLL_NtCreateKey Functions
//
NTSTATUS
NTAPI
OnNtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    )
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtCreateKey(
			KeyHandle,
			DesiredAccess,
			ObjectAttributes,
			TitleIndex,
			Class,
			CreateOptions,
			Disposition
			);

		return nRet;
	}

	nRet = pfnNtCreateKey(
		KeyHandle,
		DesiredAccess,
		ObjectAttributes,
		TitleIndex,
		Class,
		CreateOptions,
		Disposition
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateKey(
	OUT PHANDLE  KeyHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	IN ULONG  TitleIndex,
	IN PUNICODE_STRING  Class  OPTIONAL,
	IN ULONG  CreateOptions,
	OUT PULONG  Disposition  OPTIONAL
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateKey addFun = (__pfnNtCreateKey)GetProcAddress(hDll,"NtCreateKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,DesiredAccess,ObjectAttributes,TitleIndex,Class,CreateOptions,Disposition);
	}
	return ret;
}