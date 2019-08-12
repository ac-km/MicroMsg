#include <windows.h>
#include <ntsecapi.h>
#include"../HookPort.h"
#include "./Dispatch_NTDLL_NtSetValueKey.h"
#include"../../Common/DebugLog.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "../CloneAPI/CloneAPI_ADVAPI32.h"



//
//Global
//
__pfnNtSetValueKey pfnNtSetValueKey = NULL;



//
//Dispatch_NTDLL_NtSetValueKey Functions
//
NTSTATUS
NTAPI
OnNtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
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
		nRet = pfnNtSetValueKey(
			KeyHandle,
			ValueName,
			TitleIndex,
			Type,
			Data,
			DataSize
			);

		return nRet;
	}

	nRet = pfnNtSetValueKey(
		KeyHandle,
		ValueName,
		TitleIndex,
		Type,
		Data,
		DataSize
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN ULONG  TitleIndex  OPTIONAL,
	IN ULONG  Type,
	IN PVOID  Data,
	IN ULONG  DataSize
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetValueKey addFun = (__pfnNtSetValueKey)GetProcAddress(hDll,"NtSetValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValueName,TitleIndex,Type,Data,DataSize);
	}
	return ret;
}