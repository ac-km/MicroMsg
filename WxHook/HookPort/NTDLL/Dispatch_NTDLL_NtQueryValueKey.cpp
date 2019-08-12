#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryValueKey.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"
#include"../CloneAPI/CloneAPI_ADVAPI32.h"



//
//Global
//
__pfnNtQueryValueKey pfnNtQueryValueKey = NULL;



//
//Dispatch_NTDLL_NtQueryValueKey Functions
//
NTSTATUS
NTAPI
OnNtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
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
		nRet = pfnNtQueryValueKey(
			KeyHandle,
			ValueName,
			KeyValueInformationClass,
			KeyValueInformation,
			Length,
			ResultLength
			);

		return nRet;
	}
	nRet = pfnNtQueryValueKey(
		KeyHandle,
		ValueName,
		KeyValueInformationClass,
		KeyValueInformation,
		Length,
		ResultLength
		);
	return nRet;
}

NTSTATUS
NTAPI
NtQueryValueKey(
	IN HANDLE  KeyHandle,
	IN PUNICODE_STRING  ValueName,
	IN KEY_VALUE_INFORMATION_CLASS  KeyValueInformationClass,
	OUT PVOID  KeyValueInformation,
	IN ULONG  Length,
	OUT PULONG  ResultLength
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryValueKey addFun = (__pfnNtQueryValueKey)GetProcAddress(hDll,"NtQueryValueKey");
	if(addFun)
	{
		ret = addFun(KeyHandle,ValueName,KeyValueInformationClass,KeyValueInformation,Length,ResultLength);
	}
	return ret;
}