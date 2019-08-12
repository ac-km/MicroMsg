#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"



//
//Global
//
__pfnNtQueryAttributesFile pfnNtQueryAttributesFile = NULL;



//
//Dispatch_NTDLL_NtQueryAttributesFile Functions
//
NTSTATUS
NTAPI
OnNtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes)
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
		nRet = pfnNtQueryAttributesFile(
			ObjectAttributes,
			FileAttributes
			);

		return nRet;
	}
	nRet = pfnNtQueryAttributesFile(
		ObjectAttributes,
		FileAttributes
		);
	return nRet;
}

NTSTATUS
NTAPI
NtQueryAttributesFile(
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	OUT PFILE_BASIC_INFORMATION FileAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryAttributesFile addFun = (__pfnNtQueryAttributesFile)GetProcAddress(hDll,"NtQueryAttributesFile");
	if(addFun)
	{
		ret = addFun(ObjectAttributes,FileAttributes);
	}
	return ret;
}