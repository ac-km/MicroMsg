#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryFullAttributesFile.h"
#include"../../Common/DebugLog.h"
#include"../HookPort.h"



//
//Global
//
__pfnNtQueryFullAttributesFile pfnNtQueryFullAttributesFile = NULL;



//
//Dispatch_NTDLL_NtQueryFullAttributesFile Functions
//
NTSTATUS
NTAPI
OnNtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation)
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
		nRet = pfnNtQueryFullAttributesFile(
			ObjectAttributes,
			FileInformation
			);

		return nRet;
	}
	nRet = pfnNtQueryFullAttributesFile(
		ObjectAttributes,
		FileInformation
		);
	return nRet;
}

NTSTATUS
NTAPI
NtQueryFullAttributesFile(
	IN POBJECT_ATTRIBUTES  ObjectAttributes,
	OUT PFILE_NETWORK_OPEN_INFORMATION  FileInformation)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryFullAttributesFile addFun = (__pfnNtQueryFullAttributesFile)GetProcAddress(hDll,"NtQueryFullAttributesFile");
	if(addFun)
	{
		ret = addFun(ObjectAttributes,FileInformation);
	}
	return ret;
}